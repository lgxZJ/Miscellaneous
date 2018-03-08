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
    void updateInitial3dUis();

    void on_minDistance_valueChanged(int value);
    void on_maxDistance_valueChanged(int value);
    void on_posX_valueChanged(int value);
    void on_posY_valueChanged(int value);
	void on_posZ_valueChanged(int value);
    void on_coneAnglesInside_valueChanged(int value);
    void on_coneAnglesOutside_valueChanged(int value);
    void on_coneOrientationX_valueChanged(int value);
    void on_coneOrientationY_valueChanged(int value);
    void on_coneOrientationZ_valueChanged(int value);
    void on_coneOutsideVolume_valueChanged(int value);
    void on_volecityX_valueChanged(int value);
	void on_volecityY_valueChanged(int value);
	void on_volecityZ_valueChanged(int value);

    void on_distanceFactor_valueChanged(int value);

    void on_rolloffFactor_valueChanged(int value);

    void on_dopplerFactor_valueChanged(int value);

    void on_orientationFrontX_valueChanged(int value);

    void on_orientationFrontY_valueChanged(int value);

    void on_orientationFrontZ_valueChanged(int value);

    void on_orientationTopX_valueChanged(int value);

    void on_orientationTopY_valueChanged(int value);

    void on_orientationTopZ_valueChanged(int value);

    void on_listenerPosX_valueChanged(int value);

    void on_listenerPosY_valueChanged(int value);

    void on_listenerPosZ_valueChanged(int value);

    void on_listenerVolecityX_valueChanged(int value);

    void on_listenerVolecityY_valueChanged(int value);

    void on_listenerVolecityZ_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    WavPlayer      m_wavPlayer;
};

#endif // MAINWINDOW_H
