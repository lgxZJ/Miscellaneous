import QtQuick.Window 2.2
import QtQuick.Controls 1.4


Window {
    title: qsTr("Hot lists")
    color: cppThemeHolderObject.commonWindowBkColor

    width: 255
    height: 190

    minimumWidth: 150

    TableView {
        id: tableView

        property int nonLastColumnWidth: 150
        property int lastColumnImplicitWidth: nonLastColumnWidth

        anchors.fill: parent
        alternatingRowColors: false

        TableViewColumn {
            title: qsTr("Chat")
            //  substract 2 border pixels
            width: tableView.width - 2
        }

    }
}
