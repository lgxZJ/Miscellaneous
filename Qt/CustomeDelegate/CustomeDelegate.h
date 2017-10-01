#ifndef WIDGET_H
#define WIDGET_H

#include <QAbstractItemDelegate>
#include <random>

class CustomeDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    CustomeDelegate(QObject *parent = 0);

    virtual void paint(QPainter *painter,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

    virtual QWidget* createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const;
    virtual void destroyEditor(QWidget *editor,
                               const QModelIndex &index) const;
    virtual bool helpEvent(QHelpEvent *event,
                           QAbstractItemView *view,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index);
    virtual void setEditorData(QWidget *editor,
                               const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const ;
    virtual void updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const;

private:
    static std::default_random_engine m_generator;
    static std::uniform_int_distribution<int> m_randomColor;
    static std::uniform_int_distribution<int> m_randomSize;
};

#endif // WIDGET_H
