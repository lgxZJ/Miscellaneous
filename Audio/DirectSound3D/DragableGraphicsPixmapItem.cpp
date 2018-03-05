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

        auto mimeData = new QMimeData();
        mimeData->setText(QString::number(qulonglong(this)));
        drag->setMimeData(mimeData);

        drag->setDragCursor(this->pixmap().scaled(this->pixmap().width() / 2, this->pixmap().height() / 2), Qt::MoveAction);

        drag->exec(Qt::MoveAction);
    }

    QGraphicsPixmapItem::mousePressEvent(event);
}
