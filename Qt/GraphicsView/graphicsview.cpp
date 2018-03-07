#include "graphicsview.h"
#include "DragableGraphicsPixmapItem.h"
#include <QMouseEvent>
#include <QTransform>
#include <QDrag>
#include <QMimeData>

#include <QDebug>

GraphicsView::GraphicsView(QWidget* parent)
    : QGraphicsView(parent)
    , m_holdingItem(nullptr)
    , m_enableTransforms(true)
{
    auto sourceItem = new DragableGraphicsPixmapItem(QPixmap("../source.jpg"));
    sourceItem->setToolTip("Sound Source(drag to move)");
    m_scene.addItem(sourceItem);

    auto destinationItem = new DragableGraphicsPixmapItem(QPixmap("../destination.jpg"));
    destinationItem->setToolTip("Sound Destination(drag to move)");
    destinationItem->setPos(sourceItem->boundingRect().bottomRight());
    m_scene.addItem(destinationItem);

    setScene(&m_scene);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    m_sizeHint = QGraphicsView::sizeHint();
    updateGeometry();
}

void GraphicsView::bindView(GraphicsView* view, GraphicsView::CornerType type)
{
    view->setAcceptDrops(false);
    view->setScene(&m_scene);
    m_bindedViews.emplace_back(std::make_pair(type, view));
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    m_scene.setSceneRect(QRect(QPoint(0, 0), event->size()));

    for (auto viewPair : m_bindedViews) {
        QRectF viewRect;
        if (viewPair.first == GraphicsView::LeftTop)
            viewRect = QRectF(QPoint(0, 0), event->size() / 2);
        if (viewPair.first == GraphicsView::RightTop)
            viewRect = QRectF(QPoint(event->size().width() / 2, 0), event->size() / 2);
        if (viewPair.first == GraphicsView::LeftBottom)
            viewRect = QRectF(QPoint(0, event->size().height() / 2), event->size() / 2);
        if (viewPair.first == GraphicsView::RightBottom)
            viewRect = QRectF(QPoint(event->size().width() / 2, event->size().height() / 2), event->size() / 2);

        viewPair.second->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        viewPair.second->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        viewPair.second->setSceneRect(viewRect);
    }

    QGraphicsView::resizeEvent(event);
}

void GraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void GraphicsView::dropEvent(QDropEvent *event)
{
    DragableGraphicsPixmapItem* item = reinterpret_cast<DragableGraphicsPixmapItem*>
            (event->mimeData()->text().toULongLong());
    item->setPos(event->pos());

    event->accept();
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (m_enableTransforms) {
        switch (event->button()) {
        case Qt::LeftButton:    this->scale(1.1, 1.1);      break;
        case Qt::MiddleButton:  this->resetTransform();     break;
        case Qt::RightButton:   this->translate(10, 10);    break;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    //  May not call the implementation of super class which
    //  delivers the event into QGraphicsScene and do not accept
    //  drag move events itself.
    event->accept();
}



