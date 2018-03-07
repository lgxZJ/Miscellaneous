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
            m_wavPlayer.enableEffect3D(true),
            ui->sourceGroupBox->setEnabled(true);
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

}

void MainWindow::on_maxDistance_valueChanged(int value)
{

}

void MainWindow::on_posX_valueChanged(int value)
{

}

void MainWindow::on_posY_valueChanged(int value)
{

}

void MainWindow::on_coneAnglesInside_valueChanged(int value)
{

}

void MainWindow::on_coneAnglesOutside_valueChanged(int value)
{

}

void MainWindow::on_coneOrientationX_valueChanged(int value)
{

}

void MainWindow::on_coneOrientationY_valueChanged(int value)
{

}

void MainWindow::on_coneOrientationZ_valueChanged(int value)
{

}

void MainWindow::on_coneOutsideVolume_valueChanged(int value)
{

}

void MainWindow::on_volecity_valueChanged(int value)
{

}
