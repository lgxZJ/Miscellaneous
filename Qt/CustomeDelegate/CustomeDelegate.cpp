#include "CustomeDelegate.h"

#include <QPainter>
#include <QLineEdit>
#include <QHelpEvent>
#include <QToolTip>
#include <QThread>

std::default_random_engine CustomeDelegate::m_generator;
std::uniform_int_distribution<int> CustomeDelegate::m_randomColor(0, 255);
std::uniform_int_distribution<int> CustomeDelegate::m_randomSize(0, 50);

CustomeDelegate::CustomeDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{

}

void CustomeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid() || option.state == QStyle::State_None || !index.isValid())
        return;

    painter->fillRect(option.rect, QColor(m_randomColor(m_generator)));
}

//  not known
QSize CustomeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    auto size = m_randomSize(m_generator);
    return QSize(20, 20);
}

QWidget* CustomeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return new QLineEdit(parent);
}

void CustomeDelegate::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    Q_ASSERT(dynamic_cast<QLineEdit*>(editor) != 0);

    delete dynamic_cast<QLineEdit*>(editor);
}

bool CustomeDelegate::helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::ToolTip) {
        QToolTip::showText(QCursor::pos(), QString("CustomeDelegate Tooltip"), reinterpret_cast<QWidget*>(view), option.rect, 1000);
    } else if (event->type() == QEvent::WhatsThis) {
        QToolTip::showText(QCursor::pos(), QString("CustomeDelegate Whatsthis"), reinterpret_cast<QWidget*>(view), option.rect, 1000);
    }
    return true;
}

//  called when show or un-show
void CustomeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<QLineEdit*>(editor)->setText(index.data(Qt::DisplayRole).toString());
}

void CustomeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    model->setData(index, dynamic_cast<QLineEdit*>(editor)->text(), Qt::DisplayRole);
}

//  called when show
void CustomeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    dynamic_cast<QLineEdit*>(editor)->setFixedSize(option.rect.width(), option.rect.height());
    dynamic_cast<QLineEdit*>(editor)->move(option.rect.topLeft());
}
