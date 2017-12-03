#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QXmlStreamEntityResolver>
#include <QString>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    this->resize(570, 480);
    m_treeWidget.setColumnCount(2);
    m_treeWidget.setColumnWidth(0, 150);
    m_treeWidget.setColumnWidth(1, 400);

	QStringList headers;
	headers << "Element" << "Content";
    m_treeWidget.setHeaderLabels(headers);

    this->setCentralWidget(&m_treeWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openXmlFile()
{
	QString selectedFilter = tr("Xmls (*.xml)");
    auto filename = QFileDialog::getOpenFileName(this,
                                                 tr("Choose Xml"),
                                                 tr("../xml-news.xml"),
                                                 tr("Xmls (*.xml)"),
                                                 &selectedFilter,
												 (QFileDialog::Options(
													QFileDialog::DontUseNativeDialog |
													QFileDialog::ReadOnly)));
    if(!filename.isNull()) {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);

        this->setWindowTitle(filename);
        readXmlFile(file);
    }
}

void MainWindow::readXmlFile(QFile& file)
{
    m_xmlReader.setDevice(&file);

    readDocumentElement();
    readDtd();
    readProcessingInstruction();
    readCDATA();
    readComment();
    readOtherElements();

    m_treeWidget.expandAll();
}

void MainWindow::readDocumentElement()
{
    m_xmlReader.readNext();
    if (m_xmlReader.isStartDocument()) {
        auto item = new QTreeWidgetItem(
                    QStringList("Document Element"));
        item->setText(1, "xml version:" + m_xmlReader.documentVersion().toString() +
                      "    encoding:" + m_xmlReader.documentEncoding().toString() +
                      "    is standalone:" + (m_xmlReader.isStandaloneDocument() ? QString("true") : QString("false")));
        item->setBackgroundColor(1, QColor(Qt::green));
        m_treeWidget.addTopLevelItem(item);
    }
}

void MainWindow::readDtd()
{
    while (!m_xmlReader.isDTD()) {
        m_xmlReader.readNext();
    }
    auto item = new QTreeWidgetItem(QStringList("[DTD]"));
    item->setText(1, m_xmlReader.text().toString());
    item->setBackgroundColor(1, QColor(Qt::darkMagenta));
    m_treeWidget.addTopLevelItem(item);
}

void MainWindow::readProcessingInstruction()
{
    while (!m_xmlReader.isProcessingInstruction()) {
        m_xmlReader.readNext();
    }
    auto item = new QTreeWidgetItem(QStringList("[ProcessingInstruction]"));
    item->setText(1, "target: " + m_xmlReader.processingInstructionTarget().toString() +
                     "    content: " + m_xmlReader.processingInstructionData().toString());
    item->setBackgroundColor(1, QColor(Qt::yellow));
    m_treeWidget.addTopLevelItem(item);
}

void MainWindow::readCDATA()
{
    while (!m_xmlReader.isCDATA()) {
        m_xmlReader.readNext();
    }
    auto item = new QTreeWidgetItem(QStringList("[CDATA]"));
    item->setText(1, m_xmlReader.text().toString());
    item->setBackgroundColor(1, QColor(Qt::cyan));
    m_treeWidget.addTopLevelItem(item);

}

void MainWindow::readComment()
{
    while (!m_xmlReader.isComment()) {
        m_xmlReader.readNext();
    }
    auto item = new QTreeWidgetItem(QStringList("[Comment]"));
    item->setText(1, m_xmlReader.text().toString());
    item->setBackgroundColor(1, QColor(Qt::darkCyan));
    m_treeWidget.addTopLevelItem(item);
}

void MainWindow::readOtherElements()
{
    QString parentElements;
    parentElements.append("nitf,");
    parentElements.append("head,");
    parentElements.append("body,");
    parentElements.append("headline,");
    parentElements.append("byline,");
    parentElements.append("dateline,");

    QString textElements;
    textElements.append("title,");
    textElements.append("hl1,");
    textElements.append("bytag,");
    textElements.append("location,");
    textElements.append("date,");

    QTreeWidgetItem* currentItem = nullptr;
    while (!m_xmlReader.atEnd()) {
        if (m_xmlReader.readNextStartElement() == false) {
            //  on most conditions, we reach the end of elements, skip it
            if (m_xmlReader.hasError())     return;
            else							continue;
        }

        auto elementName = m_xmlReader.name().toString();
        if (parentElements.contains(elementName + ",")) {
            if (m_xmlReader.name() == "nitf") {
                currentItem = new QTreeWidgetItem(QStringList(elementName));
                m_treeWidget.addTopLevelItem(currentItem);
            } else {
                auto item = new QTreeWidgetItem(QStringList(elementName));
                currentItem->addChild(item);
                currentItem = item;
            }
        }
        else if (textElements.contains(elementName + ",")) {
            auto textItem = new QTreeWidgetItem(QStringList(elementName));
            textItem->setBackgroundColor(1, QColor(Qt::lightGray));
            textItem->setText(1, m_xmlReader.readElementText());
            currentItem->addChild(textItem);

            if (elementName == "location") {
                continue;
            }

            if (elementName == "title")
                currentItem = m_treeWidget.topLevelItem(m_treeWidget.topLevelItemCount() - 1);
            else
                currentItem = m_treeWidget.topLevelItem(m_treeWidget.topLevelItemCount() - 1)->child(1);
        }
    }

}
