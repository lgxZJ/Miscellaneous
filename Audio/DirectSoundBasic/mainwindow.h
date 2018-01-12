#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DirectSoundBasic.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_deviceComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    DirectSoundBasic devices_;
};

#endif // MAINWINDOW_H
