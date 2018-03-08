#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QEvent>

#include <cassert>

/////////////////////////////////////////////////////////////////////////

class AudioEndsEvent : public QEvent
{
public:
	AudioEndsEvent() : QEvent(type()) {}

	QEvent::Type type() { return (QEvent::Type)(QEvent::User + 1); }
};

struct AudioProgressEvent : public QEvent
{
    AudioProgressEvent(unsigned currentPlayingTime)
        : QEvent(type())
        , m_currentPlayingTime(currentPlayingTime)
    {}

    QEvent::Type type()         { return (QEvent::Type)(QEvent::User + 2); }

    unsigned m_currentPlayingTime;
};

/////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->sourceGroupBox->setEnabled(false);
    ui->listenerGroupBox->setEnabled(false);
	m_wavPlayer.setAudioEndsNotify(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wavPlayerAudioEnds()
{
	//	postEvent() takes the ownership of the 'new'ed object
	qApp->postEvent(this, new AudioEndsEvent());
}

void MainWindow::wavPlayerProgressUpdated(unsigned currentPlayingTime)
{
    qApp->postEvent(this, new AudioProgressEvent(currentPlayingTime));
}

void MainWindow::customEvent(QEvent* event)
{
	auto audioEndsEvent = dynamic_cast<AudioEndsEvent*>(event);
	if (audioEndsEvent != nullptr) {
		ui->stopButton->setEnabled(false);
		ui->playButton->setEnabled(false);

		ui->playButton->setText("Play");
		audioEndsEvent->setAccepted(true);
	}

	auto audioProgressEvent = dynamic_cast<AudioProgressEvent*>(event);
    if (audioProgressEvent != nullptr) {
        auto currentTime = audioProgressEvent->m_currentPlayingTime;
		if (!ui->playingTimeSlider->isSliderDown())
			ui->playingTimeSlider->setValue(currentTime);
        ui->currentTimeLabel->setText(QString("CurrentTime:%1s").arg(currentTime));
	}

	QObject::customEvent(event);
}

void MainWindow::on_openWaveButton_clicked(bool)
{
    auto filePath = QFileDialog::getOpenFileName(this, "Select a Wav file",
                                 QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                 "Wav files(*.wav)",
                                 Q_NULLPTR,
                                 QFileDialog::ReadOnly);
    if (!filePath.isEmpty()) {
        try {
            m_wavPlayer.setFile(filePath.toStdWString(), reinterpret_cast<HWND>(this->winId()));		

            updateInitialAudioBasicControlUis();
            updateInitialEffectChorusUis();

            if (m_wavPlayer.supportsEffect3D())
                m_wavPlayer.enableEffect3D(true),
                updateInitial3dUis();
        }
        catch (std::exception& exception) {
            QMessageBox::warning(this, "Player error", exception.what());
        }
    }
}

void MainWindow::updateInitialAudioBasicControlUis()
{
    auto totalTime = m_wavPlayer.getAudioTotalTime();
    ui->playingTimeSlider->setMinimum(0);
    ui->playingTimeSlider->setMaximum(totalTime);

    ui->currentTimeLabel->setText(QString("CurrentTime:0s"));
    ui->totalTimeLabel->setText(QString("TotalTime:%1s").arg(totalTime));

    ui->playButton->setEnabled(true);
    ui->playButton->setText("Play");
    ui->stopButton->setEnabled(false);

    updateOneForm(ui->volumeLabel, ui->volumeSlider, 100);
    m_wavPlayer.setVolume(100);

    updateOneForm(ui->channelLabel, ui->channelSlider, 0);
	if (!m_wavPlayer.supportsEffect3D())
		m_wavPlayer.setChannel(0);

    //	set minimium value also emit `valueChanged` signal, block signals first
    ui->frequencySlider->blockSignals(true);
    ui->frequencySlider->setMinimum(m_wavPlayer.getFrequencyMin());
    ui->frequencySlider->setMaximum(m_wavPlayer.getFrequencyMax());
    ui->frequencySlider->blockSignals(false);

    ui->frequencyHeader->setText(QString("Frequency(%1~%2)")
                                    .arg(m_wavPlayer.getFrequencyMin())
                                    .arg(m_wavPlayer.getFrequencyMax()));
    updateOneForm(ui->frequencyLabel, ui->frequencySlider, m_wavPlayer.getFrequency());
}

void MainWindow::updateInitialEffectChorusUis()
{
    //  set effect related sliders
    m_wavPlayer.addEffectOfType(WavPlayer::Chorus);
    m_wavPlayer.applyEffects();
    auto& baseController = m_wavPlayer.getEffectController(WavPlayer::Chorus);
    auto chorusController = dynamic_cast<EffectChorus*>(&baseController);
    assert(chorusController != nullptr);

    auto wetDryRange = EffectChorus::getWetDryRadioRange();
    updateOneForm(ui->wetDryLabel, ui->wetDrySlider, wetDryRange.m_min, wetDryRange.m_max, chorusController->getParams().fWetDryMix);

    auto depthRange = EffectChorus::getDepthRange();
    updateOneForm(ui->depthLabel, ui->depthSlider, depthRange.m_min, depthRange.m_max, chorusController->getParams().fDepth);

    auto feedbackRange = EffectChorus::getFeedbackRange();
    updateOneForm(ui->feedbackLabel, ui->feedbackSlider, feedbackRange.m_min, feedbackRange.m_max, chorusController->getParams().fFeedback);

    auto chorusFrequencyRange = EffectChorus::getFrequencyRange();
    updateOneForm(ui->chorusFrequencyLabel, ui->chorusFrequencySlider, chorusFrequencyRange.m_min, chorusFrequencyRange.m_max, chorusController->getParams().fFrequency);

    auto delayRange = EffectChorus::getDelayRange();
    updateOneForm(ui->delayLabel, ui->delaySlider, delayRange.m_min, delayRange.m_max, chorusController->getParams().fDelay);

    ui->sinRadio->setChecked(chorusController->getParams().lWaveform == EffectChorus::Sin);
    ui->triangleRadio->setChecked(chorusController->getParams().lWaveform == EffectChorus::Triangle);

    ui->neg90Radio->setChecked(chorusController->getParams().lPhase == EffectChorus::Neg90);
    ui->neg180Radio->setChecked(chorusController->getParams().lPhase == EffectChorus::Neg180);
    ui->zeroRadio->setChecked(chorusController->getParams().lPhase == EffectChorus::Zero);
    ui->pos90Radio->setChecked(chorusController->getParams().lPhase == EffectChorus::Pos90);
    ui->pos180Radio->setChecked(chorusController->getParams().lPhase == EffectChorus::Pos180);
}

void MainWindow::updateInitial3dUis()
{
#define SET_RANGE(widgetName, rangeMin, rangeMax, initValue)                    \
    ui->widgetName->blockSignals(true);                                         \
    ui->widgetName->setRange(rangeMin, rangeMax);                               \
    ui->widgetName->setValue(initValue);                                        \
    ui->widgetName->blockSignals(false);                                        \
    ui->widgetName##Label->setText(#widgetName + QString("(%1)").arg(initValue));

	auto source = m_wavPlayer.get3DSource();
	float min, max;
	source->GetMinDistance(&min);
	source->GetMaxDistance(&max);
    SET_RANGE(minDistance, 1, 100, min);
    SET_RANGE(maxDistance, 100, 1000, max);

    D3DVECTOR vector;
    source->GetPosition(&vector);
    SET_RANGE(posX, 0, 100, vector.x);
    SET_RANGE(posY, 0, 100, vector.y);
    SET_RANGE(posZ, 0, 100, vector.z);

    DWORD insideAngle, outsideAngle;
    source->GetConeAngles(&insideAngle, &outsideAngle);
    SET_RANGE(coneAnglesInside, DS3D_MINCONEANGLE, DS3D_MAXCONEANGLE, insideAngle);
    SET_RANGE(coneAnglesOutside, DS3D_MINCONEANGLE, DS3D_MAXCONEANGLE, outsideAngle);

    source->GetConeOrientation(&vector);
    SET_RANGE(coneOrientationX, -50, 50, vector.x);
    SET_RANGE(coneOrientationY, -50, 50, vector.y);
    SET_RANGE(coneOrientationZ, -50, 50, vector.z);

    long volume;
    source->GetConeOutsideVolume(&volume);
    SET_RANGE(coneOutsideVolume, DSBVOLUME_MIN, DSBVOLUME_MAX, volume);

    source->GetVelocity(&vector);
    SET_RANGE(volecityX, 0, 100, vector.x);
    SET_RANGE(volecityY, 0, 100, vector.y);
    SET_RANGE(volecityZ, 0, 100, vector.z);

    auto listener = m_wavPlayer.get3DListener();
    float distanceFactor;
    listener->GetDistanceFactor(&distanceFactor);
    SET_RANGE(distanceFactor, 0, 100, distanceFactor);

    float rolloffFactor;
    listener->GetRolloffFactor(&rolloffFactor);
    SET_RANGE(rolloffFactor, DS3D_MINROLLOFFFACTOR, DS3D_MAXROLLOFFFACTOR, rolloffFactor);

    float dopplerFactor;
    listener->GetDopplerFactor(&dopplerFactor);
    SET_RANGE(dopplerFactor, DS3D_MINDOPPLERFACTOR , DS3D_MAXDOPPLERFACTOR, dopplerFactor);

    D3DVECTOR front, top;
    listener->GetOrientation(&front, &top);
    SET_RANGE(orientationFrontX, -50, 50, front.x);
    SET_RANGE(orientationFrontY, -50, 50, front.y);
    SET_RANGE(orientationFrontZ, -50, 50, front.z);
    SET_RANGE(orientationTopX, -50, 50, top.x);
    SET_RANGE(orientationTopY, -50, 50, top.y);
    SET_RANGE(orientationTopZ, -50, 50, top.z);

    listener->GetPosition(&vector);
    SET_RANGE(listenerPosX, -50, 50, vector.x);
    SET_RANGE(listenerPosY, -50, 50, vector.y);
    SET_RANGE(listenerPosZ, -50, 50, vector.z);

    listener->GetVelocity(&vector);
    SET_RANGE(listenerVolecityX, -50, 50, vector.x);
    SET_RANGE(listenerVolecityY, -50, 50, vector.y);
    SET_RANGE(listenerVolecityZ, -50, 50, vector.z);

#undef SET_RANGE
}

void MainWindow::updateOneForm(QLabel* label, QSlider* slider, int value)
{
    label->setText(QString::number(value));
    slider->setValue(value);
}

void MainWindow::updateOneForm(QLabel *label, QSlider *slider, int min, int max, int current)
{
    slider->blockSignals(true);

    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setValue(current);
    label->setText(label->text() + QString("(%1)").arg(static_cast<int>(current)));

    slider->blockSignals(false);
}

void MainWindow::on_playButton_clicked(bool)
{
    assert(!m_wavPlayer.isPlaying());

	try {
		m_wavPlayer.play();

		ui->playButton->setDisabled(true);
		ui->playButton->setText("Continue");
		ui->stopButton->setEnabled(true);
        ui->playingTimeSlider->setEnabled(true);
        ui->volumeSlider->setEnabled(true);
        ui->channelSlider->setEnabled(true);
        ui->frequencySlider->setEnabled(true);

        ui->wetDrySlider->setEnabled(true);
        ui->feedbackSlider->setEnabled(true);
        ui->depthSlider->setEnabled(true);
        ui->chorusFrequencySlider->setEnabled(true);
        ui->delaySlider->setEnabled(true);
        ui->waveformTypesGroupBox->setEnabled(true);
        ui->phaseTypeGroupBox->setEnabled(true);

        if (m_wavPlayer.supportsEffect3D())
			ui->channelSlider->setEnabled(false),
            ui->sourceGroupBox->setEnabled(true),
            ui->listenerGroupBox->setEnabled(true);
	}
	catch (std::exception& exception) {
		QMessageBox::warning(this, "stop error", exception.what());
	}
}

void MainWindow::on_stopButton_clicked(bool)
{
	try {
		m_wavPlayer.stop();
		ui->playButton->setText("Resume");
		ui->playButton->setEnabled(true);
		ui->stopButton->setEnabled(false);
	}
	catch (std::exception& exception) {
		QMessageBox::warning(this, "stop error", exception.what());
    }
}

void MainWindow::on_playingTimeSlider_sliderReleased()
{
	//	the last second is not the actual data second, it is only used
	//	to indicate users the total audio time. if user drag the slider
	//	and release at this position, skip it.
	if (ui->playingTimeSlider->sliderPosition() == ui->playingTimeSlider->maximum())
		return;

    try {
        m_wavPlayer.playFrom(ui->playingTimeSlider->sliderPosition());
    } catch (std::exception& exception) {
        QMessageBox::warning(this, "playFrom error", exception.what());
    }
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    try {
        m_wavPlayer.setVolume(value);
        ui->volumeLabel->setText(QString::number(value));
    } catch (std::exception& exception) {
        QMessageBox::warning(this, "setVolume error", exception.what());
    }
}

void MainWindow::on_channelSlider_valueChanged(int value)
{
    try {
        m_wavPlayer.setChannel(value);
        ui->channelLabel->setText(QString::number(value));
    } catch (std::exception& exception) {
        QMessageBox::warning(this, "setChannel error", exception.what());
    }
}

void MainWindow::on_frequencySlider_valueChanged(int value)
{
    try {
        m_wavPlayer.setFrequency(value);
        ui->frequencyLabel->setText(QString::number(value));
    } catch (std::exception& exception) {
        QMessageBox::warning(this, "setFrequency error", exception.what());
    }
}

////////////////////////////////////////////////////////////////////////////////////

#define CHANGE_SLIDER_EFFECT_PARAM(sliderName, labelName, paramName, baseLabelString)                           \
    void MainWindow::on_##sliderName##_valueChanged(int value)                                                  \
    {                                                                                                           \
        try {                                                                                                   \
            m_wavPlayer.stop();                                                                                 \
                                                                                                                \
            auto controller = dynamic_cast<EffectChorus&>(m_wavPlayer.getEffectController(WavPlayer::Chorus));  \
            auto params = controller.getParams();                                                               \
            params.paramName = value;                                                                           \
            controller.setParams(params);                                                                       \
                                                                                                                \
            m_wavPlayer.resume();                                                                               \
                                                                                                                \
            ui->labelName->setText(QString(#baseLabelString "(%1)").arg(value));                                \
        } catch (std::exception& exception) {                                                                   \
            QMessageBox::warning(this, QString(#baseLabelString) + QString(" change error"), exception.what()); \
        }                                                                                                       \
    }

CHANGE_SLIDER_EFFECT_PARAM(wetDrySlider,           wetDryLabel,            fWetDryMix, WetDry)
CHANGE_SLIDER_EFFECT_PARAM(depthSlider,            depthLabel,             fDepth,     Depth)
CHANGE_SLIDER_EFFECT_PARAM(feedbackSlider,         feedbackLabel,          fFeedback,  Feedback)
CHANGE_SLIDER_EFFECT_PARAM(chorusFrequencySlider,  chorusFrequencyLabel,   fFeedback,  Frequency)
CHANGE_SLIDER_EFFECT_PARAM(delaySlider,            delayLabel,             fFeedback,  Delay)

#undef CHANGE_EFFECT_PARAM

//////////////////////////////////////////////////////////////////////////

#define CHANGE_RADIO_EFFECT_PARAM(radioButtonName, paramName, paramValue, errorString)                          \
    void MainWindow::on_##radioButtonName##_clicked(bool checked)                                               \
    {                                                                                                           \
        if (checked == false)                                                                                   \
            return;                                                                                             \
                                                                                                                \
        try {                                                                                                   \
            m_wavPlayer.stop();                                                                                 \
                                                                                                                \
            auto controller = dynamic_cast<EffectChorus&>(m_wavPlayer.getEffectController(WavPlayer::Chorus));  \
            auto params = controller.getParams();                                                               \
            params.paramName = paramValue;                                                                      \
            controller.setParams(params);                                                                       \
                                                                                                                \
            m_wavPlayer.resume();                                                                               \
        } catch (std::exception& exception) {                                                                   \
            QMessageBox::warning(this, #errorString, exception.what());                                         \
        }                                                                                                       \
    }

CHANGE_RADIO_EFFECT_PARAM(sinRadio,     lWaveform,  DSFXCHORUS_WAVE_SIN,        SinWaveformChangeError)
CHANGE_RADIO_EFFECT_PARAM(triangleRadio,lWaveform,  DSFXCHORUS_WAVE_TRIANGLE,   TriangleWaveformChangeError)
CHANGE_RADIO_EFFECT_PARAM(neg90Radio,   lPhase,     DSFXCHORUS_PHASE_NEG_90,    Neg90PhaseChangeError)
CHANGE_RADIO_EFFECT_PARAM(neg180Radio,  lPhase,     DSFXCHORUS_PHASE_NEG_180,   Neg180PhaseChangeError)
CHANGE_RADIO_EFFECT_PARAM(zeroRadio,    lPhase,     DSFXCHORUS_PHASE_ZERO,      ZeroPhaseChangeError)
CHANGE_RADIO_EFFECT_PARAM(pos90Radio,   lPhase,     DSFXCHORUS_PHASE_90,        Pos90PhaseChangeError)
CHANGE_RADIO_EFFECT_PARAM(pos180Radio,  lPhase,     DSFXCHORUS_PHASE_180,       Pos180PhaseChangeError)

void MainWindow::on_minDistance_valueChanged(int value)
{
    if (m_wavPlayer.get3DSource()->SetMinDistance(value, DS3D_IMMEDIATE) != DS_OK)
        throw std::exception("SetMinDistance error");
    ui->minDistanceLabel->setText(QString("minDistance(%1)").arg(value));
}

void MainWindow::on_maxDistance_valueChanged(int value)
{
	if (m_wavPlayer.get3DSource()->SetMaxDistance(value, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetMaxDistance error");
    ui->maxDistanceLabel->setText(QString("maxDistance(%1)").arg(value));
}

void MainWindow::on_posX_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetPosition(&vector) != DS_OK)
		throw std::exception("GetPosition error");
	if (m_wavPlayer.get3DSource()->SetPosition(value, vector.y, vector.z, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetMaxDistance error");
    ui->posXLabel->setText(QString("posX(%1)").arg(value));
}

void MainWindow::on_posY_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetPosition(&vector) != DS_OK)
		throw std::exception("GetPosition error");
	if (m_wavPlayer.get3DSource()->SetPosition(vector.x, value, vector.z, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetMaxDistance error");
    ui->posYLabel->setText(QString("posY(%1)").arg(value));
}

void MainWindow::on_posZ_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetPosition(&vector) != DS_OK)
		throw std::exception("GetPosition error");
	if (m_wavPlayer.get3DSource()->SetPosition(vector.x, vector.y, value, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetMaxDistance error");
    ui->posZLabel->setText(QString("posZ(%1)").arg(value));
}

void MainWindow::on_coneAnglesInside_valueChanged(int value)
{
	DWORD insideAngle, outsideAngle;
	if (m_wavPlayer.get3DSource()->GetConeAngles(&insideAngle, &outsideAngle) != DS_OK)
		throw std::exception("GetConeAngles error");

    if (static_cast<unsigned>(value) > outsideAngle)
		ui->coneAnglesOutside->setValue(value),
		outsideAngle = value;
	if (m_wavPlayer.get3DSource()->SetConeAngles(value, outsideAngle, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetConeAngles error");

    ui->coneAnglesInsideLabel->setText(QString("coneAnglesInside(%1)").arg(value));
}

void MainWindow::on_coneAnglesOutside_valueChanged(int value)
{
	DWORD insideAngle, outsideAngle;
	if (m_wavPlayer.get3DSource()->GetConeAngles(&insideAngle, &outsideAngle) != DS_OK)
		throw std::exception("GetConeAngles error");
	
    if (static_cast<unsigned>(insideAngle) > value)
		ui->coneAnglesInside->setValue(value),
		insideAngle = value;
	if (m_wavPlayer.get3DSource()->SetConeAngles(insideAngle, value, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetConeAngles error");

    ui->coneAnglesOutsideLabel->setText(QString("coneAngleOutside(%1)").arg(value));
}

void MainWindow::on_coneOrientationX_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetConeOrientation(&vector) != DS_OK)
		throw std::exception("GetConeOrientation error");

	if (!(vector.y == 0.0 && vector.z == 0.0 && value == 0))
		if (m_wavPlayer.get3DSource()->SetConeOrientation(value, vector.y, vector.z, DS3D_IMMEDIATE) != DS_OK)
			throw std::exception("GetConeOrientation error");

    ui->coneOrientationXLabel->setText(QString("coneOrientationX(%1)").arg(value));
}

void MainWindow::on_coneOrientationY_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetConeOrientation(&vector) != DS_OK)
		throw std::exception("GetConeOrientation error");

	if (!(vector.x == 0.0 && vector.z == 0.0 && value == 0))
		if (m_wavPlayer.get3DSource()->SetConeOrientation(vector.x, value, vector.z, DS3D_IMMEDIATE) != DS_OK)
			throw std::exception("GetConeOrientation error");

    ui->coneOrientationYLabel->setText(QString("coneOrientationY(%1)").arg(value));
}

void MainWindow::on_coneOrientationZ_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetConeOrientation(&vector) != DS_OK)
		throw std::exception("GetConeOrientation error");

	//	the orientation vector could not be all zeros in three directions!
	if (!(vector.x == 0.0 && vector.y == 0.0 && value == 0))
		if (m_wavPlayer.get3DSource()->SetConeOrientation(vector.x, vector.y, value, DS3D_IMMEDIATE) != DS_OK)
			throw std::exception("GetConeOrientation error");

    ui->coneOrientationZLabel->setText(QString("coneOrientationZ(%1)").arg(value));
}

void MainWindow::on_coneOutsideVolume_valueChanged(int value)
{
	if (m_wavPlayer.get3DSource()->SetConeOutsideVolume(value, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetConeOutsideVolume error");
    ui->coneOutsideVolumeLabel->setText(QString("coneOutsideVolume(%1)").arg(value));
}

void MainWindow::on_volecityX_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetVelocity(&vector) != DS_OK)
		throw std::exception("GetVelocity error");
	if (m_wavPlayer.get3DSource()->SetVelocity(value, vector.y, vector.z, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetVelocity error");
    ui->volecityXLabel->setText(QString("volecityX(%1)").arg(value));
}

void MainWindow::on_volecityY_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetVelocity(&vector) != DS_OK)
		throw std::exception("GetVelocity error");
	if (m_wavPlayer.get3DSource()->SetVelocity(vector.x, value, vector.z, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetVelocity error");
    ui->volecityYLabel->setText(QString("volecityY(%1)").arg(value));
}

void MainWindow::on_volecityZ_valueChanged(int value)
{
	D3DVECTOR vector;
	if (m_wavPlayer.get3DSource()->GetVelocity(&vector) != DS_OK)
		throw std::exception("GetVelocity error");
	if (m_wavPlayer.get3DSource()->SetVelocity(vector.x, vector.y, value, DS3D_IMMEDIATE) != DS_OK)
		throw std::exception("SetVelocity error");
    ui->volecityZLabel->setText(QString("volecityZ(%1)").arg(value));
}

void MainWindow::on_distanceFactor_valueChanged(int value)
{
    if (m_wavPlayer.get3DListener()->SetDistanceFactor(value, DS3D_IMMEDIATE) != DS_OK)
        throw std::exception("SetDistanceFactor error");
    ui->distanceFactorLabel->setText(QString("distanceFactor(%1 m/unit)").arg(value));
}

void MainWindow::on_rolloffFactor_valueChanged(int value)
{
    if (m_wavPlayer.get3DListener()->SetRolloffFactor(value, DS3D_IMMEDIATE) != DS_OK)
        throw std::exception("SetRolloffFactor error");
    ui->rolloffFactorLabel->setText(QString("rolloffFactor(%1 times)").arg(value));
}

void MainWindow::on_dopplerFactor_valueChanged(int value)
{
    if (m_wavPlayer.get3DListener()->SetDopplerFactor(value, DS3D_IMMEDIATE) != DS_OK)
        throw std::exception("SetDopplerFactor error");
    ui->dopplerFactorLabel->setText(QString("dopplerFactor(%1 times)").arg(value));
}

void MainWindow::on_orientationFrontX_valueChanged(int value)
{
    D3DVECTOR front, top;
    if (m_wavPlayer.get3DListener()->GetOrientation(&front, &top) != DS_OK)
        throw std::exception("GetOrientation error");

    if (!(front.y == 0.0 && front.z == 0.0 && value == 0 ||
          top.x == 0.0 && top.y == 0.0 && top.z == 0))
        if (m_wavPlayer.get3DListener()->SetOrientation(value, front.y, front.z,
                                                    top.x, top.y, top.z,
                                                    DS3D_IMMEDIATE)
                != DS_OK)
            throw std::exception("SetOrientation error");
    ui->orientationFrontXLabel->setText(QString("orientationFrontX(%1)").arg(value));
}

void MainWindow::on_orientationFrontY_valueChanged(int value)
{
    D3DVECTOR front, top;
    if (m_wavPlayer.get3DListener()->GetOrientation(&front, &top) != DS_OK)
        throw std::exception("GetOrientation error");

    if (!(front.x == 0.0 && front.z == 0.0 && value == 0 ||
          top.x == 0.0 && top.y == 0.0 && top.z == 0))
        if (m_wavPlayer.get3DListener()->SetOrientation(front.x, value, front.z,
                                                    top.x, top.y, top.z,
                                                    DS3D_IMMEDIATE)
                != DS_OK)
            throw std::exception("SetOrientation error");
    ui->orientationFrontYLabel->setText(QString("orientationFrontY(%1)").arg(value));
}

void MainWindow::on_orientationFrontZ_valueChanged(int value)
{
    D3DVECTOR front, top;
    if (m_wavPlayer.get3DListener()->GetOrientation(&front, &top) != DS_OK)
        throw std::exception("GetOrientation error");

    if (!(front.x == 0.0 && front.y == 0.0 && value == 0 ||
          top.x == 0.0 && top.y == 0.0 && top.z == 0))
        if (m_wavPlayer.get3DListener()->SetOrientation(front.x, front.y, value,
                                                    top.x, top.y, top.z,
                                                    DS3D_IMMEDIATE)
                != DS_OK)
            throw std::exception("SetOrientation error");
    ui->orientationFrontZLabel->setText(QString("orientationFrontZ(%1)").arg(value));
}

void MainWindow::on_orientationTopX_valueChanged(int value)
{
    D3DVECTOR front, top;
    if (m_wavPlayer.get3DListener()->GetOrientation(&front, &top) != DS_OK)
        throw std::exception("GetOrientation error");

    if (!(front.x == 0.0 && front.y == 0.0 && front.z == 0 ||
          value == 0.0 && top.y == 0.0 && top.z == 0))
        if (m_wavPlayer.get3DListener()->SetOrientation(front.x, front.y, front.z,
                                                    value, top.y, top.z,
                                                    DS3D_IMMEDIATE)
                != DS_OK)
            throw std::exception("SetOrientation error");
    ui->orientationTopXLabel->setText(QString("orientationTopX(%1)").arg(value));
}

void MainWindow::on_orientationTopY_valueChanged(int value)
{
    D3DVECTOR front, top;
    if (m_wavPlayer.get3DListener()->GetOrientation(&front, &top) != DS_OK)
        throw std::exception("GetOrientation error");

    if (!(front.x == 0.0 && front.y == 0.0 && front.z == 0 ||
          top.x == 0.0 && value == 0.0 && top.z == 0))
        if (m_wavPlayer.get3DListener()->SetOrientation(front.x, front.y, front.z,
                                                    top.x, value, top.z,
                                                    DS3D_IMMEDIATE)
                != DS_OK)
            throw std::exception("SetOrientation error");
    ui->orientationTopYLabel->setText(QString("orientationTopY(%1)").arg(value));
}

void MainWindow::on_orientationTopZ_valueChanged(int value)
{
    D3DVECTOR front, top;
    if (m_wavPlayer.get3DListener()->GetOrientation(&front, &top) != DS_OK)
        throw std::exception("GetOrientation error");

    if (!(front.x == 0.0 && front.y == 0.0 && front.z == 0 ||
          top.x == 0.0 && top.y == 0.0 && value == 0))
        if (m_wavPlayer.get3DListener()->SetOrientation(front.x, front.y, front.z,
                                                    top.x, top.y, value,
                                                    DS3D_IMMEDIATE)
                != DS_OK)
            throw std::exception("SetOrientation error");
    ui->orientationTopZLabel->setText(QString("orientationTopZ(%1)").arg(value));
}

void MainWindow::on_listenerPosX_valueChanged(int value)
{
    D3DVECTOR vector;
    if (m_wavPlayer.get3DListener()->GetPosition(&vector) != DS_OK)
        throw std::exception("GetVelocity error");

    if (!(value == 0.0 && vector.y == 0.0 && vector.z == 0))
        if (m_wavPlayer.get3DListener()->SetPosition(value, vector.y, vector.z, DS3D_IMMEDIATE) != DS_OK)
            throw std::exception("SetVelocity error");
    ui->listenerPosXLabel->setText(QString("listenerPosX(%1)").arg(value));
}

void MainWindow::on_listenerPosY_valueChanged(int value)
{
    D3DVECTOR vector;
    if (m_wavPlayer.get3DListener()->GetPosition(&vector) != DS_OK)
        throw std::exception("GetVelocity error");

    if (!(vector.x == 0.0 && value == 0.0 && vector.z == 0))
        if (m_wavPlayer.get3DListener()->SetPosition(vector.x, value, vector.z, DS3D_IMMEDIATE) != DS_OK)
            throw std::exception("SetVelocity error");
    m_wavPlayer.get3DListener()->CommitDeferredSettings();
    ui->listenerPosYLabel->setText(QString("listenerPosY(%1)").arg(value));
}

void MainWindow::on_listenerPosZ_valueChanged(int value)
{
    D3DVECTOR vector;
    if (m_wavPlayer.get3DListener()->GetPosition(&vector) != DS_OK)
        throw std::exception("GetVelocity error");

    if (!(vector.x == 0.0 && vector.y == 0.0 && value == 0))
        if (m_wavPlayer.get3DListener()->SetPosition(vector.x, vector.y, value, DS3D_IMMEDIATE) != DS_OK)
            throw std::exception("SetVelocity error");
    ui->listenerPosZLabel->setText(QString("listenerPosZ(%1)").arg(value));
}

void MainWindow::on_listenerVolecityX_valueChanged(int value)
{
    D3DVECTOR vector;
    if (m_wavPlayer.get3DListener()->GetVelocity(&vector) != DS_OK)
        throw std::exception("GetVelocity error");

    if (!(value == 0.0 && vector.y == 0.0 && vector.z == 0))
        if (m_wavPlayer.get3DListener()->SetVelocity(value, vector.y, vector.z, DS3D_IMMEDIATE) != DS_OK)
            throw std::exception("SetVelocity error");
    ui->listenerVolecityXLabel->setText(QString("listenerVolecityX(%1)").arg(value));
}

void MainWindow::on_listenerVolecityY_valueChanged(int value)
{
    D3DVECTOR vector;
    if (m_wavPlayer.get3DListener()->GetVelocity(&vector) != DS_OK)
        throw std::exception("GetVelocity error");

    if (!(vector.x == 0.0 && value == 0.0 && vector.z == 0))
        if (m_wavPlayer.get3DListener()->SetVelocity(vector.x, value, vector.z, DS3D_IMMEDIATE) != DS_OK)
            throw std::exception("SetVelocity error");
    ui->listenerVolecityYLabel->setText(QString("listenerVolecityY(%1)").arg(value));
}

void MainWindow::on_listenerVolecityZ_valueChanged(int value)
{
    D3DVECTOR vector;
    if (m_wavPlayer.get3DListener()->GetVelocity(&vector) != DS_OK)
        throw std::exception("GetVelocity error");
    if (m_wavPlayer.get3DListener()->SetVelocity(vector.x, vector.y, value, DS3D_IMMEDIATE) != DS_OK)
        throw std::exception("SetVelocity error");
    ui->listenerVolecityZLabel->setText(QString("listenerVolecityZ(%1)").arg(value));
}
