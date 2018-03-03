#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsProxyWidget>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_wholeView.bindView(&m_leftTopView, GraphicsView::LeftTop);
    m_wholeView.bindView(&m_rightTopView, GraphicsView::RightTop);
    m_wholeView.bindView(&m_leftBottomView, GraphicsView::LeftBottom);
    m_wholeView.bindView(&m_rightBottomView, GraphicsView::RightBottom);

    m_layout.addWidget(&m_wholeView, 0, 0, 1, 2);
    m_layout.addWidget(&m_leftTopView, 1, 0);
    m_layout.addWidget(&m_rightTopView, 1, 1);
    m_layout.addWidget(&m_leftBottomView, 2, 0);
    m_layout.addWidget(&m_rightBottomView, 2, 1);
    m_layout.addWidget(&m_paramGroup, 3, 0, 1, 2);

    m_layout.setRowStretch(0, 2);
    m_layout.setRowStretch(1, 1);
    m_layout.setRowStretch(2, 1);
    m_layout.setRowStretch(3, 0);

    centralWidget()->setLayout(&m_layout);
}
