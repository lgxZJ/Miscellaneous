#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class QLabel;
class QMimeData;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent(QDragLeaveEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

public slots:
    void onlyAcceptMoveDrag()   { m_acceptableDropAction = Qt::MoveAction; }
    void onlyAcceptCopyDrag()   { m_acceptableDropAction = Qt::CopyAction; }
    void onlyAcceptLinkDrag()   { m_acceptableDropAction = Qt::LinkAction; }

private:
	void createMenus();
	void addDragableWidgets();

    Ui::MainWindow *ui;

    QColor			m_currentBkColor;
	QColor			m_currentDragableAreaColor;
    const QColor	m_dragLeavedColor;
    const QColor	m_dragEnteredColor;
    const QColor	m_highlightColor;
	const QColor	m_unhighlightColor;

    Qt::DropAction  m_acceptableDropAction;
    QActionGroup    m_actionGroup;
	QRect			m_acceptableRect;

	const QMimeData*		m_mimeData;
	QLabel*					m_imageLabel;
	QLabel*					m_htmlLabel;
	QLabel*					m_colorLabel;

    QPoint          m_dragStartPos;
    QLabel*         m_dragLabel;
};

#endif // MAINWINDOW_H
