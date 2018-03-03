#include "graphicsview.h"
#include "DragableGraphicsPixmapItem.h"
#include <QMouseEvent>
#include <QTransform>
#include <QDrag>

#include <QDebug>

GraphicsView::GraphicsView(QWidget* parent)
    : QGraphicsView(parent)
    , m_holdingItem(nullptr)
{
    auto sourceItem = new DragableGraphicsPixmapItem(QPixmap("../source.jpg"));
    sourceItem->setToolTip("Sound Source(drag to move)");
    m_scene.addItem(sourceItem);

    auto destinationItem = new DragableGraphicsPixmapItem(QPixmap("../destination.jpg"));
    destinationItem->setToolTip("Sound Destination(drag to move)");
    destinationItem->setPos(sourceItem->boundingRect().bottomRight());
    m_scene.addItem(destinationItem);

    setScene(&m_scene);

    qDebug() << size();
    qDebug() << geometry();
}

void GraphicsView::bindView(QGraphicsView* view, GraphicsView::CornerType type)
{
    view->setScene(&m_scene);
    m_bindedViews.emplace_back(std::make_pair(type, view));
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    Q_ASSERT(m_bindedViews.size() <= 4);

    qDebug() << event->size();
    m_scene.setSceneRect(QRect(QPoint(0, 0), event->size()));

    for (auto viewPair : m_bindedViews) {
        QRectF viewRect;
        if (viewPair.first == GraphicsView::LeftTop)
            viewRect = QRectF(QPoint(0, 0), geometry().size() / 2);
        if (viewPair.first == GraphicsView::RightTop)
            viewRect = QRectF(QPoint((geometry().left() + geometry().right()) / 2, 0), geometry().size() / 2);
        if (viewPair.first == GraphicsView::LeftBottom)
            viewRect = QRectF(QPoint(0, (geometry().top() + geometry().bottom()) / 2), geometry().size() / 2);
        if (viewPair.first == GraphicsView::RightBottom)
            viewRect = QRectF(QPoint(geometry().width() / 2, geometry().height() / 2), geometry().size() / 2);
    qDebug() << viewRect << viewPair.second;
        viewPair.second->setSceneRect(viewRect);
    }

    QGraphicsView::resizeEvent(event);
}

