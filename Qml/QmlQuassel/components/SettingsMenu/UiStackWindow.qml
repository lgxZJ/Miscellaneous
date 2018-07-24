import QtQuick.Layouts 1.3
import QtQuick 2.7
import QtQuick.Controls 1.4

import "../Common"

//  NOTE:   No width and height are set explicitly, this item's size are
//  specified by callers.
Item {
    id: stack

    property int indent: 30
    property int spacing: 5

    //  casual sizes
    width: 200
    height: 200


    ColumnLayout {
        anchors.fill: parent

        TitleHeader {
            title: qsTr("Shortcuts")
            Layout.fillWidth: true
        }

        ColumnLayout {
            spacing: 10
            Layout.leftMargin: 5
            Layout.rightMargin: 5

            //  todo: improve the size deciding strategy
            FormLayout {
                height: clientStyleCombobox.height * 2

                labelBuddyPairs: [
                    CustomText {
                        text: qsTr("Client style")
                    },
                    ComboBox {
                        id: clientStyleCombobox
                    },

                    CustomText { text: qsTr("Languages") },
                    ComboBox {
                    }
                ]
            }


            CheckBox {
                id: customeStylesheetCheckbox

                Layout.fillWidth: true
                text: qsTr("Use customized style sheet")
            }
            RowLayout {
                enabled: customeStylesheetCheckbox.checked

                CustomText {
                    Layout.leftMargin: stack.indent
                    text: qsTr("Path")
                }
                TextField {
                    Layout.fillWidth: true
                }
                Button {
                    text: qsTr("...")
                }
            }

            GroupBox {
                title: qsTr("Show system tray icon")
                flat: true
                checkable: true

                Column {
                    topPadding: stack.spacing
                    //  GroupBox has a frame border width, substract it to make the total indent the same as other items
                    leftPadding: stack.indent - 7
                    spacing: stack.spacing

                    CheckBox {
                        text: qsTr("Single click hide to tray")
                    }
                    CheckBox {
                        text: qsTr("Startup animations")
                        checked: true
                    }
                }
            }

            GroupBox {
                title: qsTr("Msg forwards")

                Layout.fillWidth: true

                GridLayout {
                    columns: 4
                    columnSpacing: 20
                    rowSpacing: 5

                    CustomText { }
                    CustomText { text: qsTr("Default objects") }
                    CustomText { text: qsTr("Statss window") }
                    CustomText { text: qsTr("Current chat") }

                    //  todo: get init value via init value manager
                    CustomText { text: qsTr("User notifications") }
                    CheckBox {  }
                    CheckBox {  }
                    CheckBox {  }

                    CustomText { text: qsTr("Server notifications") }
                    CheckBox {  }
                    CheckBox {  }
                    CheckBox {  }

                    CustomText { text: qsTr("Errors") }
                    CheckBox {  }
                    CheckBox {  }
                    CheckBox {  }
                }

            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
