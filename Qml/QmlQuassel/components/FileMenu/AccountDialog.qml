import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../Common"

Window {
    width: 280
    height: 370
    title: qsTr("Edit Core Accounts")
    color: cppThemeHolderObject.commonWindowBkColor
    modality: Qt.ApplicationModal

    minimumWidth: 280
    minimumHeight: 360

    ColumnLayout {
        spacing: 1
        anchors.fill: parent
        anchors.margins: 10

        GroupBox {
            title: qsTr("Account Details")

            //  not fill height
            Layout.fillWidth: true
            //  fixed height
            height: 150

           GridLayout {
               anchors.fill: parent
               anchors.margins: 5

                CustomText {
                    text: qsTr("Account Names:")

                    Layout.row: 0
                    Layout.column: 0
                }
                BorderedTextInput {
                    textt: qsTr("ASAS")

                    Layout.row: 0
                    Layout.column: 1
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                }

                CustomText {
                    text: qsTr("Port")

                    Layout.row: 1
                    Layout.column: 0
                }
                BorderedTextInput {
                    textt: qsTr("ASAS")

                    Layout.row: 1
                    Layout.column: 1
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                }

                CustomText {
                    text: qsTr("Port")

                    Layout.row: 2
                    Layout.column: 0
                }
                SpinBox {
                    //  allow 5-digits
                    maximumValue: 99999

                    //  fixed size
                    width: 55
                    height: 20

                    Layout.row: 2
                    Layout.column: 1
                }

                CustomText {
                    text: qsTr("Users")

                    Layout.row: 3
                    Layout.column: 0
                }
                BorderedTextInput {
                    textt: qsTr("ASAS")

                    Layout.row: 3
                    Layout.column: 1
                    //  fill width in the entire second column
                    Layout.fillWidth: true
                }

                CustomText {
                    text: qsTr("Passwords")

                    Layout.row: 4
                    Layout.column: 0
                }
                BorderedTextInput {
                    id: textInputPassword
                    textt: qsTr("ASAS")

                    Layout.row: 4
                    Layout.column: 1
                    //  the only strechable item of this row in GridLayout
                    Layout.fillWidth: true

                    echoModee: TextInput.Password
                }
                CheckBox {
                    text: qsTr("Remember")

                    Layout.row: 4
                    Layout.column: 2
                }
           }
        }
        GroupBox {
            id: groupBox
            title: qsTr("Use Proxy")
            checkable: true

            //  not fill height
            Layout.fillWidth: true
            //  fixed height
            height: 150

            GridLayout {
                anchors.fill: parent
                anchors.margins: 5

                CustomText {
                    text: qsTr("代理类型")

                    Layout.row: 0
                    Layout.column: 0
                }
                ComboBox {

                    Layout.row: 0
                    Layout.column: 1
                    Layout.minimumWidth: 60
                    Layout.maximumWidth: 60
                    Layout.minimumHeight: 20
                    Layout.maximumHeight: 20
                }

                CustomText {
                    text: qsTr("Hostname")

                    Layout.row: 1
                    Layout.column: 0
                }
                BorderedTextInput {
                    textt: qsTr("")

                    Layout.row: 1
                    Layout.column: 1
                    Layout.fillWidth: true
                }

                CustomText {
                    text: qsTr("Port")

                    Layout.row: 2
                    Layout.column: 0
                }
                SpinBox {
                    //  the maximum allowed digit count is 5.
                    maximumValue: 99999

                    Layout.row: 2
                    Layout.column: 1
                }

                CustomText {
                    text: qsTr("User names")

                    Layout.row: 3
                    Layout.column: 0
                }
                BorderedTextInput {

                    Layout.row: 3
                    Layout.column: 1
                    Layout.fillWidth: true
                    Layout.rightMargin: 25
                }

                CustomText {
                    text: qsTr("Passwords")

                    Layout.row: 4
                    Layout.column: 0
                }
                BorderedTextInput {
                    Layout.row: 4
                    Layout.column: 1
                    Layout.fillWidth: true
                    Layout.rightMargin: 25
                }
            }
        }

        ColumnLayout {
            spacing: 0
            //  this is the only Height-Filled item of this ColumnLayout children
            //  which makes it uses all the lefted spaces.
            Layout.fillHeight: true
            Layout.fillWidth: true

            //  declare an empty `Row` to occupy the spare spaces
            Row { Layout.fillHeight: true }
            Row {
                //  note this attached property only works on Positioners, Item anchors and Views
                LayoutMirroring.enabled: true
                //  must fill width, or its size is the addition of two buttons' width
                Layout.fillWidth: true

                Button {
                    text: qsTr("Cancel")
                }
                Button {
                    text: qsTr("Ok")
                }
            }
        }
    }
}
