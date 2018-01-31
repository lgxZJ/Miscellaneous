#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QEvent>

#include <cassert>

class AudioEndsEvent : public QEvent
{
public:
	AudioEndsEvent() : QEvent(type()) {}

	QEvent::Type type() { return (QEvent::Type)(QEvent::User + 1); }
};

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
	qApp->postEvent(this, new AudioEndsEvent());
}

void MainWindow::customEvent(QEvent* event)
{
	auto audioEndsEvent = dynamic_cast<AudioEndsEvent*>(event);
	if (audioEndsEvent != nullptr) {
		ui->stopButton->setEnabled(false);
		ui->playButton->setEnabled(false);

		ui->playButton->setText("Play");
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
            ui->playButton->setEnabled(true);
			ui->playButton->setText("Play");
			ui->stopButton->setEnabled(false);
        } catch (std::exception& exception) {
            QMessageBox::warning(this, "Player error", exception.what());
        }
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