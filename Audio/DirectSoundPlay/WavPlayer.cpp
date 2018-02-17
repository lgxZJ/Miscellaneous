#include "WavPlayer.h"

#include <QtGlobal>

#include <cassert>

WavPlayer::WavPlayer()
	: m_wavFile()
	, m_windowHandle(NULL)
	, m_directSound8(nullptr)
    , m_soundBufferInterface(nullptr)
    , m_nextDataToPlay(nullptr)
	, m_notifyOffsets(nullptr)
	, m_notifyHandles(nullptr)
	, m_quitDataFillingThread(false)
    , m_isPlaying(false)
    , m_secondaryBufferSize(0)
	, m_notifyCount(0)
	, m_bufferSliceCount(0)
	, m_threadHandle(NULL)
	, m_additionalEndNotify(false)
	, m_endNotifyHandle(NULL)
	, m_endNotifyLoopCount(0)
	, m_dataFillingEnds(false)
    , m_currentPlayingTime(0)
{
}

WavPlayer::~WavPlayer()
{
    cleanResources(CleanAll);
}

void WavPlayer::cleanResources(WavPlayer::CleanOption option)
{
	//	must call IDirectSound::Stop(), or the data filling thread will not be notified
	if (isPlaying())		
		stop();
    m_currentPlayingTime = 0;
	if (option != CleanNoWav)
		m_wavFile.clean();

	if (m_threadHandle != NULL) {
		if (m_notifyCount > 0 && SetEvent(m_notifyHandles[m_notifyCount - 1]) != 0) {
			//todo:	do log
		}

		m_quitDataFillingThread = true;
		if (WaitForSingleObject(m_threadHandle, INFINITE) != WAIT_OBJECT_0)
			throw std::exception("WaitForSingleObject error");

		CloseHandle(m_threadHandle);
		m_threadHandle = NULL;
		m_quitDataFillingThread = false;
	}
	m_dataFillingEnds = false;

	//	no need to release secondary buffer, device object do it
	if (m_directSound8 != nullptr)
		m_directSound8->Release(),
		m_directSound8 = nullptr;

	if (m_notifyCount > 0) {
		for (unsigned i = 0; i < m_notifyCount; ++i)
			if (m_notifyHandles != nullptr && m_notifyHandles[i] != nullptr)
				CloseHandle(m_notifyHandles[i]);

		if (m_notifyHandles != nullptr)
			free(m_notifyHandles),
			m_notifyHandles = nullptr;
		if (m_notifyOffsets != nullptr)
			free(m_notifyOffsets),
			m_notifyOffsets = nullptr;

		m_notifyCount = 0;
	}
	m_additionalEndNotify = false;
}

//////////////////////////////////////////////////////////////////////////////////

void WavPlayer::setFile(std::wstring filePath, HWND windowHandle)
{
    Q_ASSERT (!filePath.empty() && windowHandle != NULL);

	if (m_isPlaying)				stop();
    if (fileSet())					cleanResources(CleanAll);

    m_wavFile.open(filePath);
	m_nextDataToPlay = static_cast<char*>(m_wavFile.getAudioData());

	m_windowHandle = windowHandle;
    preparePlayResource();
}

void WavPlayer::preparePlayResource()
{
	Q_ASSERT(m_windowHandle != NULL);

	m_notifyCount = m_bufferSliceCount = 4;

    openDefaultAudioDevice(m_windowHandle);
    createBufferOfSeconds(s_secondsInBuffer);
    prefillDataIntoBuffer();

	auto startDataPtrForNotifyCalculation = m_nextDataToPlay -
				(m_secondaryBufferSize / m_bufferSliceCount * s_prefilledBufferSliceCount);
    startDataFillingThread(startDataPtrForNotifyCalculation);
}

//////////////////////////////////////////////////////////////////////////////////

void WavPlayer::play()
{
    assert (m_soundBufferInterface != nullptr && !m_isPlaying);

	//	when the buffer is created, the play cursor is set to 0 automatically,
	//	on the later playing DirectSound internally keeps the play cursor when
	//	Play() is called.
	if (m_soundBufferInterface->Play(0, 0, DSBPLAY_LOOPING) != DS_OK)
        throw std::exception("Play error");
	m_isPlaying = true;
}

void WavPlayer::sendProgressUpdatedSignal()
{
	if (m_outerNotify)
		m_outerNotify->wavPlayerProgressUpdated(m_currentPlayingTime);
	++m_currentPlayingTime;
}

//////////////////////////////////////////////////////////////////////////////////

void WavPlayer::stop()
{
    assert (m_soundBufferInterface != nullptr && m_isPlaying);

    if (m_soundBufferInterface->Stop() != DS_OK)
        throw std::exception("Stop error");
	m_isPlaying = false;
}

//////////////////////////////////////////////////////////////////////////////////

void WavPlayer::resume()
{
    assert (m_soundBufferInterface != nullptr && !m_isPlaying);

	if (m_soundBufferInterface->Play(0, 0, DSBPLAY_LOOPING) != DS_OK)
        throw std::exception("Stop error");
	m_isPlaying = true;
}

//////////////////////////////////////////////////////////////////////////////////

//	`seconds` start from 1 to totalTime
void WavPlayer::playFrom(unsigned seconds)
{
    Q_ASSERT(seconds >= 0 &&
             seconds < m_wavFile.getAudioTime());

	auto playing = isPlaying();

    cleanResources(CleanNoWav);
	m_currentPlayingTime = seconds;
	m_nextDataToPlay = static_cast<char*>(m_wavFile.getAudioData())
							+ (m_secondaryBufferSize / s_secondsInBuffer) * seconds;
    preparePlayResource();

	if (playing)	play();
}

//////////////////////////////////////////////////////////////////////////////////

void WavPlayer::setAudioEndsNotify(IAudioEndNotify* outerNotify)
{
	assert (outerNotify != NULL);

	m_outerNotify = outerNotify;
}

//////////////////////////////////////////////////////////////////////////////////

void WavPlayer::openDefaultAudioDevice(HWND windowHandle)
{
    if (DirectSoundCreate8(&DSDEVID_DefaultPlayback, &m_directSound8, NULL) != DS_OK) {
        throw std::exception("Error: maybe no default audio device in your system");
    }

    if (m_directSound8->SetCooperativeLevel(windowHandle, DSSCL_PRIORITY) != DS_OK) {
        throw std::exception("set cooperative level on default audio device failed!");
    }
}

void WavPlayer::createBufferOfSeconds(unsigned seconds)
{
    DSBUFFERDESC bufferDescription;
    bufferDescription.dwSize = sizeof(bufferDescription);
    bufferDescription.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY |
                                DSBCAPS_GLOBALFOCUS |
                                DSBCAPS_GETCURRENTPOSITION2 |
                                DSBCAPS_LOCDEFER ;
    bufferDescription.dwBufferBytes = m_secondaryBufferSize
                                    = m_wavFile.getWaveFormat().nAvgBytesPerSec * seconds;
    bufferDescription.dwReserved = 0;
	bufferDescription.lpwfxFormat = &m_wavFile.getWaveFormat();
    bufferDescription.guid3DAlgorithm = GUID_NULL;

    IDirectSoundBuffer* soundBuffer;
    if (m_directSound8->CreateSoundBuffer(&bufferDescription, &soundBuffer, NULL) != DS_OK) {
        throw std::exception("create secondary buffer failed:CreateSoundBuffer");
    }

    if (soundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_soundBufferInterface)
            != S_OK) {
        throw std::exception("IDirectSoundBuffer8 interface not supported!");
    }
}

void WavPlayer::prefillDataIntoBuffer()
{
	Q_ASSERT(m_bufferSliceCount > 1);

    //  fill half buffer to signal the notify event to do next data filling
    LPVOID firstAudioAddress;
    LPVOID secondAudioAddress;
    DWORD  firstAudioBytes;
    DWORD  secondAudioBytes;
    HRESULT result = m_soundBufferInterface->Lock(0,
									m_secondaryBufferSize / m_bufferSliceCount * s_prefilledBufferSliceCount,
                                    &firstAudioAddress, &firstAudioBytes,
                                    &secondAudioAddress, &secondAudioBytes,
                                    0);
    if (result == DSERR_BUFFERLOST) {
		if (m_soundBufferInterface->Restore() == DS_OK) {
			result = m_soundBufferInterface->Lock(0,
				m_secondaryBufferSize / m_bufferSliceCount * s_prefilledBufferSliceCount,
				&firstAudioAddress, &firstAudioBytes,
				&secondAudioAddress, &secondAudioBytes,
				0);
		}
    }
    if (result != DS_OK) {
        throw std::exception("Cannot lock entire secondary buffer(restore tryed)");
    }

	Q_ASSERT(firstAudioBytes == (m_secondaryBufferSize / m_bufferSliceCount * s_prefilledBufferSliceCount) &&
             secondAudioAddress == nullptr &&
             secondAudioBytes == 0);
    CopyMemory(firstAudioAddress, m_nextDataToPlay, firstAudioBytes);
    if (m_soundBufferInterface->Unlock(firstAudioAddress, firstAudioBytes,
                                       secondAudioAddress, secondAudioBytes)
            != DS_OK) {
        throw std::exception("Unlick failed when fill data into secondary buffer");
    }

	m_nextDataToPlay += firstAudioBytes;
}

//  set notify event to stream the sound audio playing
void WavPlayer::startDataFillingThread(char* startDataPtr)
{
	Q_ASSERT(m_notifyCount > 1);
	Q_ASSERT(m_bufferSliceCount > 1);

	DWORD entireBufferLoopCount = (m_wavFile.getDataEndPtr() - startDataPtr) / m_secondaryBufferSize;
	DWORD bufferEndOffset = (m_wavFile.getDataEndPtr() - startDataPtr) % m_secondaryBufferSize;

	//	the offset of end notify NOT locate in the data filling notify offset
	m_additionalEndNotify = (bufferEndOffset % (m_secondaryBufferSize / m_bufferSliceCount))
									!= 0;
	m_notifyCount += m_additionalEndNotify;
	m_endNotifyLoopCount = entireBufferLoopCount + 1;

	//	todo: bug here
	m_additionalNotifyIndex = 0;
	if (m_additionalEndNotify)
		for (unsigned i = 1; i < m_bufferSliceCount; ++i)
			if (bufferEndOffset < (m_secondaryBufferSize / m_bufferSliceCount * i)) {
				m_additionalNotifyIndex = i;
				break;
			}
	
	//	add a stop notify count at the end of entire notifies to make the data filling
	//	thread exit gracefully
	++m_notifyCount;
	m_notifyHandles = static_cast<HANDLE*>(malloc(sizeof(HANDLE)* (m_notifyCount)));
	if (m_notifyHandles == nullptr)
		throw std::exception("malloc error");
	m_notifyOffsets = static_cast<DWORD*>(malloc(sizeof(DWORD)* (m_notifyCount)));
	if (m_notifyOffsets == nullptr)
		throw std::exception("malloc error");

	for (unsigned i = 0; i < m_notifyCount; ++i) {
		m_notifyHandles[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (m_notifyHandles[i] == NULL)
			throw std::exception("CreateEvent error");

		if (m_additionalEndNotify && i == m_additionalNotifyIndex) {
			//	set buffer end notify
			m_notifyOffsets[i] = bufferEndOffset;
			m_endNotifyHandle = m_notifyHandles[i];
		}
		else if (i == m_notifyCount - 1) {
			//	do nothing
		} else {
			//	NOTE:	the entire buffer size must can be devided by this `notifyCount`,
			//	or it will lost some bytes when filling data into the buffer. since the end
			//	notify is inside the notify count, we need to calculate the buffer slice index.
			unsigned bufferSliceIndex = getBufferIndexFromNotifyIndex(i);
			m_notifyOffsets[i] = m_secondaryBufferSize / m_bufferSliceCount * bufferSliceIndex;
			
			if (!m_additionalEndNotify && m_notifyOffsets[i] == bufferEndOffset)
				m_endNotifyHandle = m_notifyHandles[i];
		}
	}
	//	skip the exit notify which we toggle explicitly
	setNotifyEvent(m_notifyHandles, m_notifyOffsets, m_notifyCount - 1);

	if ((m_threadHandle = CreateThread(NULL, NULL, &dataFillingThread, this, 0, NULL))
			== NULL) {
		CloseHandle(m_notifyHandles[0]);
		CloseHandle(m_notifyHandles[1]);
		throw std::exception("Data filling thread create failed");
	}
}

//	the end notify is the last notify and is already judged before calling this function
unsigned WavPlayer::getBufferIndexFromNotifyIndex(unsigned notifyIndex)
{
	return notifyIndex > m_additionalNotifyIndex ? (notifyIndex - 1) : notifyIndex;
}

void WavPlayer::setNotifyEvent(HANDLE* notifyHandles, DWORD* bufferOffsets, unsigned num)
{
	IDirectSoundNotify8* notifyInterface;
    if (m_soundBufferInterface->QueryInterface(IID_IDirectSoundNotify8, (LPVOID*)&notifyInterface)
            != S_OK) {
        throw std::exception("cannot get notify interface");
    }

	DSBPOSITIONNOTIFY* positionNotifies = (DSBPOSITIONNOTIFY*)malloc(sizeof(DSBPOSITIONNOTIFY)* num);
	if (positionNotifies == nullptr) {
		throw std::exception("malloc error");
	}

	for (unsigned i = 0; i < num; ++i) {
		positionNotifies[i].dwOffset = bufferOffsets[i];
		positionNotifies[i].hEventNotify = notifyHandles[i];
	}

    if (notifyInterface->SetNotificationPositions(num, &positionNotifies[0]) != DS_OK) {
        notifyInterface->Release();
        throw std::exception("SetNotificationPositions failed");
    }

    notifyInterface->Release();
}

DWORD WINAPI WavPlayer::dataFillingThread(LPVOID param)
{
	WavPlayer* wavPlayer = reinterpret_cast<WavPlayer*>(param);

	while (!wavPlayer->m_quitDataFillingThread) {
		try {
			DWORD notifyIndex = WaitForMultipleObjects(wavPlayer->m_notifyCount, wavPlayer->m_notifyHandles, FALSE, INFINITE);
			if (!(notifyIndex >= WAIT_OBJECT_0 &&
				  notifyIndex <= WAIT_OBJECT_0 + wavPlayer->m_notifyCount - 1))

				throw std::exception("WaitForSingleObject error");

			if (notifyIndex == wavPlayer->m_notifyCount - 1)
				break;

			//	each notify represents one second(or approximately one second) except the exit notify
			if (!(wavPlayer->m_additionalNotifyIndex == notifyIndex && wavPlayer->m_endNotifyLoopCount > 0))
				wavPlayer->sendProgressUpdatedSignal();

			//	if return false, the audio ends
			if (tryToFillNextBuffer(wavPlayer, notifyIndex) == false) {
				wavPlayer->stop();
                wavPlayer->sendProgressUpdatedSignal();
				wavPlayer->sendAudioEndsSignal();
				//	not break the loop, we need to update the audio progress although data filling ends
			}
		}
		catch (std::exception& exception) {
			OutputDebugStringA("exception in data filling thread:");
			OutputDebugStringA(exception.what());
		}
	}
	return 0;
}

void WavPlayer::sendAudioEndsSignal()
{
	if (m_outerNotify)
		m_outerNotify->wavPlayerAudioEnds();
}

//	if it return false, it fills less than half buffer size data, which indicates the audio is ending
bool WavPlayer::tryToFillNextBuffer(WavPlayer* wavPlayer, unsigned notifyEventIndex)
{
	if (wavPlayer->m_notifyHandles[notifyEventIndex] == wavPlayer->m_endNotifyHandle) {
		if (--wavPlayer->m_endNotifyLoopCount == 0)			return false;

		//	additional end notify event do no fill data into buffer
		if (wavPlayer->m_additionalEndNotify)	return true;	
	}

	if (wavPlayer->m_dataFillingEnds)
		return true;

	DWORD bufferSliceSize = wavPlayer->m_secondaryBufferSize / wavPlayer->m_bufferSliceCount;
	DWORD filledDataSize = bufferSliceSize;
	if ((wavPlayer->m_nextDataToPlay + bufferSliceSize) >= wavPlayer->m_wavFile.getDataEndPtr()) {
		filledDataSize = wavPlayer->m_wavFile.getDataEndPtr() - wavPlayer->m_nextDataToPlay + 1;
		wavPlayer->m_dataFillingEnds = true;
	}

	lockAndFillData(wavPlayer, 
					wavPlayer->m_nextDataToPlay, 
					filledDataSize, 
					wavPlayer->getBufferIndexFromNotifyIndex(notifyEventIndex));
	wavPlayer->m_nextDataToPlay += filledDataSize;
	return true;
}

void WavPlayer::lockAndFillData(WavPlayer* wavPlayer, char* dataPtr, DWORD dataSizeInBytes, unsigned bufferSliceIndex)
{
	LPVOID firstAudioAddress;
	LPVOID secondAudioAddress;
	DWORD  firstAudioBytes;
	DWORD  secondAudioBytes;
	DWORD  bufferOffset = wavPlayer->m_secondaryBufferSize / wavPlayer->m_bufferSliceCount *
						  ((bufferSliceIndex + s_prefilledBufferSliceCount) % wavPlayer->m_bufferSliceCount);
	HRESULT result = wavPlayer->m_soundBufferInterface->Lock(
							bufferOffset,
							dataSizeInBytes,
							&firstAudioAddress, &firstAudioBytes,
							&secondAudioAddress, &secondAudioBytes,
							0);
	if (result == DSERR_BUFFERLOST) {
		if (wavPlayer->m_soundBufferInterface->Restore() == DS_OK) {
			result = wavPlayer->m_soundBufferInterface->Lock(
				bufferOffset,
				dataSizeInBytes,
				&firstAudioAddress, &firstAudioBytes,
				&secondAudioAddress, &secondAudioBytes,
				0);
		}
	}
	if (result != DS_OK) {
		throw std::exception("Cannot lock entire secondary buffer(restore tryed)");
	}
			
	CopyMemory(firstAudioAddress, dataPtr, firstAudioBytes);
	dataPtr += firstAudioBytes;
	CopyMemory(secondAudioAddress, dataPtr, secondAudioBytes);

	if (wavPlayer->m_soundBufferInterface->Unlock(	firstAudioAddress, firstAudioBytes,
													secondAudioAddress, secondAudioBytes )
			!= DS_OK) {
		throw std::exception("Unlick failed when fill data into secondary buffer");
	}
}
