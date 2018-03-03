#include "DragableGraphicsPixmapItem.h"

#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneMouseEvent>

#include <QDebug>

void DragableGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = true;
}

void DragableGraphicsPixmapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_pressed = false;
}

void DragableGraphicsPixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_pressed) {
        auto drag = new QDrag((QObject*)event->widget());
        drag->setMimeData(new QMimeData());
        drag->start();
    }

    QGraphicsPixmapItem::mousePressEvent(event);
}
