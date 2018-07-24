import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    color: cppThemeHolderObject.commonWindowBkColor
    width: 610
    height: 300

    TableView {
        property int nonLastColumnWidth: 250

        id: tableView
        anchors.fill: parent
        alternatingRowColors: false

        TableViewColumn {
            id: chatColumn
            title: "聊天"
            width: tableView.nonLastColumnWidth
        }
        TableViewColumn {
            id: topicColumn
            title: "主题"
            width: tableView.nonLastColumnWidth
        }
        TableViewColumn {
            id: nickAccountColumn

            property int lastColumnImplicitWidth: 80

            title: "昵称账户"
            //  substract 2 pixels to make the horizontal scrollbar behaves correctly.
            width: {
                //  substract two border pixel to get the real width
                var actualWidth = tableView.width - (tableView.columnCount - 1) * tableView.nonLastColumnWidth - 2
                return actualWidth > lastColumnImplicitWidth
                            ? actualWidth
                            : lastColumnImplicitWidth
            }
        }
    }

}
