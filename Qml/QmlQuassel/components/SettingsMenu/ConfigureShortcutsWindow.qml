import QtQuick 2.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4


import "../Common"

import "../Common/Common.js" as Common

Window {
    id: shortcutWindow
    title: qsTr("Configure Shortcuts")
    color: cppThemeHolderObject.commonWindowBkColor

    width: 565
    height: 555

    minimumWidth: 450
    minimumHeight: 185

    //  Qt flags have strange behaviours, searched this for a lot of time. Qt.WindowContextHelpButtonHint
    //  and Qt.WindowSystemMenuHint works together.
    flags: Qt.Window
            | Qt.WindowTitleHint
            | Qt.WindowCloseButtonHint
            | Qt.WindowSystemMenuHint
            | Qt.WindowContextHelpButtonHint

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        //  Set current focus to this ColumnLayout to make `Keys` work.
        focus: true
        Keys.onPressed: {
            if (event.key === Qt.Key_Escape)
                shortcutWindow.close()
        }

        TitleHeader {
            title: qsTr("Shortcuts")
            Layout.fillWidth: true
        }

        ScrollView {
            id: view

            //  the default vertical scrollbar width measured by my self
            property int scrollbarWidth: 22

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 5

            verticalScrollBarPolicy: Qt.ScrollBarAsNeeded
            horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

            //  a inner group with margins
            ColumnLayout {
                //  NOTE:   We can use a JavaScript function to define property bindings
                //  but Qt functions may not. When the parent(ScrollView)'s height is smaller
                //  than 200, this ColumnLayout's is set to 200. This makes the ScrollView
                //  to show a vertical ScrollBar automatically.
                height: Common.max(view.height, 200)
                //  NOTE:   Use the height value to determine the scrollbar's visibility.
                width: view.width - ((view.height < 200) ? view.scrollbarWidth : 0)

                FormLayout {
                    Layout.fillWidth: true
                    //  FormLayout does not automatically calculate its size according
                    //  to its children's size.
                    height: Common.max(textInFormLayout.height, textFieldInFormLayout.height)

                    labelBuddyPairs: [
                        CustomText {
                            id: textInFormLayout
                            text: qsTr("Search")
                        },
                        TextField {
                            id: textFieldInFormLayout
                            Layout.fillWidth: true
                        }
                    ]
                }

                TreeView {
                    id: shortcutTreeView

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: 30

                    sortIndicatorVisible: true

                    TableViewColumn {
                        id: actionTreeColumn
                        title: qsTr("Actions")
                        width: 210
                    }
                    TableViewColumn {
                        title: qsTr("Shortcuts")
                        width: shortcutTreeView.width - actionTreeColumn.width - 2
                    }

                    //  todo: model

                    onClicked: {
                        editShortcutsForms.enabled = true
                    }
                }
                FormLayoutInGroupBox {
                    id: editShortcutsForms

                    Layout.fillWidth: true
                    checkable: false
                    enabled: false

                    labelBuddyPairs: [
                        RadioButton {
                            text: qsTr("Default")
                        },
                        CustomText {
                            Layout.fillWidth: true

                            text: qsTr("None")
                        },

                        RadioButton {
                            text: qsTr("Customize")
                        },
                        RowLayout {
                            Button {
                                width: 16
                                height: 16
                                iconSource: "qrc:///resources/ShortcutSettings.svg"
                            }
                            //  an empty item to fullfill the remained spaces
                            Item {
                                Layout.fillWidth: true
                            }
                        }
                    ]
                }
            }
        }
        //  todo: reuse
        RowLayout {
            id: buttonRowLayout

            //  todo: enable according to whether the default shortcut setttings were changed
            Button {
                text: qsTr("reset")
                enabled: false
            }
            Button {
                text: qsTr("Restore Defaults")
            }

            Item { Layout.fillWidth: true }

            Button {
                text: qsTr("Ok")
            }
            Button {
                text: qsTr("Cancel")
            }
            Button {
                text: qsTr("Apply")
                enabled: false
            }
        }
    }
}
