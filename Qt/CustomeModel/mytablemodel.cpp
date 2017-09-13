#include "mytablemodel.h"

Qt::ItemFlags MyTableModel::flags(const QModelIndex &index) const
{
    return QAbstractTableModel::flags(index);
}

QVariant MyTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        return index.column() * index.column();
    }
    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical) {
        if (role == Qt::DisplayRole)    return QVariant("row:" + QString::number(section));
        else                            return QVariant();
    }
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole)    return QVariant("column:" + QString::number(section));
        else                            return QVariant();
    }
}

//  if `parent` is invalid, return the whole table row count!
//  else return the children row count of the `parent`
int MyTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return 10;
}

//  if `parent` is invalid, return the whole table column count!
//  else return the children column count of the `parent`
int MyTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return 10;
}
