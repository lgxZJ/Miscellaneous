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

    void bindView(GraphicsView* view, CornerType type);

    void enableTransforms(bool enabled) { m_enableTransforms = enabled; }
    void setSizeHint(const QSize& size) { m_sizeHint = size; }

protected:
    void resizeEvent(QResizeEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    QSize sizeHint() const override { return m_sizeHint; }

private:
    QGraphicsPixmapItem*    m_holdingItem;

    QGraphicsPixmapItem     m_source;
    QGraphicsPixmapItem     m_destination;

    QGraphicsScene          m_scene;
    QSize                   m_sizeHint;
    bool                    m_enableTransforms;
    std::vector<std::pair<CornerType, GraphicsView*>> m_bindedViews;
};

#endif // GRAPHICSVIEW_H
