#ifndef CUSTOMEVIEW_H
#define CUSTOMEVIEW_H

#include <QAbstractItemView>

class QLabel;

class CustomeView : public QAbstractItemView
{
    Q_OBJECT

public:
    CustomeView(QWidget *parent = 0);
    ~CustomeView();

    //  virtuals
    virtual void paintEvent(QPaintEvent* event);

protected:
    //  pure virtuals
    virtual QRect visualRect(const QModelIndex &index) const;
    virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    virtual QModelIndex indexAt(const QPoint &point) const;
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers);
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden(const QModelIndex &index) const;
    virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command);
    virtual QRegion visualRegionForSelection(const QItemSelection &selection) const;

private:
    void disableResize();
    void updateOneRect(QPainter& painter, const QRect rect, const QColor& color, const QModelIndex& index);

private:
    QSize       m_childSquareSize;
    QModelIndex m_currentModelIndex;

    QRect       m_leftBottomSquare;
    QRect       m_leftTopSquare;
    QRect       m_rightTopSquare;
    QRect       m_rightBottomSquare;
};

#endif // CUSTOMEVIEW_H
