import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    title: qsTr("quassel")
    color: cppThemeHolderObject.commonWindowBkColor

    width: 260
    height: 200

    minimumWidth: 150

    TableView {
        id: tableView

        property int nonLastColumnWidth: 150
        property int lastColumnImplicitWidth: nonLastColumnWidth

        anchors.fill: parent
        alternatingRowColors: false

        TableViewColumn {
            title: "MessageId"
            width: tableView.nonLastColumnWidth
        }
        TableViewColumn {
            title: "SendId"
            width: tableView.nonLastColumnWidth
        }
        TableViewColumn {
            title: "Message"
            width:  {
                //  substract two border pixel to get the real width
                var actualWidth = tableView.width - (tableView.columnCount - 1) * tableView.nonLastColumnWidth - 2
                return actualWidth > tableView.lastColumnImplicitWidth
                            ? actualWidth
                            : tableView.lastColumnImplicitWidth
            }
        }
    }
}
