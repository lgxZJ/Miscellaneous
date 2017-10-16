#ifndef COREAUDIOHELPER_XP_H
#define COREAUDIOHELPER_XP_H

#include <QObject>
#include <QWidget>
#include <DeviceInfo.h>
#include <Windows.h>

class QSlider;
class QCheckBox;

class CoreAudioHelper_xp : public QObject
{
    Q_OBJECT
public:
    explicit CoreAudioHelper_xp(QObject* parent = nullptr);
    ~CoreAudioHelper_xp();

public slots:
    int init()			   { return 0; }
    void setControl(QSlider *slider, QCheckBox *checkBox);
    QList<AudioDeviceInfo> getAudioDevices();

    bool getMute(UINT deviceIndex);
	void setMute(UINT index, bool mute);

    int getMasterVolume(UINT deviceIndex);
    void setMasterVolume(UINT index, int volume);

signals:
//    void deviceNumChanged(const QString &deviceId, int type);
    void error(const QString &msg);

private:
    bool getDeviceCaps(int deviceIndex);
    bool getDestinationLines(int deviceIndex);
    bool getDstLineControls(int deviceIndex);
    bool getDstLineControl(int deviceIndex, int lineIndex, DWORD controlType);

	bool setOrGetMute(bool ifSet, bool muteFlag, int deviceIndex, DWORD controlId, bool* getResult);
	bool setOrGetVolume(bool ifSet, DWORD volume, int deviceIndex, DWORD controlId, DWORD* getResult);
	bool destinationLineIsOne(UINT deviceIndex);
	bool isMuteControlValid(UINT deviceIndex, DWORD lineId);
	bool isVolumeControlValid(UINT deviceIndex, DWORD lineId);
	void updateControlDisplay(UINT deviecIndex);

private:
    typedef    DWORD    LineId;
    struct InnerInfo
    {
        struct MuteAndVolumeControl
        {
            bool            isVolumeValid;
			bool			isMuteValid;

            MIXERCONTROL    volumeControl;
            MIXERCONTROL    muteControl;
        };

        MIXERCAPS               deviceCaps;
        //  stored in destination line order got in MIXERCAPS, in most situations
        //  there is only ONE destination line.
        std::vector<MIXERLINE>  destinationLines;
        std::map<LineId, MuteAndVolumeControl> lineControls;
    };

	class MsgReceiver : public QWidget
	{
	public:
		explicit MsgReceiver(CoreAudioHelper_xp* helper) : QWidget(), m_helper(helper)	
		{}

		virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result);

	private:
		CoreAudioHelper_xp*	m_helper;
	};

    std::vector<HMIXER>     m_inputDeviceHandles;
    std::vector<InnerInfo>  m_mixerInfos;

	QSlider*		m_slider;
	QCheckBox*		m_checkBox;
	MsgReceiver*	m_msgReceiver;
};

#endif // COREAUDIOHELPER_XP_H
