#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CustomeStyle.h"

#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto nativeStyles = QStyleFactory::keys();
    for (auto it = nativeStyles.begin(); it != nativeStyles.end(); ++it) {
        ui->styleComboBox->addItem(*it);
    }
    ui->styleComboBox->addItem("CustomeStyle");

    connect(ui->enableEditCheckBox, &QCheckBox::clicked,
            [this](bool checked) {
                ui->styleComboBox->setEditable(checked);
            });
    connect(ui->applyStyleButton, &QPushButton::released,
            [this]() {
                auto styleType = ui->styleComboBox->currentText();
                auto app = static_cast<QApplication*>(QCoreApplication::instance());
                if (styleType == "CustomeStyle")
                    app->setStyle(new CustomeStyle());
                else
                    app->setStyle(QStyleFactory::create(styleType));
            });

    ui->styleComboBox->setCurrentIndex(0);
    ui->applyStyleButton->released();
}

MainWindow::~MainWindow()
{
    delete ui;
}
