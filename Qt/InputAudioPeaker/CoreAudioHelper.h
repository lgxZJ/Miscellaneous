#ifndef COREAUDIOHELPER_H
#define COREAUDIOHELPER_H

#include <Windows.h>
#include <QObject>
#include <qsemaphore.h>
#include <qtimer.h>
#include <QList>
#include <qthread.h>

#include <queue>

class CoreAudioHelper;
class QSlider;
class QCheckBox;

struct AudioDeviceInfo
{
    AudioDeviceInfo()
    {
        id = -1;
        name = "";
        handle = NULL;
		muted = false;
        started = false;
		manuallyClosed = false;
		volumeFilterPercent = 0.50;
    }

    bool operator==(const AudioDeviceInfo &other) const
    {
        return other.id == id && other.name == name;
    }


    int     id;
    QString name;
    HWAVEIN handle;
    bool    started;
	bool	muted;
	bool	manuallyClosed;
    DWORD   waveFormat;
	double	volumeFilterPercent;
};

//	NOTE:	can only start one device at a time
class CoreAudioHelper : public QObject
{
    Q_OBJECT
public:
	static CoreAudioHelper* instance();

public slots:

    QList<AudioDeviceInfo> getInputDevices();

    bool startInputDevice(UINT index);
    bool stopInputDevice(UINT index);

    bool getMute(UINT index);
    void setMute(UINT index, bool mute);

	//	volume range 0-100
	int getMasterVolume(UINT index);
	void setMasterVolume(UINT index, int volume);

signals:
    void error(const QString &msg);
	void currentDeviceUnplugged();
	//	peak value range from 0 to 100
    void peakChanged(qint16 value);

protected:
	const QList<AudioDeviceInfo>& getInfos() { return m_infos; }

private:
	//	a single slot buffer thread
	class BufferWaiterThread : public QThread
	{
	public:
		BufferWaiterThread(CoreAudioHelper* helper)
			: m_helper(helper)
		{}

		virtual void run();

	private:
		CoreAudioHelper*	m_helper;
	};

	static void CALLBACK waveInProc(
					HWAVEIN   hwi,
					UINT      uMsg,
					DWORD_PTR dwInstance,
					DWORD_PTR dwParam1,
					DWORD_PTR dwParam2);

	CoreAudioHelper();
	~CoreAudioHelper();

    void appendPeakValue(qint16 value);
	bool startPeakGetter();
	bool unprepareBuffer();
    WAVEFORMATEX chooseAppropriateFormat();
	void emitPeakLevelAndContinue();
	void clearInfo(UINT index);
	void resetHelperMembers();

private:
    QList<AudioDeviceInfo>  m_infos;
	WAVEHDR					m_waveHdr;
    char					m_buffer[16];
	int						m_currentDeviceIndex;

    bool                m_is8BitsSample;
	std::queue<qint16>	m_peakValueQueue;

	bool				m_stopThread;
	bool				m_emitUnplugged;
	BufferWaiterThread*	m_bufferWaiterThread;
	QSemaphore			m_bufferFilled;
};



#endif // COREAUDIOHELPER_H
