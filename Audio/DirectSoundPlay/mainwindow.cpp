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
		}
		catch (std::exception& exception) {
			QMessageBox::warning(this, "Player error", exception.what());
		}

        auto totalTime = m_wavPlayer.getAudioTotalTime();
        ui->playingTimeSlider->setMinimum(0);
        ui->playingTimeSlider->setMaximum(totalTime);

        ui->currentTimeLabel->setText(QString("CurrentTime:0s"));
        ui->totalTimeLabel->setText(QString("TotalTime:%1s").arg(totalTime));

        ui->playButton->setEnabled(true);
		ui->playButton->setText("Play");
		ui->stopButton->setEnabled(false);
    }
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
