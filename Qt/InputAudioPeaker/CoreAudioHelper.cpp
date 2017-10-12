#include "CoreAudioHelper.h"

//////////////////////////////////////////////////////////////////////////////////////////////

#define     CHECK_RETURN(returnValue)													\
		if (returnValue == MMSYSERR_NODRIVER) {											\
			emit error("no driver present");											\
			return false;																\
		}																				\
		if (returnValue != MMSYSERR_NOERROR) {											\
			return false;																\
		}

//////////////////////////////////////////////////////////////////////////////////////////////

CoreAudioHelper* CoreAudioHelper::instance()
{
	static CoreAudioHelper* instance = NULL;

	if (instance == NULL) {
		instance = new CoreAudioHelper();
	}
	return instance;
}

CoreAudioHelper::CoreAudioHelper()
	: m_currentDeviceIndex(-1)
    , m_is8BitsSample(false)
	, m_bufferFilled(1)
	, m_stopThread(false)
	, m_emitUnplugged(false)
	, m_bufferWaiterThread(NULL)
{
	//	init the available resource to 0
	m_bufferFilled.acquire(1);
}

CoreAudioHelper::~CoreAudioHelper()
{
	for (auto oneInfo : m_infos) {
		if (oneInfo.handle != NULL) {
			if (oneInfo.started)
				waveInStop(oneInfo.handle);
			waveInClose(oneInfo.handle);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

QList<AudioDeviceInfo> CoreAudioHelper::getInputDevices()
{
    auto inputAudioDeviceNum = waveInGetNumDevs();
    QList<AudioDeviceInfo> result;

    for (int i = 0; i < inputAudioDeviceNum; ++i) {
        WAVEINCAPS waveInCaps;
        auto returnValue = waveInGetDevCaps(i, &waveInCaps, sizeof(waveInCaps)) ;

        if (returnValue == MMSYSERR_NODRIVER) {
            return result;
        }
        if (returnValue != MMSYSERR_NOERROR) {
            emit error("no driver present");
            return result;
        }

        AudioDeviceInfo deviceInfo;
        deviceInfo.id = i;
        deviceInfo.name = QString::fromStdWString(waveInCaps.szPname);
        deviceInfo.waveFormat = waveInCaps.dwFormats;
        result.push_back(deviceInfo);
    }

    m_infos = result;
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////

bool CoreAudioHelper::startInputDevice(UINT index)
{
    auto& deviceInfo = m_infos.at(index);
	Q_ASSERT(deviceInfo.handle == NULL && deviceInfo.started == false);

    m_currentDeviceIndex = index;
    WAVEFORMATEX waveFormatEx = chooseAppropriateFormat();
    if (waveFormatEx.nChannels == -1) {
        emit error("no appropriate pcm foramt");
        return false;
    }

    auto returnValue = waveInOpen((LPHWAVEIN)&deviceInfo.handle, index, &waveFormatEx,
                                  (DWORD_PTR)CoreAudioHelper::waveInProc,
                                  (DWORD_PTR)this, 
								  CALLBACK_FUNCTION);
    CHECK_RETURN(returnValue);

	startPeakGetter();

	m_stopThread = false;
	m_bufferWaiterThread = new BufferWaiterThread(this);
	m_bufferWaiterThread->start();
    return true;
}

WAVEFORMATEX CoreAudioHelper::chooseAppropriateFormat()
{
    //  common suported formats
    static DWORD formats[20] = {
        WAVE_FORMAT_1M08, WAVE_FORMAT_1S08, WAVE_FORMAT_1M16, WAVE_FORMAT_1S16,
        WAVE_FORMAT_2M08, WAVE_FORMAT_2S08, WAVE_FORMAT_2M16, WAVE_FORMAT_2S16,
        WAVE_FORMAT_44M08,WAVE_FORMAT_44S08,WAVE_FORMAT_44M16,WAVE_FORMAT_44S16,
        WAVE_FORMAT_48M08,WAVE_FORMAT_48S08,WAVE_FORMAT_48M16,WAVE_FORMAT_48S16,
        WAVE_FORMAT_96M08,WAVE_FORMAT_96S08,WAVE_FORMAT_96M16,WAVE_FORMAT_96S16,
    };
    static WORD channels[20] = {
        1,2,1,2,    1,2,1,2,    1,2,1,2,    1,2,1,2,   1,2,1,2,
    };
    static DWORD samplePerSec[20] = {
        11025,11025,11025,11025,
        22050,22050,22050,22050,
        44100,44100,44100,44100,
        48000,48000,48000,48000,
        96000,96000,96000,96000,
    };
    static WORD bitsPerSample[20] ={
        8,8,16,16,  8,8,16,16,  8,8,16,16,  8,8,16,16,  8,8,16,16,
    };


    WAVEFORMATEX waveFormatEx;
    waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
    //  use -1 to indicate error
    waveFormatEx.nChannels = -1;

    auto deviceFormatFlags = m_infos.at(m_currentDeviceIndex).waveFormat;
    for (int i = 0; i < 20; ++i) {
        if (deviceFormatFlags & formats[i]) {
            waveFormatEx.nChannels = channels[i];
            waveFormatEx.nSamplesPerSec = samplePerSec[i];
            waveFormatEx.wBitsPerSample = bitsPerSample[i];

            if (bitsPerSample[i] == 8)
				m_is8BitsSample = true;
            break;
        }
    }

    waveFormatEx.nBlockAlign = waveFormatEx.nChannels * waveFormatEx.wBitsPerSample / 8;
    waveFormatEx.nAvgBytesPerSec = waveFormatEx.nSamplesPerSec * waveFormatEx.nBlockAlign;
    waveFormatEx.cbSize = 0;
    return waveFormatEx;
}



void CoreAudioHelper::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    switch (uMsg) {
    case WIM_OPEN:
		break;
	case WIM_CLOSE:
	{
		//	when plug-out or waveInClose() called

		CoreAudioHelper* helper = reinterpret_cast<CoreAudioHelper*>(dwInstance);
		auto& deviceInfo = helper->m_infos[helper->m_currentDeviceIndex];

		if (deviceInfo.manuallyClosed == false)
			helper->m_emitUnplugged = true;
		helper->clearInfo(helper->m_currentDeviceIndex);
		break;
	}
    case WIM_DATA:
	{
        //  buffer already filled with input audio data
        CoreAudioHelper* helper = reinterpret_cast<CoreAudioHelper*>(dwInstance);
		Q_ASSERT(helper->m_waveHdr.dwFlags & WHDR_DONE);

        qint32 peakMin = 255;
		qint32 peakMax = 0;
		for (char* ptr = helper->m_buffer; ptr < &helper->m_buffer[16]; ) {
			qint32 dataValue;
			if (helper->m_is8BitsSample) {
				dataValue = *(unsigned char*)ptr;
				ptr++;
			} else {
				dataValue = *(qint16*)ptr;	
				ptr += 2;
			}
			if (dataValue < peakMin)	peakMin = dataValue;
			if (dataValue > peakMax)	peakMax = dataValue;
		}

		helper->appendPeakValue(max(-peakMin, peakMax)); 

        break;
    }
    default:
        Q_ASSERT(false && "never receive other msg!");
    }

}//todo:remove file output

void CoreAudioHelper::clearInfo(UINT index)
{
	Q_ASSERT(index >= 0 && index < m_infos.size());
	auto& deviceInfo = m_infos[index];

	deviceInfo.manuallyClosed = false;
	deviceInfo.muted = false;
	deviceInfo.started = false;
	deviceInfo.volumeFilterPercent = 0.5;
	deviceInfo.handle = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

bool CoreAudioHelper::stopInputDevice(UINT index)
{
	Q_ASSERT(index >= 0 && index < m_infos.size());

	auto& deviceInfo = m_infos[index];
	if (deviceInfo.handle == NULL)
		return true;

	m_stopThread = true;
	m_bufferWaiterThread->wait();

	if (deviceInfo.started) {
		auto returnValue = waveInStop(deviceInfo.handle);
		CHECK_RETURN(returnValue);

		deviceInfo.started = false;
	}
	
	auto returnValue = waveInClose(deviceInfo.handle); //std::cout << GetLastError(); std::cout << "-" << returnValue;
	CHECK_RETURN(returnValue);

	clearInfo(index);
	resetHelperMembers();
	return true;
}


void CoreAudioHelper::resetHelperMembers()
{
	m_currentDeviceIndex = -1;
	m_is8BitsSample = false;
	m_stopThread = false;
	m_emitUnplugged = false;
	m_bufferWaiterThread = NULL;

	if (!m_peakValueQueue.empty())
		m_peakValueQueue.swap(std::queue<qint16>());
}

/////////////////////////////////////////////////////////////////////////////////////////////

void CoreAudioHelper::emitPeakLevelAndContinue()
{
	if (!m_peakValueQueue.empty()) {
		qint32 peakValue = m_peakValueQueue.front();
		m_peakValueQueue.pop();

		if (!m_infos.at(m_currentDeviceIndex).muted) {
			if (m_is8BitsSample) {
				//	when 8-bit sample, the range is 0--255, the silence data value is 127
				emit peakChanged(qint32(abs(peakValue - 127) / 1.27) * 
								 m_infos.at(m_currentDeviceIndex).volumeFilterPercent);
			}
			else {
				//	when 16-bit sample, the range  is -32767--32767, the silence data value is 0
				emit peakChanged(qint32(abs(peakValue) / 327.67) * 
								 m_infos.at(m_currentDeviceIndex).volumeFilterPercent);
			}
			startPeakGetter();
		}
	}
}

bool CoreAudioHelper::unprepareBuffer()
{
	auto deviceInfo = m_infos.at(m_currentDeviceIndex);
	auto returnValue = waveInUnprepareHeader(deviceInfo.handle, &m_waveHdr, sizeof(m_waveHdr));
	CHECK_RETURN(returnValue); 
	return true;
}


bool CoreAudioHelper::startPeakGetter()
{
	Q_ASSERT(m_currentDeviceIndex >= 0 && m_currentDeviceIndex < m_infos.size());
	auto& deviceInfo = m_infos[m_currentDeviceIndex];

	ZeroMemory(m_buffer, sizeof(m_buffer));
	m_waveHdr.dwFlags = 0;
	m_waveHdr.lpData = (LPSTR)m_buffer;
	m_waveHdr.dwBufferLength = sizeof(m_buffer);

	auto returnValue = waveInPrepareHeader(deviceInfo.handle, &m_waveHdr, sizeof(m_waveHdr));
	CHECK_RETURN(returnValue);

	returnValue = waveInAddBuffer(deviceInfo.handle, &m_waveHdr, sizeof(m_waveHdr));
	CHECK_RETURN(returnValue);

	returnValue = waveInStart(deviceInfo.handle);
	CHECK_RETURN(returnValue);

	deviceInfo.started = true;
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////

bool CoreAudioHelper::getMute(UINT index)
{
    return m_infos.at(index).muted;
}

//////////////////////////////////////////////////////////////////////////////////////////////

//	internal thread will check this field and not emit signals if muted
void CoreAudioHelper::setMute(UINT index, bool mute)
{
	Q_ASSERT(index >= 0 && index < m_infos.size());

	m_infos[index].muted = mute;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int CoreAudioHelper::getMasterVolume(UINT index)
{
	return m_infos.at(index).volumeFilterPercent * 100;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void CoreAudioHelper::setMasterVolume(UINT index, int volume)
{
	Q_ASSERT(index >= 0 && index < m_infos.size());
	Q_ASSERT(volume >= 0 && volume <= 100);

	m_infos[index].volumeFilterPercent = volume / 100.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////

//	non-qt thread have no qt event loop which causing signal/slot not working,
//	we use a queue to keep the value and a semaphore to notify the internal thread
//	to emit the signal.
void CoreAudioHelper::appendPeakValue(qint16 value)
{
	m_peakValueQueue.push(value);
	//	cannot call Win32 api inside a callback, so we notify the buffer waiter thread
	m_bufferFilled.release(1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

#undef CHECK_RETURN

/////////////////////////////////////////////////////////////////////////////////////////////////

void CoreAudioHelper::BufferWaiterThread::run()
{
	while (true) {
			m_helper->m_bufferFilled.acquire(1);

			m_helper->unprepareBuffer();

			if (m_helper->m_stopThread)		
				break;
			if (m_helper->m_emitUnplugged) {
				emit m_helper->currentDeviceUnplugged();
				m_helper->m_emitUnplugged = false;
				break;
			}						
			m_helper->emitPeakLevelAndContinue();
	}
}