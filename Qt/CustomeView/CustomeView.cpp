#include "CustomeView.h"
#include <QPainter>
#include <QDebug>

CustomeView::CustomeView(QWidget *parent)
    : QAbstractItemView(parent)
    , m_currentModelIndex()
{
    this->setModel(nullptr);
    disableResize();

    m_childSquareSize    = QSize(this->viewport()->width() / 3, this->viewport()->height() / 3);
    m_leftTopSquare      = QRect(QPoint(0, 0), m_childSquareSize);
    m_leftBottomSquare   = QRect(QPoint(0, this->height() / 3 * 2), m_childSquareSize);
    m_rightTopSquare     = QRect(QPoint(this->width() / 3 * 2, 0), m_childSquareSize);
    m_rightBottomSquare  = QRect(QPoint(this->width() / 3 * 2, this->height() / 3 * 2), m_childSquareSize);
}

void CustomeView::disableResize()
{
    const int cellSize = 200;

    this->setMaximumWidth(cellSize);
    this->setMaximumHeight(cellSize);
    this->setMinimumWidth(cellSize);
    this->setMinimumHeight(cellSize);

    this->resize(cellSize, cellSize);
    this->viewport()->resize(cellSize, cellSize);
}

CustomeView::~CustomeView()
{
}

void CustomeView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this->viewport());

    if (this->model() != nullptr) {
        updateOneRect(painter, m_leftTopSquare, 0xff22ff, this->model()->index(0, 0));
        updateOneRect(painter, m_rightTopSquare, 0xff2244, this->model()->index(0, 1));
        updateOneRect(painter, m_rightBottomSquare, 0x999911, this->model()->index(1, 0));
        updateOneRect(painter, m_leftBottomSquare, 0x992244, this->model()->index(1, 1));
    }
    if (this->selectionMode()) {
        auto selectedIndexes = this->selectionModel()->selectedIndexes();
        for (auto index : selectedIndexes) {
            if (index == this->model()->index(0, 0))
                painter.fillRect(m_leftTopSquare.adjusted(20, 20, -20, -20), Qt::cyan);
            if (index == this->model()->index(0, 1))
                painter.fillRect(m_rightTopSquare.adjusted(20, 20, -20, -20), Qt::cyan);
            if (index == this->model()->index(1, 0))
                painter.fillRect(m_rightBottomSquare.adjusted(20, 20, -20, -20), Qt::cyan);
            if (index == this->model()->index(1, 1))
                painter.fillRect(m_leftBottomSquare.adjusted(20, 20, -20, -20), Qt::cyan);
        }

    }

    QAbstractItemView::paintEvent(event);
}

void CustomeView::updateOneRect(QPainter& painter, const QRect rect,
                                const QColor& color, const QModelIndex& index)
{
    painter.fillRect(rect, color);
    painter.drawText(rect, 0, this->model()->data(index).toString());
}

QRect CustomeView::visualRect(const QModelIndex &index) const
{
    if (!index.isValid())   return QRect();

    int row = index.row();
    int column = index.column();
    if (row == 0) {
        if (column == 0)    return m_leftTopSquare;
        if (column == 1)    return m_rightTopSquare;
    }
    if (row == 1) {
        if (column == 0)    return m_rightBottomSquare;
        if (column == 1)    return m_leftBottomSquare;
    }

    return QRect();
}

void CustomeView::scrollTo(const QModelIndex &index, QAbstractItemView::ScrollHint hint)
{
    //  do nothing
}

//  used when users clicks at the client area
QModelIndex CustomeView::indexAt(const QPoint &point) const
{
    Q_ASSERT(this->model() != nullptr);

    if (m_leftTopSquare.contains(point))    return this->model()->index(0, 0);
    if (m_rightTopSquare.contains(point))   return this->model()->index(0, 1);
    if (m_rightBottomSquare.contains(point))return this->model()->index(1, 0);
    if (m_leftBottomSquare.contains(point)) return this->model()->index(1, 1);

    return QModelIndex();
}

QModelIndex CustomeView::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    Q_ASSERT(this->model() != nullptr);

    if (cursorAction != QAbstractItemView::MoveRight ||
        !m_currentModelIndex.isValid())
        return QModelIndex();
    else {
        auto row = m_currentModelIndex.row();
        auto column = m_currentModelIndex.column();
        if (row == 0) {
            if (column == 0)    return this->model()->index(0, 1);
            if (column == 1)    return this->model()->index(1, 0);
        }
        if (row == 1) {
            if (column == 0)    return this->model()->index(1, 1);
            if (column == 1)    return this->model()->index(0, 0);
        }
    }
}

int CustomeView::horizontalOffset() const
{
    return 0;
}

int CustomeView::verticalOffset() const
{
    return 0;
}

bool CustomeView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

void CustomeView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    if (m_leftTopSquare.contains(rect))
        m_currentModelIndex = this->model()->index(0, 0);
    else if (m_rightTopSquare.contains(rect))
        m_currentModelIndex = this->model()->index(0, 1);
    else if (m_rightBottomSquare.contains(rect))
        m_currentModelIndex = this->model()->index(1, 0);
    else if (m_leftBottomSquare.contains(rect))
        m_currentModelIndex = this->model()->index(1, 1);

    if (m_currentModelIndex.isValid())
        this->selectionModel()->select(m_currentModelIndex, command);
}

QRegion CustomeView::visualRegionForSelection(const QItemSelection &selection) const
{
    return QRegion();
}
