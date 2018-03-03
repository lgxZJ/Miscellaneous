#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graphicsview.h"
#include <QGridLayout>
#include <QGroupBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);


private:
    Ui::MainWindow *ui;

    QGridLayout             m_layout;

    GraphicsView            m_wholeView;
    QGraphicsView           m_leftTopView;
    QGraphicsView           m_rightTopView;
    QGraphicsView           m_leftBottomView;
    QGraphicsView           m_rightBottomView;

    QGroupBox               m_paramGroup;
};

#endif // MAINWINDOW_H
