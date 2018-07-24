import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import "../../Common"

import "../../Common/Common.js" as Common

Item {
    id: root
    width: 200
    height: 200

    property int groupBoxTopMargin: 5
    property int horizontalMargin: 5

    ColumnLayout {
        anchors.fill: parent

        TitleHeader {
            Layout.fillWidth: true

            title: qsTr("Chat and nick lists")
        }
        //  todo: same problem happens again, maybe my fault? (GroupBox)
        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: root.horizontalMargin
            Layout.rightMargin: root.horizontalMargin

            //  todo: reuse from other places?
            RowLayout {
                Layout.fillWidth: true

                CheckBox {
                    id: customeFontCheckBox
                    text: qsTr("Custom fonts")
                }
                TextField {
                    Layout.fillWidth: true

                    enabled: customeFontCheckBox.checked
                    text: qsTr("SimSun 9pt")
                }
                Button {
                    enabled: customeFontCheckBox.checked
                    text: qsTr("Select...")
                }
            }

            CheckBox {
                Layout.fillWidth: true

                text: qsTr("Show icons")
            }
            GroupBox {
                Layout.fillWidth: true

                title: qsTr("Chat lists")

                ColumnLayout {
                    anchors.fill: parent
                    anchors.topMargin: root.groupBoxTopMargin

                    CheckBox {
                        Layout.fillWidth: true

                        text: qsTr("Shoew tip on tool bar")
                    }
                    CheckBox {
                        Layout.fillWidth: true

                        text: qsTr("Use mouse wheel to change selected chat")
                    }
                    GroupBox {
                        Layout.fillWidth: true


                        title: qsTr("Use custom color")
                        checkable: true

                        height: 400

                        Row {
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom

                            id: row
                            spacing: 40

                            GridLayout {
                                columns: 2
                                height: 165
                                width: 100

                                CustomText {
                                    text: qsTr("Standard")
                                }
                                ColorBlock {
                                    innerColor: "black"
                                }

                                CustomText {
                                    text: qsTr("Non active:")
                                }
                                ColorBlock {
                                    innerColor: Common.rgb(160, 160, 164)
                                }

                                CustomText {
                                    text: qsTr("Unread:")
                                }
                                ColorBlock {
                                    innerColor: Common.rgb(0, 0, 245)
                                }

                                CustomText {
                                    text: qsTr("Hightlight：")
                                }
                                ColorBlock {
                                    innerColor: Common.rgb(255, 128, 0)
                                }

                                CustomText {
                                    text: qsTr("Other activities:")
                                }
                                ColorBlock {
                                    innerColor: Common.rgb(0, 128, 0)
                                }
                            }
                            TableView {
                                anchors.top: row.top
                                anchors.bottom: row.bottom

                                width: 235
                                height: 170
                            }
                        }

                    }
                }
            }
            GroupBox {
                Layout.fillWidth: true

                title: qsTr("Custom nick lists")
                checkable: true

                Row {
                    spacing: 90
                    anchors.fill: parent
                    anchors.topMargin: root.groupBoxTopMargin

                    ColorBlockAfterText {
                        text: qsTr("Online:")
                        color: "black"
                    }
                    ColorBlockAfterText {
                        text: qsTr("Offline:")
                        color: "black"
                    }
                }
            }
            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }

}
