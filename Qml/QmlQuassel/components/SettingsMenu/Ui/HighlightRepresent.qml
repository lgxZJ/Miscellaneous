import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick 2.7

import "../../Common"


ColumnLayout {

    TitleHeader {
        Layout.fillWidth: true
        title: qsTr("Highlight")
    }
    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 5

        GroupBox {
            Layout.fillWidth: true
            Layout.fillHeight: true

            checkable: false
            title: qsTr("Custom highlight display")

            ColumnLayout {
                anchors.fill: parent

                TableView {
                    id: highlightTableView

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TableViewColumn {
                        title: qsTr("Highlight")
                        //  The caller ensures that this width won't be negative values
                        width: highlightTableView.width - expColumn.width
                                                        - csColumn.width
                                                        - enableColumn.width
                                                        - channelColumn.width - 2
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        id: expColumn

                        title: qsTr("Match patterns")
                        width: 65
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        id: csColumn

                        title: qsTr("CS")
                        width: 25
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        id: enableColumn

                        title: qsTr("Enable")
                        width: 35
                        horizontalAlignment: Text.AlignHCenter
                    }
                    TableViewColumn {
                        id: channelColumn

                        title: qsTr("Channels")
                        width: 35
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Row {
                    Button { text: qsTr("Add") }
                    Button { text: qsTr("Delete") }
                }
            }
        }
        GroupBox {
            Layout.fillWidth: true

            checkable: false
            title: qsTr("Highlight nicks")

            Column {
                RadioButton { text: qsTr("All nicks corresponding to ids") }
                RadioButton { text: qsTr("Current nick") }
                RadioButton { text: qsTr("None") }
                CheckBox { text: qsTr("Case sensitive") }
            }
        }
    }
}
