#include "CppDsCapture.h"

#include <iostream>
#include <cassert>

#include <dsound.h>
#include <mmsystem.h>

#include <QDebug>

//////////////////////////////////////////////////////////////////////////////////////

#define SAFE_CALL(funcCall)                                                         \
    if (funcCall != DS_OK) {                                                        \
        throw DsException(__DATE__, __TIME__, __FILE__, __LINE__, GetLastError());  \
    }


#define SAFE_CALL_WITH_CHECK_CODE(funcCall, checkCode)                              \
    if (funcCall != checkCode) {                                                    \
        throw DsException(__DATE__, __TIME__, __FILE__, __LINE__, GetLastError());  \
    }

///////////////////////////////////////////////////////////////////////////////////////

CppDsCapture::CppDsCapture()
    : m_captureDeviceInterface(nullptr)
    , m_captureBufferInterface(nullptr)
    , m_filePath(nullptr)
{
    qDebug() <<"sdsadsad";

    SAFE_CALL(DirectSoundCaptureEnumerate(DSCaptureEnumCallback, this));

    qDebug() <<"sdsadsad";
}

BOOL CppDsCapture::DSCaptureEnumCallback(LPGUID guid,
                                         LPCWSTR description,
                                         LPCWSTR module,
                                         LPVOID context)
{
    Q_UNUSED(description);
    Q_UNUSED(module);

    qDebug() <<"sdsadsad1" << guid << description;

    if (guid != nullptr) {
        reinterpret_cast<CppDsCapture*>(context)->m_captureDeviceGuids.push_back(*guid);
        qDebug() <<"sdsadsad4545";

        reinterpret_cast<CppDsCapture*>(context)->m_avaiableDeviceNames.append(QString::fromStdWString(description));
    }
    qDebug() <<"sdsadsad2";

    return TRUE;
}

CppDsCapture::~CppDsCapture()
{
    if (m_captureBufferInterface)   m_captureBufferInterface->Release();
    if (m_captureDeviceInterface)   m_captureDeviceInterface->Release();
    if (m_filePath)                 delete m_filePath;
}

/////////////////////////////////////////////////////////////////////////////

void CppDsCapture::openDevice(unsigned deviceIndex)
{
    assert (deviceIndex < m_captureDeviceGuids.size());

    SAFE_CALL(DirectSoundCaptureCreate8(&m_captureDeviceGuids[deviceIndex], &m_captureDeviceInterface, NULL));
}

////////////////////////////////////////////////////////////////////////////

void CppDsCapture::closeCurrentDevice()
{
    assert (m_captureDeviceInterface != nullptr);

    m_captureDeviceInterface->Release();
    m_captureDeviceInterface = nullptr;
}

////////////////////////////////////////////////////////////////////////////

DSCCAPS CppDsCapture::currentDeviceCaps()
{
    assert (m_captureDeviceInterface != nullptr);

    DSCCAPS caps = {sizeof(caps)};
    SAFE_CALL(m_captureDeviceInterface->GetCaps(&caps));

    return caps;
}

////////////////////////////////////////////////////////////////////////////

void CppDsCapture::startCapture(EffectType aecType, bool enableNoiseFill,
                                EffectType nsType,  const QString& filePath)
{
    assert(m_captureDeviceInterface != nullptr);

    m_filePath = new char[filePath.size() + 1];
	//	cannot directly use QString.data() becaue it uses UTF8 by default
    memcpy(m_filePath, filePath.toStdString().c_str(), filePath.size() + 1);

    initFormatStruct();
    initEffectAndBufferStruct(aecType, nsType);
    createAudioBuffer();
    setCaptureEffectParameters(aecType, nsType, enableNoiseFill);

    m_notifyThread = std::thread(threadWaitingToWriteAudioToFile, this);
    SAFE_CALL(m_captureBufferInterface->Start(0));
}

void CppDsCapture::initFormatStruct()
{
	//	cannot use capabilities which capture devices do not support !
	auto caps = currentDeviceCaps();

	m_waveformat.cbSize = 0;
	m_waveformat.wFormatTag = WAVE_FORMAT_PCM;
	m_waveformat.nChannels = caps.dwChannels;
	m_waveformat.wBitsPerSample = 16; //  use 16 bits depth to use Noise Suppression
	m_waveformat.nBlockAlign = m_waveformat.wBitsPerSample * m_waveformat.nChannels / 8;
	m_waveformat.nSamplesPerSec = 44100;
	m_waveformat.nAvgBytesPerSec = m_waveformat.nSamplesPerSec * m_waveformat.nBlockAlign;
}

void CppDsCapture::initEffectAndBufferStruct(EffectType aecType, EffectType nsType)
{
	aecType = AcousticEchoCancellationSystem; nsType = NoiseSuppressionSystem;
	m_bufferDesc.dwSize = sizeof(m_bufferDesc);
	m_bufferDesc.dwReserved = 0;
	m_bufferDesc.dwFlags = DSCBCAPS_CTRLFX ;
	m_bufferDesc.lpwfxFormat = &m_waveformat;
	m_bufferDesc.dwBufferBytes = m_waveformat.nAvgBytesPerSec * 10;
	m_bufferDesc.dwFXCount = (aecType != None) + (nsType != None);
    m_bufferSize = m_bufferDesc.dwBufferBytes;

	(m_bufferDesc.dwFXCount > 0) ?
		m_bufferDesc.lpDSCFXDesc = reinterpret_cast<LPDSCEFFECTDESC>(&m_effectDesc) :
		m_bufferDesc.lpDSCFXDesc = nullptr;
    if (aecType != None) {
		m_effectDesc[0].dwSize = sizeof(m_effectDesc[0]);
		m_effectDesc[0].dwFlags = DSCFX_LOCSOFTWARE ;
		m_effectDesc[0].guidDSCFXClass =  captureEffectClassOfEffectType(aecType);
		m_effectDesc[0].guidDSCFXInstance = captureEffectInstanceOfEffectType(aecType);
		m_effectDesc[0].dwReserved1 = 0;
		m_effectDesc[0].dwReserved2 = 0;
    }
    if (nsType != None) {
		m_effectDesc[1].dwSize = sizeof(m_effectDesc[1]);
		m_effectDesc[1].dwFlags = DSCFX_LOCSOFTWARE ;
		m_effectDesc[1].guidDSCFXClass = captureEffectClassOfEffectType(nsType);
		m_effectDesc[1].guidDSCFXInstance = captureEffectInstanceOfEffectType(nsType);
		m_effectDesc[1].dwReserved1 = 0;
		m_effectDesc[1].dwReserved2 = 0;
    }
}

void CppDsCapture::createAudioBuffer()
{
	IDirectSoundCaptureBuffer* bufferInterface;
    SAFE_CALL(m_captureDeviceInterface->CreateCaptureBuffer(&m_bufferDesc, &bufferInterface, nullptr));
    //  possibly, this effect is not avaiable at the software location
    SAFE_CALL_WITH_CHECK_CODE(bufferInterface->QueryInterface(IID_IDirectSoundCaptureBuffer8, reinterpret_cast<LPVOID*>(&m_captureBufferInterface)),
                              S_OK);
    bufferInterface->Release();
}

void CppDsCapture::setCaptureEffectParameters(EffectType aecType, EffectType nsType, bool enableNoiseFill)
{
    if (aecType != None) {
        IDirectSoundCaptureFXAec8* aecInterface = 0;
        SAFE_CALL(m_captureBufferInterface->GetObjectInPath(captureEffectClassOfEffectType(aecType),
                                                           0,
                                                           captureEffectInterfaceOfEffectType(aecType),
                                                           reinterpret_cast<LPVOID*>(&aecInterface)));

        DSCFXAec aecStruct;
        aecStruct.fEnable = true;
        aecStruct.fNoiseFill = enableNoiseFill;
        SAFE_CALL(aecInterface->SetAllParameters(&aecStruct));
    }
    if (nsType != None) {
        IDirectSoundCaptureFXNoiseSuppress8 * nsInterface;
        SAFE_CALL(m_captureBufferInterface->GetObjectInPath(captureEffectClassOfEffectType(nsType),
                                                           1,
                                                           captureEffectInterfaceOfEffectType(nsType),
                                                           reinterpret_cast<LPVOID*>(&nsInterface)));

        DSCFXNoiseSuppress nsStruct = { true };
        SAFE_CALL(nsInterface->SetAllParameters(&nsStruct));
    }
}

void CppDsCapture::threadWaitingToWriteAudioToFile(CppDsCapture* captureClassPtr)
{
    IDirectSoundNotify8* notifyInterface;
    SAFE_CALL_WITH_CHECK_CODE(captureClassPtr->m_captureBufferInterface->QueryInterface(IID_IDirectSoundNotify8,
                                                                                        reinterpret_cast<LPVOID*>(&notifyInterface)),
                              S_OK);

    HANDLE notifyEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (notifyEventHandle == NULL) {
        throw DsException(__DATE__, __TIME__, __FILE__, __LINE__, GetLastError());
    }

    DSBPOSITIONNOTIFY notifyStruct;
    notifyStruct.dwOffset = DSBPN_OFFSETSTOP;
    notifyStruct.hEventNotify = notifyEventHandle;
    SAFE_CALL(notifyInterface->SetNotificationPositions(1, &notifyStruct));

    auto returnValue = WaitForSingleObject(notifyEventHandle, INFINITE);
    assert (returnValue == WAIT_OBJECT_0);
	writeWavFile(captureClassPtr);

    notifyInterface->Release();
}

void CppDsCapture::writeWavFile(CppDsCapture* captureClassPtr)
{
    HMMIO mmioHandle = mmioOpenA(captureClassPtr->m_filePath, NULL, MMIO_CREATE |MMIO_WRITE);
    if (mmioHandle == NULL) {
        throw DsException(__DATE__, __TIME__, __FILE__, __LINE__, GetLastError());
    }

    //  create the whole RIFF chunk
    MMCKINFO riffChunkInfo;
	riffChunkInfo.ckid = mmioFOURCC('R', 'I', 'F', 'F');
	riffChunkInfo.cksize = 4 +
						   4 + 4 + sizeof(WAVEFORMATEX) +
						   4 + 4 + captureClassPtr->m_bufferSize;;
    riffChunkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    SAFE_CALL_WITH_CHECK_CODE(mmioCreateChunk(mmioHandle, &riffChunkInfo, MMIO_CREATERIFF), MMSYSERR_NOERROR);

    //  write the fmt chunk, after writing the file position is changed
    MMCKINFO fmtChunk;
	fmtChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	fmtChunk.cksize = sizeof(WAVEFORMATEX);;
	SAFE_CALL_WITH_CHECK_CODE(mmioCreateChunk(mmioHandle, &fmtChunk, 0), MMSYSERR_NOERROR);

    if (mmioWrite(mmioHandle, reinterpret_cast<char*>(&captureClassPtr->m_waveformat), sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX)) {
        throw DsException(__DATE__, __TIME__, __FILE__, __LINE__, GetLastError());
    }
	SAFE_CALL_WITH_CHECK_CODE(mmioAscend(mmioHandle, &fmtChunk, 0), MMSYSERR_NOERROR);

    //  create the data chunk
	MMCKINFO dataChunkInfo;
	dataChunkInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	dataChunkInfo.cksize = captureClassPtr->m_bufferSize;;
	SAFE_CALL_WITH_CHECK_CODE(mmioCreateChunk(mmioHandle, &dataChunkInfo, 0), MMSYSERR_NOERROR);

    LPVOID bufferPtr;
    DWORD  writtenSize;
    SAFE_CALL(captureClassPtr->m_captureBufferInterface->Lock(0, captureClassPtr->m_bufferSize,
                                                              &bufferPtr, &writtenSize,
                                                              NULL, NULL,
                                                              DSCBLOCK_ENTIREBUFFER));
    assert (writtenSize == captureClassPtr->m_bufferSize);

    if (mmioWrite(mmioHandle, reinterpret_cast<char*>(bufferPtr), writtenSize) != writtenSize) {
        throw DsException(__DATE__, __TIME__, __FILE__, __LINE__, GetLastError());
    }
	SAFE_CALL_WITH_CHECK_CODE(mmioAscend(mmioHandle, &dataChunkInfo, 0), MMSYSERR_NOERROR);

    SAFE_CALL(captureClassPtr->m_captureBufferInterface->Unlock(bufferPtr, writtenSize, NULL, NULL));

    //  close mmio handle
    SAFE_CALL_WITH_CHECK_CODE(mmioAscend(mmioHandle, &riffChunkInfo, 0), MMSYSERR_NOERROR);
    SAFE_CALL_WITH_CHECK_CODE(mmioClose(mmioHandle, 0), 0);
}

GUID CppDsCapture::captureEffectClassOfEffectType(CppDsCapture::EffectType effectType)
{
    switch (effectType) {
    case AcousticEchoCancellationMicrosoft: return GUID_DSCFX_CLASS_AEC;
    case AcousticEchoCancellationSystem:    return GUID_DSCFX_CLASS_AEC;
    case NoiseSuppressionMicrosoft:         return GUID_DSCFX_CLASS_NS;
    case NoiseSuppressionSystem:            return GUID_DSCFX_CLASS_NS;
    case None:                              throw std::exception("Capture Effect None Used, code logic error!");

    default:                           throw std::exception("Logic error");
    }
}

GUID CppDsCapture::captureEffectInstanceOfEffectType(CppDsCapture::EffectType effectType)
{
    switch (effectType) {
    case AcousticEchoCancellationMicrosoft: return GUID_DSCFX_MS_AEC;
    case AcousticEchoCancellationSystem:    return GUID_DSCFX_SYSTEM_AEC;
    case NoiseSuppressionMicrosoft:         return GUID_DSCFX_MS_NS;
    case NoiseSuppressionSystem:            return GUID_DSCFX_SYSTEM_NS;
    case None:                              throw std::exception("Capture Effect None used, code logic error!");

    default:                                throw std::exception("Logic error!");
    }
}

GUID CppDsCapture::captureEffectInterfaceOfEffectType(EffectType effectType)
{
    switch (effectType) {
    case AcousticEchoCancellationMicrosoft:
    case AcousticEchoCancellationSystem:    return IID_IDirectSoundCaptureFXAec8;
    case NoiseSuppressionMicrosoft:
    case NoiseSuppressionSystem:            return IID_IDirectSoundCaptureFXNoiseSuppress8;
    case None:                              throw std::exception("Capture Effect None used, code logic error!");

    default:                                throw std::exception("Logic error!");
    }
}

////////////////////////////////////////////////////////////////////////////
