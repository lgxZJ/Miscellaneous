#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMetaObject::connectSlotsByName(this);

    for (QObject* childObject : this->centralWidget()->children()) {
        QWidget* childWidget = dynamic_cast<QWidget*>(childObject);
        if (childWidget != nullptr)    //  succeed
            childWidget->setDisabled(true);
    }
	ui->deviceComboBox->setEnabled(true);


    for (unsigned i = 0; i < devices_.deviceCount(); ++i) {
        ui->deviceComboBox->addItem(QString::fromStdWString(devices_.deviceDescription(i)));
    }
    if (devices_.deviceCount())
        ui->deviceComboBox->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_deviceComboBox_currentIndexChanged(int index)
{
    if (index == -1)        return;

    ui->descriptionLabel->setText(QString::fromStdWString(devices_.deviceDescription(index)));
    ui->guidLabel->setText(QString::fromStdWString(devices_.deviceGuid(index)));
    ui->driverModuleLabel->setText(QString::fromStdWString(devices_.deviceDriverModule(index)));

    DSCAPS capabilities = devices_.deviceCapabilities(index);
    if (capabilities.dwFlags & DSCAPS_CERTIFIED)        ui->certifiedCheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_CONTINUOUSRATE)   ui->rangeBasedCheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_EMULDRIVER)       ui->enumeratedCheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_PRIMARY16BIT)     ui->primary16CheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_PRIMARY8BIT)      ui->primary8CheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_PRIMARYMONO)      ui->monoPrimaryCheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_PRIMARYSTEREO)    ui->stereoPrimaryCheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_SECONDARY16BIT)   ui->secondary16CheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_SECONDARY8BIT)    ui->secondary8CheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_SECONDARYMONO)    ui->monoSecondaryCheckBox->setChecked(true);
    if (capabilities.dwFlags & DSCAPS_SECONDARYSTEREO)  ui->stereoSecondaryCheckBox->setChecked(true);

    ui->minMaxSampleRateLabel->setText(QString::number(capabilities.dwMinSecondarySampleRate) +
                                       "-" +
                                       QString::number(capabilities.dwMaxSecondarySampleRate) +
                                       "Hz");
    ui->maxHardwareBufferLabel->setText(QString::number(capabilities.dwMaxHwMixingAllBuffers));
    ui->maxStaticLabel->setText(QString::number(capabilities.dwMaxHwMixingStaticBuffers));
    ui->maxStreamingLabel->setText(QString::number(capabilities.dwMaxHwMixingStreamingBuffers));
    ui->unallocatedHardwareBufferLabel->setText(QString::number(capabilities.dwFreeHwMixingAllBuffers));
    ui->unallocatedStaticLabel->setText(QString::number(capabilities.dwFreeHwMixingStaticBuffers));
    ui->unallocatedStreamingLabel->setText(QString::number(capabilities.dwFreeHwMixingStreamingBuffers));
    ui->max3DLabel->setText(QString::number(capabilities.dwMaxHw3DAllBuffers));
    ui->maxStatic3DLabel->setText(QString::number(capabilities.dwMaxHw3DStaticBuffers));
    ui->max3DStreamingLabel->setText(QString::number(capabilities.dwMaxHw3DStreamingBuffers));
    ui->unallocated3DLabel->setText(QString::number(capabilities.dwFreeHw3DAllBuffers));
    ui->unallocatedStatic3DLabel->setText(QString::number(capabilities.dwFreeHw3DStaticBuffers));
    ui->unallocatedStream3DLabel->setText(QString::number(capabilities.dwFreeHw3DStreamingBuffers));
    ui->totalMemoryLabel->setText(QString::number(capabilities.dwTotalHwMemBytes));
    ui->freeMemoryLabel->setText(QString::number(capabilities.dwFreeHwMemBytes));
    ui->largestContinugousLabel->setText(QString::number(capabilities.dwMaxContigFreeHwMemBytes));
    ui->transferRateLabel->setText(QString::number(capabilities.dwUnlockTransferRateHwBuffers));
    ui->overheadLabel->setText(QString::number(capabilities.dwPlayCpuOverheadSwBuffers));

    DWORD speakerConfiguration = devices_.deviceConfiguration(index);
    ui->stereo5Subwoofer1Radio->setChecked(speakerConfiguration == DSSPEAKER_5POINT1_BACK);
    ui->stereo5Subwoofer1Radio->setChecked(speakerConfiguration == DSSPEAKER_5POINT1_SURROUND);
    ui->directPlayRadio->setChecked(speakerConfiguration == DSSPEAKER_DIRECTOUT);
    ui->headphoneRadio->setChecked(speakerConfiguration == DSSPEAKER_HEADPHONE);
    ui->monoSpeakerRadio->setChecked(speakerConfiguration == DSSPEAKER_MONO);
    ui->quadraSpeakerRadio->setChecked(speakerConfiguration == DSSPEAKER_QUAD);
    ui->stereoSpeakerRadio->setChecked(speakerConfiguration == DSSPEAKER_STEREO);
        ui->stereoDegree5Radio->setChecked(speakerConfiguration == DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_MIN));
        ui->stereoDegree10Radio->setChecked(speakerConfiguration == DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_NARROW));
        ui->stereoDegree20Radio->setChecked(speakerConfiguration == DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE));
        ui->stereoDegree180Radio->setChecked(speakerConfiguration == DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_MAX));
    ui->surroundpeakerRadio->setChecked(speakerConfiguration == DSSPEAKER_SURROUND);
    ui->stereo7Subwoofer1Radio->setChecked(speakerConfiguration == DSSPEAKER_7POINT1_WIDE);
    ui->stereo7Subwoofer1Radio->setChecked(speakerConfiguration == DSSPEAKER_7POINT1_SURROUND);
}
