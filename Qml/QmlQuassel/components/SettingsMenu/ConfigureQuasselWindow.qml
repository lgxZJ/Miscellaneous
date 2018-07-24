import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick 2.7
import QtQuick.Window 2.2

import QtQml.Models 2.2

import "../Common"
import "./Ui"
import "./RemoteCore"

import "../Common/Common.js" as Common

//  NOTE:   The window's maximum width is automatically limited to the screen width in pixels.
Window {
    id: configureQuasselWindow
    title: qsTr("Configure Interfaces")
    //todo::
    color: cppThemeHolderObject.configureQuasselWindow.backgroundColor

    width: 820
    height: 570

    minimumWidth: 525
    minimumHeight: 520

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

        Row {
            id: firstRow
            spacing: 5

            Layout.fillWidth: true
            Layout.fillHeight: true

            //  todo: improper height
            SimpleTreeView {
                id: treeView

                width: {
                    if (firstRow.width > 660)   return maxWidth

                    var currentWidth = firstRow.width - stackView.minimumWidth
                    if (currentWidth < minWidth)    return minWidth
                    if (currentWidth > maxWidth)    return maxWidth

                    return currentWidth
                }
                height: parent.height

                model: ListModel {
                    ListElement {
                        //  NOTE:   Qt show incorrect error msgs when defining roles inside ListElement.
                        //  Use `@disable-check` directive to disable it.

                        //  @disable-check M16
                        text: qsTr("Interfaces")
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Chaat views")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Chat monitor")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Chat and nick lists")
                        //  @disable-check M16
                        isSubEntry: true
                    }

                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Custom chat list")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Input components")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Topic components")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Outstanding display")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Notifications")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Storage fetch")
                        //  @disable-check M16
                        isSubEntry: true
                    }

                    ListElement {
                        //  @disable-check M16
                        text: qsTr("IRC")
                        //  @disable-check M16
                        isDisabled: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Identification")
                        //  @disable-check M16
                        isSubEntry: true
                        //  @disable-check M16
                        isDisabled: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Networks")
                        //  @disable-check M16
                        isSubEntry: true
                        //  @disable-check M16
                        isDisabled: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Alias")
                        //  @disable-check M16
                        isSubEntry: true
                        //  @disable-check M16
                        isDisabled: true
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Ignore lists")
                        //  @disable-check M16
                        isSubEntry: true
                        //  @disable-check M16
                        isDisabled: true
                    }

                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Remote cores")
                    }
                    ListElement {
                        //  @disable-check M16
                        text: qsTr("Connections")
                        //  @disable-check M16
                        isSubEntry: true
                    }
                }

                onItemClicked: { stackView.currentIndex = index }
            }

            StackLayout {
                id: stackView

                property int minimumWidth: 460

                width: {
                    var currentWidth = firstRow.width - firstRow.spacing - treeView.maxWidth
                    return firstRow.width < 660
                        ? minimumWidth
                        : currentWidth
                }
                height: parent.height

                currentIndex: 1

                //  we does not specify width and height here since items inside StackLayout are
                //  automatically fulfilled.
                UiStackWindow {}
                ChatView {}
                ChatMonitor {}
                ChatAndNickList {}
                CustomChatList {}
                InputComponent {}
                TopicComponent {}
                HighlightRepresent {}
                Notifications {}

                ColumnLayout {

                    TitleHeader {
                        title: qsTr("快捷键")
                        Layout.fillWidth: true
                    }

                    ConnectToCoreContent {
                        parentObject: configureQuasselWindow

                        Layout.margins: 10
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                    }
                }
                Connect {}
            }
        }

        RowLayout {
            Layout.fillWidth: true

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
