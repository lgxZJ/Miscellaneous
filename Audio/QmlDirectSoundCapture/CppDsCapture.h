#ifndef CPPDSCAPTURE_H
#define CPPDSCAPTURE_H

#include <QObject>
#include <QVariant>

#include <tchar.h>
#include <initguid.h>
#include <Windows.h>
#include <dsound.h>

#include <vector>
#include <thread>
#include <memory>
#include <array>

////////////////////////////////////////////////////////////////

class DsException : public std::exception
{
public:
    DsException(const char* date,
                const char* time,
                const char* file,
                int line,
                DWORD errorCode)
        : m_what(std::string(date) + time + file + std::to_string(line) + std::to_string(errorCode))
    {
    }

    const char* what() const override
    {
        return m_what.c_str();
    }

private:
    const std::string m_what;
};

/////////////////////////////////////////////////////////////////

//  NOTE: This class only supports opening one device at the same time.
class CppDsCapture : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList avaiableDeviceNames READ avaiableDeviceNames)

public:
    enum EffectType {
        AcousticEchoCancellationMicrosoft,
        AcousticEchoCancellationSystem,
        NoiseSuppressionMicrosoft,
        NoiseSuppressionSystem,
        None
    };
    Q_ENUM(EffectType)

    CppDsCapture();
    ~CppDsCapture();

    unsigned getDeviceCount() const  { return static_cast<unsigned>(m_captureDeviceGuids.size()); }
    DSCCAPS  currentDeviceCaps();

    //////////////////////////////////////////////////////////////
    //  QML functions
    Q_INVOKABLE void    openDevice(unsigned deviceIndex);
    Q_INVOKABLE void    closeCurrentDevice();
    Q_INVOKABLE void    startCapture(EffectType aecType, bool enableNoiseFill,
                                      EffectType nsType,  const QString& filePath);
    QVariantList avaiableDeviceNames() const { return m_avaiableDeviceNames; }

private:
    static BOOL CALLBACK DSCaptureEnumCallback(LPGUID guid,
                                               LPCWSTR description,
                                               LPCWSTR module,
                                               LPVOID context);
    static void threadWaitingToWriteAudioToFile(CppDsCapture* captureClassPtr);
    static void writeWavFile(CppDsCapture* captureClassPtr);

    GUID captureEffectClassOfEffectType(EffectType efectType);
    GUID captureEffectInstanceOfEffectType(EffectType effectType);
    GUID captureEffectInterfaceOfEffectType(EffectType effectType);
    void setCaptureEffectParameters(EffectType aecType, EffectType nsType, bool enableNoiseFill);
    void initFormatStruct();
    void initEffectAndBufferStruct(EffectType aecType,
                                   EffectType nsType);
    void createAudioBuffer();


    std::vector<GUID>           m_captureDeviceGuids;
    IDirectSoundCapture8*       m_captureDeviceInterface;
    IDirectSoundCaptureBuffer8* m_captureBufferInterface;
    char*                       m_filePath; //  mmioOpen use non-const filename

    DWORD                       m_bufferSize;
    WAVEFORMATEX                m_waveformat;
	DSCEFFECTDESC				m_effectDesc[2];
	DSCBUFFERDESC				m_bufferDesc;

    std::thread                 m_notifyThread;

    ////////////////////////////////////////////////////////
    //  QML propertyies
    QVariantList    m_avaiableDeviceNames;
};

////////////////////////////////////////////////////////////////

#endif // CPPDSCAPTURE_H
