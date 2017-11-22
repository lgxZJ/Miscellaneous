#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDragEnterEvent>
#include <QPainter>
#include <QMimeData>
#include <QLabel>
#include <QDrag>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_dragLeavedColor(0xffffff),
    m_dragEnteredColor(0xF0FFFF),
    m_currentBkColor(0xffffff),
	m_acceptableRect(QRect(QPoint(50, 50), QPoint(200, 200))),
	m_highlightColor(0x98AFC7),
    m_unhighlightColor(Qt::lightGray),
    m_currentDragableAreaColor(Qt::lightGray),
    m_acceptableDropAction(Qt::CopyAction),
    m_actionGroup(this),
	m_mimeData(nullptr),
	m_imageLabel(nullptr),
	m_htmlLabel(nullptr),
    m_colorLabel(nullptr),
    m_dragLabel(nullptr)
{
    ui->setupUi(this);

	createMenus();
    this->setFixedSize(250, 250);
	addDragableWidgets();
    this->setAcceptDrops(true);
}

void MainWindow::createMenus()
{
	auto menu = this->menuBar()->addMenu(tr("Acceptable Drags"));

    auto copyAction = menu->addAction("Only Copy",
                        this, SLOT(onlyAcceptCopyDrag()),
                        QKeySequence(Qt::CTRL + Qt::Key_C));
    copyAction->setCheckable(true);
    copyAction->setChecked(true);
    m_actionGroup.addAction(copyAction);

    auto moveAction = menu->addAction("Only Move",
                        this, SLOT(onlyAcceptMoveDrag()),
                        QKeySequence(Qt::CTRL + Qt::Key_M));
    moveAction->setCheckable(true);
    moveAction->setChecked(false);
    m_actionGroup.addAction(moveAction);

    auto linkAction = menu->addAction("Only Link",
                        this, SLOT(onlyAcceptLinkDrag()),
                        QKeySequence(Qt::CTRL + Qt::Key_L));
    linkAction->setCheckable(true);
    linkAction->setChecked(false);
    m_actionGroup.addAction(linkAction);
}

void MainWindow::addDragableWidgets()
{
	m_imageLabel = new QLabel(this);
	m_imageLabel->setPixmap(QPixmap(":/img/image.jpg"));
    m_imageLabel->move(160, 205);
	m_imageLabel->resize(30, 30);
	m_imageLabel->show();

	m_colorLabel = new QLabel(this);
	m_colorLabel->setAutoFillBackground(true);
	auto palette = m_colorLabel->palette();
	palette.setColor(QPalette::Window, QColor(Qt::green));
	m_colorLabel->setPalette(palette);
	m_colorLabel->resize(30, 30);
	m_colorLabel->move(215, 205);
	m_colorLabel->show();

	m_htmlLabel = new QLabel(this);
	m_htmlLabel->setText("html");
	m_htmlLabel->move(215, 160);
	m_htmlLabel->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(QFont("Courier", 10));
	painter.setPen(Qt::red);
	auto& fontMetrics = painter.fontMetrics();

	painter.fillRect(QRect(0, 0, this->width(), this->height()), m_currentBkColor);
	painter.fillRect(m_acceptableRect, m_currentDragableAreaColor);

	if (m_mimeData == nullptr)
		return;

	//	we check the least popular format here to make them processed first!
	if (m_mimeData->hasHtml()) {
		painter.drawText(m_acceptableRect, Qt::AlignCenter, m_mimeData->html());
	} else if (m_mimeData->hasImage()) {
		auto image = m_mimeData->imageData().value<QImage>();
		painter.drawImage(m_acceptableRect, image);
	} else if (m_mimeData->hasColor()) {
		auto color = m_mimeData->colorData().value<QColor>();
		painter.drawText(m_acceptableRect, 
						 Qt::AlignCenter, 
						 QString("color(red,green,blue):%1,%2,%3")
							.arg(QString::number(color.red()))
							.arg(QString::number(color.green()))
							.arg(QString::number(color.blue())));
	} else if (m_mimeData->hasText()) {
		painter.drawText(m_acceptableRect, Qt::AlignCenter, m_mimeData->text());
	} else   if (m_mimeData->hasUrls()) {
		auto& urls = m_mimeData->urls();
		for (int i = 0; i < urls.size(); ++i) {
			painter.drawText(QPoint(0, painter.fontMetrics().ascent() + i * 20),
							 fontMetrics.elidedText(urls.at(i).toLocalFile(), Qt::ElideMiddle, m_acceptableRect.width()));
		}
	}
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->setDropAction(m_acceptableDropAction);
	event->accept();
    m_currentBkColor = m_dragEnteredColor;

    QWidget::dragEnterEvent(event);
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
	m_currentBkColor = m_dragLeavedColor;

    repaint();
    QWidget::dragLeaveEvent(event);
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    if (m_acceptableRect.contains(event->answerRect()))
        m_currentDragableAreaColor = m_highlightColor;
    else
		m_currentDragableAreaColor = m_unhighlightColor;

    repaint();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    m_currentDragableAreaColor = m_unhighlightColor;
    m_currentBkColor = m_dragLeavedColor;
    m_mimeData = event->mimeData();
  
    repaint();
	m_mimeData = nullptr;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        m_dragStartPos = event->pos();

        if (m_imageLabel->geometry().contains(event->pos()))
            m_dragLabel = m_imageLabel;
        else if (m_colorLabel->geometry().contains(event->pos()))
            m_dragLabel = m_colorLabel;
        else if (m_htmlLabel->geometry().contains(event->pos()))
            m_dragLabel = m_htmlLabel;
        else
            m_dragStartPos = QPoint();
    }

    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QDrag drag(this);

    if (m_dragStartPos.isNull())
        goto end;
    if ((event->pos() - m_dragStartPos).manhattanLength() < 5)
        goto end;

    QMimeData* mimeData = new QMimeData();
    if (m_dragLabel == m_htmlLabel)
        mimeData->setHtml("<html>This is a html page</html>");
    if (m_dragLabel == m_colorLabel)
        mimeData->setColorData(QVariant(QColor(Qt::cyan)));
    if (m_dragLabel == m_imageLabel)
        mimeData->setImageData(QVariant(QPixmap(":/img/image.jpg")));
    drag.setMimeData(mimeData);

	drag.exec(Qt::CopyAction | Qt::MoveAction | Qt::LinkAction);

end:
    QMainWindow::mouseMoveEvent(event);
}
