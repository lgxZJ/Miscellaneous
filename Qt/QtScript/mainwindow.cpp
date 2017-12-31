#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMetaProperty>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QTreeWidget>
#include <QListWidget>
#include <QFile>
#include <QWebEngineView>

//////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMetaObject::connectSlotsByName(this);
    m_jsEngine.globalObject().setProperty("cppButton", m_jsEngine.newQObject(ui->buttonChangeInJs));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//////////////////////////////////////////////////////////////////////////

void MainWindow::on_lineEditEvaluateJs_textChanged(const QString &text)
{
    if (!text.isEmpty())
        ui->buttonEvaluateJs->setEnabled(true);
    if (text.isEmpty())
        ui->lineEditJsResult->clear(),
        ui->lineEditJsResult->setEnabled(false);
}

void MainWindow::on_lineEditEvaluateJs_returnPressed()
{
    ui->buttonEvaluateJs->clicked(true);
}

void MainWindow::on_buttonEvaluateJs_clicked(bool)
{
    ui->lineEditJsResult->setText(
                m_jsEngine.evaluate(ui->lineEditEvaluateJs->text()).toString());
    ui->lineEditJsResult->setEnabled(ui->buttonEvaluateJs->isEnabled());
}

////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_lineEditPropertyLeft_textChanged(const QString &leftOperand)
{
    if (leftOperand.isEmpty())
        ui->lineEditPropertyRight->clear(),
        ui->lineEditPropertyRight->setEnabled(false);
    else
        ui->lineEditPropertyRight->setEnabled(true);
}

void MainWindow::on_lineEditPropertyRight_textChanged(const QString &rightOperand)
{
    if (rightOperand.isEmpty())
        ui->lineEditPropertyOperator->clear(),
        ui->lineEditPropertyOperator->setEnabled(false);
    else
        ui->lineEditPropertyOperator->setEnabled(true);
}

void MainWindow::on_lineEditPropertyOperator_textChanged(const QString &operatorr)
{
    if (!operatorr.isEmpty())
        ui->buttonEvaluatePropertyCalculateResult->setEnabled(true);
    if (operatorr.isEmpty())
        ui->lineEditEvaluatePropertyResult->clear(),
        ui->lineEditEvaluatePropertyResult->setEnabled(false);
}

void MainWindow::on_lineEditPropertyOperator_returnPressed()
{
    ui->buttonEvaluatePropertyCalculateResult->clicked(true);
}

void MainWindow::on_buttonEvaluatePropertyCalculateResult_clicked(bool)
{
    auto jsObject = m_jsEngine.newObject();
    jsObject.setProperty("leftOperand", ui->lineEditPropertyLeft->text());
    jsObject.setProperty("rightOperand", ui->lineEditPropertyRight->text());
    m_jsEngine.globalObject().setProperty("cppObject", jsObject);

    ui->lineEditEvaluatePropertyResult->setText(m_jsEngine.evaluate(
        "cppObject.leftOperand" +
        ui->lineEditPropertyOperator->text() +
        "cppObject.rightOperand").toString());
    ui->lineEditEvaluatePropertyResult->setEnabled(
        ui->buttonEvaluatePropertyCalculateResult->isEnabled());
}

////////////////////////////////////////////////////////////////////////

void MainWindow::on_buttonChangeInJs_clicked(bool)
{
    QFile jsFile(":/js/demo.js");
    if (jsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        auto jsStr = QString::fromStdString(jsFile.readAll().toStdString());
        auto jsResult = m_jsEngine.evaluate(jsStr);

        if (jsResult.isError())
            ui->buttonChangeInJs->setText(jsResult.toString());
    }
}

