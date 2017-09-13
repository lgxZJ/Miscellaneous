#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>

class MyTableModel : public QAbstractTableModel
{
protected:
    //  read only virtuals
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
};

#endif // MYTABLEMODEL_H
