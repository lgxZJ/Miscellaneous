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

    void on_wetDrySlider_valueChanged(int value);
    void on_depthSlider_valueChanged(int value);
    void on_feedbackSlider_valueChanged(int value);
    void on_chorusFrequencySlider_valueChanged(int value);
    void on_delaySlider_valueChanged(int value);
    void on_sinRadio_clicked(bool checked);
    void on_triangleRadio_clicked(bool checked);
    void on_neg90Radio_clicked(bool checked);
    void on_neg180Radio_clicked(bool checked);
    void on_zeroRadio_clicked(bool checked);
    void on_pos90Radio_clicked(bool checked);
    void on_pos180Radio_clicked(bool checked);


    void updateOneForm(QLabel* label, QSlider* slider, int value);
    void updateOneForm(QLabel* label, QSlider* slider, int min, int max, int current);
    void updateInitialAudioBasicControlUis();
    void updateInitialEffectChorusUis();

private:
    Ui::MainWindow *ui;

    WavPlayer      m_wavPlayer;
};

#endif // MAINWINDOW_H
