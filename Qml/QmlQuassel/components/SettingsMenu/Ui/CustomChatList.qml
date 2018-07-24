import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick 2.7

import "../../Common"


ColumnLayout {
    TitleHeader {
        Layout.fillWidth: true

        title: qsTr("Custom chat lists")
    }
    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.margins: 5

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            TableView {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            Button {
                Layout.fillWidth: true

                text: qsTr("Rename...")
                iconSource: "qrc:///resources/NetworkRename.png"
            }
            Button {
                Layout.fillWidth: true

                text: qsTr("Add...")
                iconSource: "qrc:///resources/AddAccount.svg"
            }
            Button {
                Layout.fillWidth: true

                text: qsTr("Delete...")
                iconSource: "qrc:///resources/DeleteAccount.png"
            }
        }
        GroupBox {
            title: qsTr("Chat list settings")
            checkable: false
            checked: false

            //  seems `Layout.preferredWidth` not work
            implicitWidth: 140
            Layout.fillHeight: true

            Column {
                anchors.fill: parent
                anchors.margins: 5

                spacing: 5

                RowLayout {
                    width: parent.width
                    spacing: 10

                    CustomText { text: qsTr("Networks") }
                    ComboBox {
                        Layout.fillWidth: true

                        model: [ qsTr("All") ]
                    }
                }
                CheckBox { text: qsTr("Show status window") }
                CheckBox { text: qsTr("Show channels") }
                CheckBox { text: qsTr("Show search") }
                CheckBox { text: qsTr("Hide inactive chats") }
                CheckBox { text: qsTr("Auto add new chats") }
                CheckBox { text: qsTr("Ordered alphabetically") }
                CustomText { text: qsTr("Minimum activities") }
                ComboBox {
                    width: parent.width
                    model: [ qsTr("No activities") ]
                }
            }
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            CustomText {
                text: qsTr("Preview")
            }
            TableView {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
