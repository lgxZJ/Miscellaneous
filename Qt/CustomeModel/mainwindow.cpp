#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mytablemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto model = new MyTableModel();
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}
