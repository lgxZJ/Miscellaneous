#include "CustomeView.h"
#include <QApplication>
#include <QStandardItemModel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CustomeView customeView;
    customeView.setSelectionMode(QAbstractItemView::SingleSelection);
    customeView.setSelectionBehavior(QAbstractItemView::SelectItems);


    QStandardItemModel model;
    model.insertColumns(0, 2);
    model.insertRows(0, 2);
    model.setData(model.index(0, 0), QVariant("00"), Qt::DisplayRole);
    model.setData(model.index(0, 1), QVariant("01"), Qt::DisplayRole);
    model.setData(model.index(1, 0), QVariant("10"), Qt::DisplayRole);
    model.setData(model.index(1, 1), QVariant("11"), Qt::DisplayRole);

    customeView.setModel(&model);
    customeView.show();

    return a.exec();
}
