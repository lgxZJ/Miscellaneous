#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJSEngine>

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
    void on_lineEditEvaluateJs_textChanged(const QString& text);
    void on_lineEditEvaluateJs_returnPressed();
    void on_buttonEvaluateJs_clicked(bool);

    void on_lineEditPropertyLeft_textChanged(const QString& text);
    void on_lineEditPropertyRight_textChanged(const QString& text);
    void on_lineEditPropertyOperator_textChanged(const QString& text);
    void on_lineEditPropertyOperator_returnPressed();
    void on_buttonEvaluatePropertyCalculateResult_clicked(bool);

    void on_buttonChangeInJs_clicked(bool);

private:
    Ui::MainWindow *ui;

    QJSEngine               m_jsEngine;
};

#endif // MAINWINDOW_H
