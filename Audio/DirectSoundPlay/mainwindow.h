#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "WavPlayer.h"

namespace Ui {
    class MainWindow;
}

class QLabel;
class QSlider;

class MainWindow : public QMainWindow
				 , public IAudioEndNotify
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void wavPlayerAudioEnds() override;
	void wavPlayerProgressUpdated(unsigned currentPlayingTime) override;

	void customEvent(QEvent* event) override;

private slots:
    void on_openWaveButton_clicked(bool);
    void on_playButton_clicked(bool);
	void on_stopButton_clicked(bool);
    void on_playingTimeSlider_sliderReleased();
    void on_volumeSlider_valueChanged(int value);
    void on_channelSlider_valueChanged(int value);
    void on_frequencySlider_valueChanged(int value);

    void updateOneForm(QLabel* label, QSlider* slider, int value);

private:
    Ui::MainWindow *ui;

    WavPlayer      m_wavPlayer;
};

#endif // MAINWINDOW_H
