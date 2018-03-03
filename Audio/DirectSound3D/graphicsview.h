#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <qgraphicsview.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include <vector>

class GraphicsView : public QGraphicsView
{
public:
    enum CornerType { LeftTop, RightTop, LeftBottom, RightBottom };

    GraphicsView(QWidget* parent = nullptr);

    void bindView(QGraphicsView* view, CornerType type);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsPixmapItem*    m_holdingItem;

    QGraphicsPixmapItem     m_source;
    QGraphicsPixmapItem     m_destination;

    QGraphicsScene          m_scene;
    std::vector<std::pair<CornerType, QGraphicsView*>> m_bindedViews;
};

#endif // GRAPHICSVIEW_H
