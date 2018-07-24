import QtQuick 2.5
import QtQuick 2.7
import QtQuick.Window 2.2

import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Controls 1.4

import QtQuick.Controls.Styles 1.4

import "./components/Common"

import "./components/FileMenu"
import "./components/HelpMenu"
import "./components/SettingsMenu"
import "./components/ViewMenu"
import "./components/MenuStyles"
import "./components/ThemeMenu"
import "./components/LanguagesMenu"

import "./components/Common/Common.js" as Common

////////////////////////////////////////////////////////////////////////////////
//  There are some strange UI parameters scatterred in this project, this is
//  because it is a fake software of `Quassel`. I choose to fake as many as
//  possible which means it will be really similiar to `Quassel`.
////////////////////////////////////////////////////////////////////////////////
//  todo: write theme manager containing colors and icon
ApplicationWindow {
    id: appWindow
    visible: true
    width: minimumWidth
    height: minimumHeight
    title: qsTr("Quassel IRC")
    color: cppThemeHolderObject.commonWindowBkColor

    minimumWidth: 470
    minimumHeight: 310

    menuBar: MenuBar {
        //style: MenuBarStyle {}

        FileMenu {}
        ViewMenu {}
        SettingsMenu {}
        HelpMenu {}
        ThemeMenu {}
        LanguagesMenu {}
    }

    /////////////////////////////////////////////////////////////////////
    //  WARNING:    Cannot use ToolBar here, strange behaviours appears.
    /////////////////////////////////////////////////////////////////////
    toolBar: Rectangle {
        implicitWidth: 500
        implicitHeight: 34

        Rectangle {
            anchors.fill: parent
            border.width: 1
            border.color: cppThemeHolderObject.toolBar.firstTopBorderColor
            color: cppThemeHolderObject.commonWindowBkColor

            RowLayout {
                anchors.fill: parent

                //  NOTE:   Due to tech limitations we cannot show tooltip
                ToolButton {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 30

                    enabled: false
                    iconSource: "qrc:///resources/ConnectToCore.png"
                    tooltip: qsTr("Connect to IRC")
                }
                ToolButton {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 30

                    enabled: false
                    iconSource: "qrc:///resources/DisconnectFromCore.png"
                    tooltip: qsTr("Disconnect from IRC")
                }
                ToolButton {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 30

                    iconSource: "qrc:///resources/ActivePoundSign.svg"
                    tooltip: qsTr("Join a channel")
                    //  NOTE:   Due to tech limitations we cannot assign a different icon
                    //  to a child window which has a separate TaskBar button.
                    onClicked: Common.showDialog("qrc:///JoinChannel.qml", null)
                }
                ToolButton {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 30

                    enabled: false
                    iconSource: "qrc:///resources/NonActivePoundSign.svg"
                    tooltip: qsTr("Disconnect from current channel")
                }
                Item {
                    Layout.fillWidth: true
                }
            }
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right

                height: 1
                y: parent.height - 1
                color: cppThemeHolderObject.toolBar.firstBottomBorderColor
            }
            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right

                height: 1
                y: parent.height - 2
                color: cppThemeHolderObject.toolBar.secondBottomBorderColor
            }
        }
    }

    statusBar: StatusBar {
        style: StatusBarStyle {
                  background: Rectangle { color: cppThemeHolderObject.statusBarBkColor }
        }

        RowLayout {
            anchors.fill: parent
            Label { text: qsTr("Not ready") }
        }
    }

    Splash {
        id: splashScreen
        anchors.fill: parent
        z: 1
    }

    Component.onCompleted: {
        toolBar.visible = false
        statusBar.visible = false
    }

    Timer {
        interval: 3000
        repeat: false
        running: true
        onTriggered: {
            splashScreen.visible = false
            toolBar.visible = true
            statusBar.visible = true
            //splashScreen.z = -1
        }
    }

    SplitView {
        anchors.fill: parent

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            StackLayout {
                Layout.fillWidth: true
                Layout.fillHeight: false

                id: stackLayout
                currentIndex: 0

                GridLayout {
                    Layout.fillWidth: true

                    implicitHeight: 20
                    columns: 2

                    TextField {
                        Layout.fillWidth: true
                        enabled: false

                    }
                    Button {
                        iconSource: "qrc:///resources/InputMsg.svg"
                        onClicked: {
                            stackLayout.currentIndex = 1
                            editableTedtField.focus = true
                        }
                    }
                }
                TextField {
                    id: editableTedtField

                    focus: true
                    height: 20

                    onFocusChanged: {
                        if (!focus)
                            stackLayout.currentIndex = 0
                    }
                }
            }
            TextArea {
                Layout.fillWidth: true

                implicitHeight: 70
                textColor: cppThemeHolderObject.commonTextBkColor
                readOnly: true
            }
            Image {
                Layout.fillHeight: true
                Layout.fillWidth: true

                fillMode: Image.Pad
                source: "qrc:///resources/brand.png"
            }
            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: false

                Button {
                    id: expandIndicator

                    iconSource: "qrc:///resources/RightArrow.svg"
                    onCheckedChanged: {
                        if (checked)    iconSource = "qrc:///resources/LeftArrow.svg"
                        else            iconSource = "qrc:///resources/RightArrow.svg"
                    }

                    checkable: true
                    implicitWidth: 16
                    implicitHeight: 24
                }
                RowLayout {
                    id: toolsRowLayout
                    property int buttonWidth: 24
                    property int buttonHeight: 24

                    Layout.fillWidth: true
                    Layout.fillHeight: false
                    Layout.preferredHeight: buttonHeight - 1

                    RowLayout {
                        Layout.fillHeight: false
                        Layout.fillWidth: true
                        Layout.preferredHeight: parent.Layout.preferredHeight
                        visible: expandIndicator.checked

                        ToolButton { Layout.fillHeight: true; Layout.preferredWidth: toolsRowLayout.buttonWidth; checkable: true; iconSource: "qrc:///resources/Bold.svg" }
                        ToolButton { Layout.fillHeight: true; Layout.preferredWidth: toolsRowLayout.buttonWidth; checkable: true; iconSource: "qrc:///resources/Italic.svg" }
                        ToolButton { Layout.fillHeight: true; Layout.preferredWidth: toolsRowLayout.buttonWidth; checkable: true; iconSource: "qrc:///resources/Underline.svg" }
                        ComboBox { Layout.margins: 1; Layout.fillHeight: true; implicitWidth: 35; }
                        ComboBox { Layout.margins: 1; Layout.fillHeight: true; implicitWidth: 35 }
                    }
                    TextField {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.margins: 1
                    }
                }
            }
        }
        ColumnLayout {
            Layout.minimumWidth: 80
            Layout.maximumWidth: 160

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 15
                Layout.maximumHeight: 15
                Layout.minimumHeight: 15
                Layout.margins: 1
                color: cppThemeHolderObject.customedTitleBkColor
                border.width: 1
                border.color: cppThemeHolderObject.customedTitleBorderColor

                onColorChanged: console.log(color)

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 1

                    CustomText { text: qsTr("Nicks") }
                    Item {
                        Layout.fillWidth: true
                    }
                    ToolButton {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 15
                        iconSource: "qrc:///resources/CascadeButton.svg"
                    }
                    ToolButton {
                        Layout.fillHeight: true
                        Layout.preferredWidth: 15
                        iconSource: "qrc:///resources/CloseButton.svg"
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
