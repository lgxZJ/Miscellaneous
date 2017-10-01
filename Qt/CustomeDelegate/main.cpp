#include "CustomeDelegate.h"
#include <QApplication>

#include <QTableView>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTableView view;

    QStandardItemModel model;
    model.insertColumns(0, 2);
    model.insertRows(0, 2);
    model.setData(model.index(0, 0), QVariant("A"), Qt::DisplayRole);
    model.setData(model.index(0, 1), QVariant("B"), Qt::DisplayRole);
    model.setData(model.index(1, 0), QVariant("C"), Qt::DisplayRole);
    model.setData(model.index(1, 1), QVariant("D"), Qt::DisplayRole);

    view.setModel(&model);
    view.setItemDelegate(new CustomeDelegate());
    view.show();

    return a.exec();
}
