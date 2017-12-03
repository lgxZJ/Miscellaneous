#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void openXmlFile();
    void readXmlFile(QFile& file);
    void readDocumentElement();
    void readDtd();
    void readProcessingInstruction();
    void readCDATA();
    void readComment();
    void readOtherElements();

private:
    Ui::MainWindow *ui;

    QTreeWidget         m_treeWidget;
    QXmlStreamReader    m_xmlReader;
};

#endif // MAINWINDOW_H
