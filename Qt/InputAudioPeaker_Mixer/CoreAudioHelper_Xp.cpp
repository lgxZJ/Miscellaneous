#include "CoreAudioHelper_Xp.h"

#include <QCheckBox>
#include <QSlider>

#define     CHECK_RETURN(value, CallName)   \
    if (value != MMSYSERR_NOERROR) {		\
		emit error(CallName);				\
        return false;						\
    }

#define		CHECK(value, CallName)			\
	if (value != MMSYSERR_NOERROR) {		\
		emit error(CallName);				\
		return;								\
	}

/////////////////////////////////////////////////////////////////////////////

CoreAudioHelper_xp::CoreAudioHelper_xp(QObject* parent)
    : QObject(parent)
	, m_msgReceiver(new CoreAudioHelper_xp::MsgReceiver(this))
	, m_checkBox(nullptr)
	, m_slider(nullptr)
{}

CoreAudioHelper_xp::~CoreAudioHelper_xp()
{
    for (auto inputDeviceHandle : m_inputDeviceHandles) {
        if (inputDeviceHandle != NULL)  mixerClose(inputDeviceHandle);
    }
}

/////////////////////////////////////////////////////////////////////////////

void CoreAudioHelper_xp::setControl(QSlider *slider, QCheckBox *checkBox)
{
	m_slider = slider;
	m_checkBox = checkBox;
}

/////////////////////////////////////////////////////////////////////////////

QList<AudioDeviceInfo> CoreAudioHelper_xp::getAudioDevices()
{
    QList<AudioDeviceInfo> result;

    //  reserve member storage
    auto deviceNum = waveInGetNumDevs();
    m_mixerInfos.resize(deviceNum);
    m_inputDeviceHandles.resize(deviceNum, NULL);

    for (int deviceIndex = 0; deviceIndex < deviceNum; ++deviceIndex) {
        if (getDeviceCaps(deviceIndex) == false)        return {};
        if (getDestinationLines(deviceIndex) == false)  return {};
        if (getDstLineControls(deviceIndex) == false)   return {};

        AudioDeviceInfo info;
        info.id = deviceIndex;
        info.name = QString::fromStdWString(std::wstring(
                        m_mixerInfos[deviceIndex].deviceCaps.szPname));
        result.push_back(info);
    }

    return result;
}

bool CoreAudioHelper_xp::getDeviceCaps(int deviceIndex)
{

	auto result = mixerOpen(&m_inputDeviceHandles[deviceIndex], deviceIndex, 
							m_msgReceiver->winId(), 0, 
							MIXER_OBJECTF_WAVEIN | CALLBACK_WINDOW);
	CHECK_RETURN(result, "mixerOpen");

	result = mixerGetDevCaps((UINT_PTR)m_inputDeviceHandles[deviceIndex],
							 &m_mixerInfos[deviceIndex].deviceCaps,
							 sizeof(m_mixerInfos[deviceIndex].deviceCaps));
    CHECK_RETURN(result, "mixerGetDevCaps");

    return true;
}

bool CoreAudioHelper_xp::getDestinationLines(int deviceIndex)
{
    //  reserve member storage
    int destinationCount = m_mixerInfos[deviceIndex].deviceCaps.cDestinations;
    m_mixerInfos[deviceIndex].destinationLines.resize(destinationCount);

    for (int destinationIndex = 0; destinationIndex < destinationCount; ++destinationIndex) {

        MIXERLINE* destinationLine = &m_mixerInfos[deviceIndex]
                                        .destinationLines[destinationIndex];
        destinationLine->cbStruct = sizeof(*destinationLine);
        destinationLine->dwDestination = destinationIndex;

		auto result = mixerGetLineInfo((HMIXEROBJ)deviceIndex, destinationLine,
										MIXER_GETLINEINFOF_DESTINATION | MIXER_OBJECTF_WAVEIN);
        CHECK_RETURN(result, "mixerGetLineInfo");
    }

    return true;
}

bool CoreAudioHelper_xp::getDstLineControls(int deviceIndex)
{
    for (int dstLineIndex = 0;
         dstLineIndex < m_mixerInfos[deviceIndex].deviceCaps.cDestinations;
         ++dstLineIndex) {

        if (getDstLineControl(deviceIndex, dstLineIndex, MIXERCONTROL_CONTROLTYPE_VOLUME) == false)
            return false;
        if (getDstLineControl(deviceIndex, dstLineIndex, MIXERCONTROL_CONTROLTYPE_MUTE) == false)
            return false;
    }
    return true;
}

bool CoreAudioHelper_xp::getDstLineControl(int deviceIndex, int lineIndex, DWORD controlType)
{
    DWORD lineId = m_mixerInfos[deviceIndex].destinationLines[lineIndex].dwLineID;;

    MIXERLINECONTROLS mixerLineControls;
    mixerLineControls.cbStruct = sizeof(mixerLineControls);
    mixerLineControls.dwLineID = lineId;
    //  todo:maybe error
    mixerLineControls.cControls = 1;
    mixerLineControls.dwControlType = controlType;
    mixerLineControls.cbmxctrl = sizeof(MIXERCONTROL);

    //  store to the appropriate control
    if (controlType == MIXERCONTROL_CONTROLTYPE_VOLUME) {
        m_mixerInfos[deviceIndex].lineControls[lineId].isVolumeValid = false;
        mixerLineControls.pamxctrl = &m_mixerInfos[deviceIndex].lineControls[lineId].volumeControl;
    } else if (controlType == MIXERCONTROL_CONTROLTYPE_MUTE) {
        m_mixerInfos[deviceIndex].lineControls[lineId].isMuteValid = false;
        mixerLineControls.pamxctrl = &m_mixerInfos[deviceIndex].lineControls[lineId].muteControl;
    }

	auto result = mixerGetLineControls((HMIXEROBJ)deviceIndex, &mixerLineControls,
										MIXER_GETLINECONTROLSF_ONEBYTYPE | MIXER_OBJECTF_WAVEIN);
    CHECK_RETURN(result, "mixerGetLineControls");

    if (controlType == MIXERCONTROL_CONTROLTYPE_VOLUME) {
		m_mixerInfos[deviceIndex].lineControls[lineId].isVolumeValid = true;
    } else if (controlType == MIXERCONTROL_CONTROLTYPE_MUTE) {
		m_mixerInfos[deviceIndex].lineControls[lineId].isMuteValid = true;
    }
    return true;
}

/////////////////////////////////////////////////////////////////////////////

bool CoreAudioHelper_xp::getMute(UINT deviceIndex)
{
    Q_ASSERT(deviceIndex >= 0 && deviceIndex < m_mixerInfos.size());

	if (!destinationLineIsOne(deviceIndex))
		return false;

    DWORD lineId = m_mixerInfos[deviceIndex].destinationLines[0].dwLineID;
	if (!isMuteControlValid(deviceIndex, lineId))
		return false;

    DWORD controlId = m_mixerInfos[deviceIndex].lineControls[lineId].muteControl.dwControlID;
	bool result = false;
	setOrGetMute(false, false, deviceIndex, controlId, &result);

	return result;
}

bool CoreAudioHelper_xp::destinationLineIsOne(UINT deviceIndex)
{
	//  almost every mixer has only ONE destination line and
	//  we don't known what the return value should be when
	//  where is more than one line.
	if (m_mixerInfos[deviceIndex].destinationLines.size() != 1) {
		emit error("no destination lines");
		return false;
	}

	return true;
}

bool CoreAudioHelper_xp::isMuteControlValid(UINT deviceIndex, DWORD lineId)
{
	if (!m_mixerInfos[deviceIndex].lineControls[lineId].isMuteValid) {
		emit error("no valid mute control");
		return false;
	}

	return true;
}

bool CoreAudioHelper_xp::setOrGetMute(
	bool ifSet, 
	bool muteFlag, int deviceIndex, DWORD controlId,
	bool* getResult)
{
	MIXERCONTROLDETAILS_BOOLEAN value;
	if (ifSet)	value.fValue = muteFlag;

	MIXERCONTROLDETAILS details;
	details.cbStruct = sizeof(details);
	details.dwControlID = controlId;
	details.cChannels = 1;
	details.cMultipleItems = 0;
	details.cbDetails = sizeof(value);
	details.paDetails = &value;

	MMRESULT result;
	if (ifSet)	result = mixerSetControlDetails((HMIXEROBJ)deviceIndex, &details,
												MIXER_OBJECTF_WAVEIN | MIXER_GETCONTROLDETAILSF_VALUE);
	else		result = mixerGetControlDetails((HMIXEROBJ)deviceIndex, &details,
												MIXER_OBJECTF_WAVEIN | MIXER_GETCONTROLDETAILSF_VALUE);
	CHECK_RETURN(result, "mixerGetControlDetails");

	if (!ifSet)	*getResult = value.fValue;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////

void CoreAudioHelper_xp::setMute(UINT deviceIndex, bool mute)
{
	Q_ASSERT(deviceIndex >= 0 && deviceIndex < m_mixerInfos.size());

	if (!destinationLineIsOne(deviceIndex))
		return;

	DWORD lineId = m_mixerInfos[deviceIndex].destinationLines[0].dwLineID;
	if (!isMuteControlValid(deviceIndex, lineId))
		return;

	DWORD controlId = m_mixerInfos[deviceIndex].lineControls[lineId].muteControl.dwControlID;
	setOrGetMute(true, mute, deviceIndex, controlId, nullptr);
}


//////////////////////////////////////////////////////////////////////////////////////////////

int CoreAudioHelper_xp::getMasterVolume(UINT deviceIndex)
{
	Q_ASSERT(deviceIndex >= 0 && deviceIndex < m_mixerInfos.size());

	if (!destinationLineIsOne(deviceIndex))
		return false;

	DWORD lineId = m_mixerInfos[deviceIndex].destinationLines[0].dwLineID;
	if (!isVolumeControlValid(deviceIndex, lineId))
		return false;

	DWORD controlId = m_mixerInfos[deviceIndex].lineControls[lineId].volumeControl.dwControlID;
	DWORD result = 0;
	setOrGetVolume(false, 0, deviceIndex, controlId, &result);

	//	volume range is 0~100, but the inner represent value range fis 0~65535, convert here
	return result / (65535 / 100.0);
}

bool CoreAudioHelper_xp::isVolumeControlValid(UINT deviceIndex, DWORD lineId)
{
	if (!m_mixerInfos[deviceIndex].lineControls[lineId].isVolumeValid) {
		emit error("no valid volume control");
		return false;
	}

	return true;
}

bool CoreAudioHelper_xp::setOrGetVolume(
	bool ifSet, 
	DWORD volume, int deviceIndex, DWORD controlId,
	DWORD* getResult)
{
	MIXERCONTROLDETAILS_UNSIGNED value;
	if (ifSet)	value.dwValue = volume;

	MIXERCONTROLDETAILS details;
	details.cbStruct = sizeof(details);
	details.dwControlID = controlId;
	details.cChannels = 1;
	details.cMultipleItems = 0;
	details.cbDetails = sizeof(value);
	details.paDetails = &value;

	MMRESULT result;
	if (ifSet)	result = mixerSetControlDetails((HMIXEROBJ)deviceIndex, &details,
												MIXER_OBJECTF_WAVEIN | MIXER_GETCONTROLDETAILSF_VALUE);
	else		result = mixerGetControlDetails((HMIXEROBJ)deviceIndex, &details,
												MIXER_OBJECTF_WAVEIN | MIXER_GETCONTROLDETAILSF_VALUE);
	CHECK_RETURN(result, "mixerGetControlDetails");

	if (!ifSet)	*getResult = value.dwValue;
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
void CoreAudioHelper_xp::setMasterVolume(UINT deviceIndex, int volume)
{
	Q_ASSERT(deviceIndex >= 0 && deviceIndex < m_mixerInfos.size());

	if (!destinationLineIsOne(deviceIndex))
		return;

	DWORD lineId = m_mixerInfos[deviceIndex].destinationLines[0].dwLineID;
	if (!isVolumeControlValid(deviceIndex, lineId))
		return;

	DWORD controlId = m_mixerInfos[deviceIndex].lineControls[lineId].volumeControl.dwControlID;
	DWORD result = 0;
	//	volume range is 0~100, but the inner represent value range fis 0~65535, convert here
	setOrGetVolume(true, volume * (65535 / 100.0), deviceIndex, controlId, nullptr);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CoreAudioHelper_xp::updateControlDisplay(UINT deviceIndex)
{
	if (m_checkBox)
		m_checkBox->setChecked(getMute(deviceIndex));
	if (m_slider)
		m_slider->setValue(getMasterVolume(deviceIndex));
}

/////////////////////////////////////////////////////////////////////////////////////////////////
#include <QDebug>
bool CoreAudioHelper_xp::MsgReceiver::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
	if (QString(eventType) == "windows_generic_MSG") {
		MSG* msg = reinterpret_cast<MSG*>(message);

		if (msg->message == MM_MIXM_CONTROL_CHANGE) {
			for (int deviceIndex = 0; deviceIndex < m_helper->m_mixerInfos.size(); ++deviceIndex) {
				if (m_helper->m_inputDeviceHandles[deviceIndex] == reinterpret_cast<HMIXER>(msg->wParam)) {
					m_helper->updateControlDisplay(deviceIndex);
				}
			}
		}
		if (msg->message == MM_MIXM_LINE_CHANGE) {
		}
	}
	return false;

}
