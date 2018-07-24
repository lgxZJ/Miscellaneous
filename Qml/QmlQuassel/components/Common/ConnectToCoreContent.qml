import QtQuick.Controls 1.4
import QtQuick 2.7
import QtQuick.Layouts 1.3

import QtQuick.Controls.Styles 1.4

import "../Common/Common.js" as Common

//  Because this item is used more than one place, i extracted it to make it reusable.
ColumnLayout {
    property QtObject parentObject: null

    ColumnLayout {
        id: outerColumnLayout
        Layout.fillHeight: true
        Layout.fillWidth: true

        GroupBox {
            title: qsTr("Core Accounts")

            //  Column respects children's size, when is fills to parent, it will
            //  not change children's size, manually do this here
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.minimumWidth: 90
            Layout.minimumHeight: 84

            Layout.maximumHeight: 230

            Layout.preferredWidth: 380
            Layout.preferredHeight: 230


            RowLayout {
                id: rowInGroupBox
                anchors.fill: parent

                BackgroundListView {
                    id: accountListView
                    width: rowInGroupBox.width - buttonColumn.width

                    Layout.fillWidth: true  //  make ListView expand not buttons when resizing
                    Layout.fillHeight: true

                    bkColor: "#F0F0F0"
                    borderColor: "#828790"
                }


                Column {
                    id: buttonColumn
                    Layout.alignment: Qt.AlignTop
                    spacing: 8

                    Button {id:aaa
                        text: qsTr("Edit")
                        iconSource: "qrc:///resources/EditAccount.svg"

                        enabled: Common.backgroundListNotEmpty(accountListView)

                        onClicked: Common.showDialog("qrc:///components/FileMenu/AccountDialog.qml", parentObject)
                    }
                    Button {
                        text: qsTr("Add")
                        iconSource: "qrc:///resources/AddAccount.svg"
                    }
                    Button {
                        text: qsTr("Delete")
                        iconSource: "qrc:///resources/DeleteAccount.png"

                        enabled: Common.backgroundListNotEmpty(accountListView)
                    }
                }
            }
        }

        Column {
            property int spacingInPixel: 7

            id: autoConnectColumn
            spacing: spacingInPixel

            CheckBox {
                id: autoConnectCheckBox
                text: qsTr("Automatic connect on startup")
            }

            ExclusiveGroup { id: exclusiveGroupConnect }
            Row {
                leftPadding: 20

                Column {
                    spacing: autoConnectColumn.spacingInPixel
                    bottomPadding: 7

                    RadioButton {
                        text: qsTr("Connect using the last used account")
                        enabled: autoConnectCheckBox.checked
                        checked: true   //  initially checked
                        exclusiveGroup: exclusiveGroupConnect
                    }
                    Row {
                        enabled: autoConnectCheckBox.checked
                        spacing: 5

                        RadioButton {
                            id: radioButtonAlwaysConnect
                            text: qsTr("Always connect to")
                            exclusiveGroup: exclusiveGroupConnect
                        }
                        ComboBox {
                            id: accountComboBox
                            enabled: radioButtonAlwaysConnect.checked

                            //  TODO:
                            //model: accountListView.model
                        }
                    }
                }
            }
        }
    }

    Row {
        Layout.fillWidth: true
        LayoutMirroring.enabled: true

        spacing: 5

        Button {
            text: qsTr("Cancel")
        }
        Button {
            text: qsTr("Ok")
        }
    }
}
