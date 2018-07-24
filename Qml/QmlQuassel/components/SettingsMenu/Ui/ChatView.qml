import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4


import "../../Common"

import "../../Common/Common.js" as Common

ColumnLayout {
    id: chatView

    property int colorSpacing: 30
    property int groupInnerMargin: 5

    width: 400
    height: 400

    TitleHeader {
        title: qsTr("Chat view")
        Layout.fillWidth: true
    }
    ColumnLayout {
        id: columnLayout

        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.leftMargin: 5

        RowLayout {
            id: firstRow

            property int childPreferredWidth: 50

            Layout.fillWidth: true

            CustomText {
                id: timeFormatText
                text: qsTr("Time format:")
            }

            //  WARNING:    Make the following two item's Layout.preferredWidth set to
            //  a same value to make them extend their size equally. Notice that the
            //  Layout.fillWidth property need to be set to true also.
            TextField {
                //  todo; tool tip
                Layout.fillWidth: true
                Layout.preferredWidth: firstRow.childPreferredWidth
                text: qsTr("[hh:mm:ss]")

                ToolTip {
                    visible: parent.hovered
                    delay: 1000
                    text: qsTr("Usage examples:\n"
                                + " dd.MM.yyy\t 21.05.2001\n"
                                + " ddd MMMM d yy\t Tue May 21 01\n"
                                + " hh:mm:ss:zzz\t 14:!3:09:042\n"
                                + " h:m:s ap\t 2:13:9 pm")
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.preferredWidth: firstRow.childPreferredWidth
            }
        }

        RowLayout {
            CheckBox {
                id: customChatFont
                text: qsTr("Custom chat window fonts:")
                checked: false
            }
            TextField {
                Layout.fillWidth: true

                readOnly: true
                enabled: customChatFont.checked
                text: qsTr("Monospace 9pt")
            }
            Button {
                text: qsTr("Select...")
                enabled: customChatFont.checked
            }
        }

        CheckBox {
            Layout.fillWidth: true
            text: qsTr("Allow colorful text(text color of mIRC)")
        }
        CheckBox {
            Layout.fillWidth: true
            text: qsTr("Show preview window when mouse hover urls")
        }
        CheckBox {
            Layout.fillWidth: true
            text: qsTr("Auto set label when switch chats")
        }
        CheckBox {
            Layout.fillWidth: true
            text: qsTr("set marker line automatically when Quassel loses focua")
        }
        RowLayout {
            Layout.fillWidth: true

            CustomText { text: qsTr("Web Search URL:") }
            TextField {
                Layout.fillWidth: true
                text: qsTr("https://www.google.com/search?q=%s")

                ToolTip {
                    visible: parent.hovered
                    delay: 1000
                    text: qsTr("The URL to open with the selected text as the \n"
                                + "parameter. Place %s where selected text should \n"
                                + "go.\n\n"
                                + "Eg:\n\n"
                                + "https://www.google.com/search?q=%s\n"
                                + "https://duckkduckgo.com/?q=%s")
                }
            }
        }
        GroupBox {
            title: qsTr("Custom colors")
            checked: false
            checkable: true

            Layout.fillWidth: true

            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right


                GridLayout {
                    Layout.fillWidth: true
                    Layout.margins: chatView.groupInnerMargin

                    columns: 2

                    AutoGrayText { text: qsTr("Actions:");  }
                    ColorBlock { innerColor: "blue"}

                    AutoGrayText { text: qsTr("Channel msg:") }
                    ColorBlock { innerColor: "black" }

                    AutoGrayText { text: qsTr("Commands:") }
                    ColorBlock { innerColor: Common.rgb(150, 0, 150) }

                    AutoGrayText { text: qsTr("Server msgs:") }
                    ColorBlock { innerColor: Common.rgb(145, 100, 9) }

                    AutoGrayText { text: qsTr("Errro msgs:") }
                    ColorBlock { innerColor: Common.rgb(145, 100, 9) }
                }
                Item { Layout.fillWidth: true }
                GridLayout {
                    Layout.fillWidth: true

                    columns: 2

                    AutoGrayText { text: qsTr("Time format:")}
                    ColorBlock { innerColor: Common.rgb(112, 124, 112) }

                    AutoGrayText { text: qsTr("Hight foreground colors") }
                    ColorBlock { innerColor: "black" }

                    AutoGrayText { text: qsTr("Hight background colors") }
                    ColorBlock { innerColor: Common.rgb(255, 128, 0) }

                    AutoGrayText { text: qsTr("Labels") }
                    ColorBlock { innerColor: "red" }

                    AutoGrayText { text: qsTr("Backgrounds") }
                    ColorBlock { innerColor: "white" }
                }
            }
        }

        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Use colors from senders")
            checkable: true
            checked: false

            ColumnLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                GridLayout {
                    //  WARNING:    To arrange Layout's children without streching them, we have
                    //  to assign it a size first. However, the `Layout.fillWidth` property's
                    //  behavior seems do not affect it. To make things work, we can either set the
                    //  `Layout.preferredWidth` property of GridLayout or manually set the `columnSpacing`
                    //  of GridLayout. However, they all cause a Binding Loop. Therefore i have to
                    //  manually set `Layout.fillWidth` property for an empty Item to mimic the column
                    //  space.
                    Layout.fillWidth: true

                    columns: 8

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5
                        Layout.leftMargin: 5

                        ColorBlock { innerColor: Common.rgb(233, 13, 127); }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5

                        ColorBlock { innerColor: Common.rgb(142, 85, 233) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5

                        ColorBlock { innerColor: Common.rgb(179, 14, 14) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5

                        ColorBlock { innerColor: Common.rgb(23, 179, 57) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5

                        ColorBlock { innerColor: Common.rgb(88, 175, 179) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5

                        ColorBlock { innerColor: Common.rgb(157, 84, 179) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5

                        ColorBlock { innerColor: Common.rgb(179, 151, 117) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.topMargin: 5
                        Layout.rightMargin: 5

                        ColorBlock { innerColor: Common.rgb(49, 118, 179) }
                        Item { Layout.fillWidth: true }
                    }


                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5
                        Layout.leftMargin: 5

                        ColorBlock { innerColor: Common.rgb(233, 13, 127) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5

                        ColorBlock { innerColor: Common.rgb(142, 85, 233) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5

                        ColorBlock { innerColor: Common.rgb(179, 14, 14) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5

                        ColorBlock { innerColor: Common.rgb(23, 179, 57) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5

                        ColorBlock { innerColor: Common.rgb(88, 175, 179) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5

                        ColorBlock { innerColor: Common.rgb(157, 84, 179) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5

                        ColorBlock { innerColor: Common.rgb(179, 151, 117) }
                        Item { Layout.fillWidth: true }
                    }
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.bottomMargin: 5
                        Layout.rightMargin: 5

                        ColorBlock { innerColor: Common.rgb(49, 118, 179) }
                        Item { Layout.fillWidth: true }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true

                    CustomText { text: qsTr("My own msgs:") }
                    ColorBlock { innerColor: "black" }
                }
            }
        }
        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
