#include <iostream>
#include <locale>
#include "CoreAudioHelper.h"

#include <qcoreapplication.h>
#include <qobject.h>

#include "PeakReceiver.h"

////////////////////////////////////////////////////////////////////

class HelperGetter : public CoreAudioHelper
{
public:
	const QList<AudioDeviceInfo>& getInfos() { return CoreAudioHelper::getInfos(); }
};

//////////////////////////////////////////////////////////////////////////////////////////

void test_Instance_ReturnSame()
{
	Q_ASSERT(CoreAudioHelper::instance() == CoreAudioHelper::instance());
	Q_ASSERT(CoreAudioHelper::instance() == CoreAudioHelper::instance());
}

void test_Instance_StatusShouldBeInitial()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();

	for (int i = 0; i < deviceInfos.size(); ++i) {
		auto& deviceInfos = ((HelperGetter*)HelperGetter::instance())->getInfos();
		Q_ASSERT(deviceInfos.at(i).handle == NULL);
		Q_ASSERT(deviceInfos.at(i).started == false);
		Q_ASSERT(deviceInfos.at(i).manuallyClosed == false);
		Q_ASSERT(deviceInfos.at(i).muted == false);
		Q_ASSERT(deviceInfos.at(i).volumeFilterPercent == 0.5);
	}

	//	teardown
	for (int i = 0; i < deviceInfos.size(); ++i) {
		HelperGetter::instance()->stopInputDevice(i);
	}
}

////////////////

void test_startInputDevice_ShouldBeStartedStatus()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();
	for (int i = 0; i < deviceInfos.size(); ++i) {
		HelperGetter::instance()->startInputDevice(i);

		auto& deviceInfos = ((HelperGetter*)HelperGetter::instance())->getInfos();
		Q_ASSERT(deviceInfos.at(i).handle != NULL);
		Q_ASSERT(deviceInfos.at(i).started == true);
	}

	//	teardown
	for (int i = 0; i < deviceInfos.size(); ++i) {
		HelperGetter::instance()->stopInputDevice(i);
	}
}

void test_stopInputDevice_StatusShouldBeInitial()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();
	for (int i = 0; i < deviceInfos.size(); ++i) {
		HelperGetter::instance()->stopInputDevice(i);

		auto& deviceInfos = ((HelperGetter*)HelperGetter::instance())->getInfos();
		Q_ASSERT(deviceInfos.at(i).handle == NULL);
		Q_ASSERT(deviceInfos.at(i).started == false);
		Q_ASSERT(deviceInfos.at(i).manuallyClosed == false);
		Q_ASSERT(deviceInfos.at(i).muted == false);
		Q_ASSERT(deviceInfos.at(i).volumeFilterPercent == 0.5);
	}

	//	teardown
	for (int i = 0; i < deviceInfos.size(); ++i) {
		HelperGetter::instance()->stopInputDevice(i);
	}
}

////////////////

void test_startStopStart_ShouldBeStartedStatus()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();
	for (int i = 0; i < deviceInfos.size(); ++i) {
		HelperGetter::instance()->startInputDevice(i);
		HelperGetter::instance()->stopInputDevice(i);
		HelperGetter::instance()->startInputDevice(i);

		auto& deviceInfos = ((HelperGetter*)HelperGetter::instance())->getInfos();
		Q_ASSERT(deviceInfos.at(i).handle != NULL);
		Q_ASSERT(deviceInfos.at(i).started == true);
	}

	//	teardown
	for (int i = 0; i < deviceInfos.size(); ++i) {
		HelperGetter::instance()->stopInputDevice(i);
	}
}

//////////

void test_getMute_IntialReturnFalse()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();

	for (int i = 0; i < deviceInfos.size(); ++i) {
		Q_ASSERT(CoreAudioHelper::instance()->getMute(i) == false);
	}
}

void test_getMute_GetWhatSet()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();

	for (int i = 0; i < deviceInfos.size(); ++i) {
		CoreAudioHelper::instance()->setMute(i, true);
	}

	for (int i = 0; i < deviceInfos.size(); ++i) {
		Q_ASSERT(CoreAudioHelper::instance()->getMute(i) == true);
	}
}

/////////////

void test_getMasterVolume_InitialReturn50()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();

	for (int i = 0; i < deviceInfos.size(); ++i) {
		Q_ASSERT(CoreAudioHelper::instance()->getMasterVolume(i) == 50);
	}
}

void test_getMasterVolume_ReturnWhatSet()
{
	auto& deviceInfos = HelperGetter::instance()->getInputDevices();

	for (int i = 0; i < deviceInfos.size(); ++i) {
		CoreAudioHelper::instance()->setMasterVolume(i, 15);
	}

	for (int i = 0; i < deviceInfos.size(); ++i) {
		Q_ASSERT(CoreAudioHelper::instance()->getMasterVolume(i) == 15);
	}
}

////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
    std::wcout.imbue(std::locale(""));

    auto helper = CoreAudioHelper::instance();

	std::cout << "list all input audio devices:\n";
    auto result = helper->getInputDevices();
    for (auto oneResult :result)
        std::wcout << L"\t" << oneResult.name.toStdWString();
	std::cout << "\n\n";

	if (!result.empty()) {
		std::cout << "=======tests begins=======\n";
		test_Instance_ReturnSame();
		test_Instance_StatusShouldBeInitial();
		test_startInputDevice_ShouldBeStartedStatus();
		test_stopInputDevice_StatusShouldBeInitial();
		test_startStopStart_ShouldBeStartedStatus();
		test_getMute_IntialReturnFalse();
		test_getMute_GetWhatSet();
		test_getMasterVolume_InitialReturn50();
		std::cout << "=======tests ends=======\n";
	}

	
	PeakReceiver receiver;
	if (!result.empty()) {
		
		QObject::connect(helper, SIGNAL(peakChanged(qint16)), &receiver, SLOT(peakShow(qint16)));
		QObject::connect(helper, SIGNAL(currentDeviceUnplugged()), &receiver, SLOT(deviceUnplugged()));

		helper->startInputDevice(0);
		helper->setMasterVolume(0, 100);
		std::cout << "\nplease unplug the current device\n\n";
	}

    return app.exec();
}
