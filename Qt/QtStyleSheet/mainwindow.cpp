#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace CustomeStyleSheet
{
    //  needed because customize QMenu::separator need to
    //  change the size which in turn changes item size also
    //
    //  leave some space for indicator, QMenu::indicator is
    //  inside QMenu::item

    auto styleSheet =
        R"(
        QMenuBar {
            width:60px;
            background-color:white;
            border:1px dotted gray;

        }
        QMenuBar::item {
            height:15px;
            width:60px;

            background:transparent;
            border:2px groove gray;
            margin-right:1px;
        }
        QMenuBar::item:selected {
            color:green;
            background-color:rgb(236, 242, 245);
        }
        QMenuBar::item:pressed {
            color:green;
            background-color:rgb(233, 227, 227);
        }

        QMenu {
            background-color:white;
            border:1px solid rgb(0, 171, 255);
            padding:1px;
        }
        QMenu::item {

            height:15px;
            width:60px;

            background-color:white;
            margin:1px;

            padding:2px 2px 2px 20px;

            border:2px groove gray;
        }
        QMenu::item:selected {
            color:green;
            background-color:rgb(236, 242, 245);
        }
        QMenu::item:pressed {
            color:green;
            background-color:rgb(233, 227, 227);
        }

        QMenu::separator {
            height:1px;
            margin:1px;
            background-color:lightblue;
        }

        QMenu::indicator:checked {
            width:10px;
            height:10px;
            margin-left:3px;
            border:0px solid blue;
            image:url(:/checked-icon.png);
        }
        QMenu::right-arrow {
            width:15px;
            height:15px;
            image:url(:/right-arrow.png);
        }

        QToolBar {
            border: 1px dotted gray;
            background-color:white;
        }
        QToolBar::handle {
            width:15px;
            height:15px;
            image:url(:/drag-icon.png);
        }

        QTabWidget::pane {
            background:white;
            border:1px solid darkgray;
            top:-10px;
            border-radius:5px;
            background-color:qlineargradient(x0:0, y1:0, x1:0, y1:1,
                                             stop: 0.2 #111111,
                                             stop: 0.4 #222222,
                                             stop: 0.6 #444444,
                                             stop: 0.8 #888888,
                                             stop: 1.0 #ffffff);
        }
        QTabWidget::tab-bar {
            subcontrol-position:center;
        }
        QTabBar::tab {
            background:qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                       stop: 0 #E1E1E1, stop: 0.4 #DDDDDD,
                                       stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);
            border:2px solid gray;
            border-radius:4px;
            padding:3px 11px 3px 11px;
        }
        QTabBar::tab:!selected {
            margin-top:2px;
        }
        QTabBar::tab:hover {
        background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                    stop: 0 #eaeaea, stop: 0.4 #e4e4e4,
                                    stop: 0.5 #d7d7d7, stop: 1.0 #eaeaea);
        }
        QTabBar::tab:selected {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                        stop: 0 #fafafa, stop: 0.4 #f4f4f4,
                                        stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);
            margin-left:-4px;
            margin-right:-4px;
        }
        QTabBar::tab:first {
            border-bottom-right-radius:0px;
        }
        QTabBar::tab:last {
            border-bottom-left-radius:0px;
        }
        QTabBar::tab:middle {
            border-bottom-left-radius:0px;
            border-bottom-right-radius:0px;
        }
        QTabBar::tab:last:selected {
            margin-left:0px;
            margin-right:0px;
        }
        QTabBar::tab:first:selected {
            margin-left:0px;
            margin-right:0px;
        }

        QGroupBox {
            border:1px solid qlineargradient(x0:0, y1:0, x2:1, y2:0,
                                  stop: 0.0 #00ff7f,
                                  stop: 0.2 #4682b4,
                                  stop: 0.4 #d2b48c,
                                  stop: 0.6 #008080,
                                  stop: 0.8 #d8bfd8,
                                  stop: 1.0 #ff6347);

        }
        QGroupBox::title {
            subcontrol-origin:margin;
            subcontrol-position: top center;
            color:qlineargradient(x0:0, y1:0, x2:1, y2:0,
                                  stop: 0.0 #00ff7f,
                                  stop: 0.2 #4682b4,
                                  stop: 0.4 #d2b48c,
                                  stop: 0.6 #008080,
                                  stop: 0.8 #d8bfd8,
                                  stop: 1.0 #ff6347);
        }

        QComboBox {
            margin-top:0.5ex;
            border:2px groove gray;
            background-color:lightgray;
        }
        QComboBox:on {
            color:qlineargradient(x0:0, y1:0, x2:1, y2:0,
                                    stop: 0.0 #00ff7f,
                                    stop: 0.2 #4682b4,
                                    stop: 0.4 #d2b48c,
                                    stop: 0.6 #008080,
                                    stop: 0.8 #d8bfd8,
                                    stop: 1.0 #ff6347);
        }
        QComboBox QAbstractItemView {
            border:2px groove gray;
            selection-background-color:qlineargradient(x0:0, y1:0, x2:1, y2:0,
                                                        stop: 0.0 #00ff7f,
                                                        stop: 0.2 #4682b4,
                                                        stop: 0.4 #d2b48c,
                                                        stop: 0.6 #008080,
                                                        stop: 0.8 #d8bfd8,
                                                        stop: 1.0 #ff6347);
        }

        QLCDNumber {
            border:1px dotted yellow;
            color:lightblue;
        }
        QLCDNumber:hover {
            border: 1px solid yellow;
            color: blue;
        }

        QRadioButton {
            color:lightgray;
        }
        QRadioButton:hover {
            color:fuchsia;
        }
        QRadioButton:checked  {
            color:qlineargradient(x0:0, y1:0, x2:1, y2:0,
                                stop: 0.0 #00ff7f,
                                stop: 0.2 #4682b4,
                                stop: 0.4 #d2b48c,
                                stop: 0.6 #008080,
                                stop: 0.8 #d8bfd8,
                                stop: 1.0 #ff6347);
        }

        QCheckBox {
            color:lightgray;
        }
        QCheckBox:hover {
            color:fuchsia;
        }
        QCheckBox:checked  {
            color:qlineargradient(x0:0, y1:0, x2:1, y2:0,
                                stop: 0.0 #00ff7f,
                                stop: 0.2 #4682b4,
                                stop: 0.4 #d2b48c,
                                stop: 0.6 #008080,
                                stop: 0.8 #d8bfd8,
                                stop: 1.0 #ff6347);
        }

        QProgressBar {
            border-radius:3px;
            border: 2px solid lightgray;
            text-align:center;
            background-color:#f8f8f8;
        }
        QProgressBar::chunk {
            background-color:purple;
            margin:1px 2px 1px 0px;
            width:10%;
            border-radius:2px;
        }

        QDial {
            background-color:#008080;
        }
        )";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->layout()->setMargin(0);
    qApp->setStyleSheet(CustomeStyleSheet::styleSheet);
}

MainWindow::~MainWindow()
{
    delete ui;
}
