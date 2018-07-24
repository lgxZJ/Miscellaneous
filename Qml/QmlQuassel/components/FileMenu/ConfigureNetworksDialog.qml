import QtQuick.Window 2.2
import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQml.Models 2.2
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0

import "../Common"

import "../Common/Common.js" as Common

Window {
    id: window
    title: qsTr("Configure Networks")

    modality: Qt.ApplicationModal
    color: cppThemeHolderObject.commonWindowBkColor
    minimumWidth: 435
    minimumHeight: 148

    width: 563
    height: 585

    //  Qt flags have strange behaviours, searched this for a lot of time. Qt.WindowContextHelpButtonHint
    //  and Qt.WindowSystemMenuHint works together.
    flags: Qt.Window
            | Qt.WindowTitleHint
            | Qt.WindowCloseButtonHint
            | Qt.WindowSystemMenuHint
            | Qt.WindowContextHelpButtonHint

    ColumnLayout {
        id: windowLayout

        property int horizontalPadding: 10

        anchors.fill: parent

        TitleHeader {
            id: titleHeader
            title: qsTr("Networks")

            Layout.topMargin: 15
            Layout.leftMargin: parent.horizontalPadding
            Layout.rightMargin: parent.horizontalPadding
            Layout.fillWidth: true
        }

        ScrollView {
            id: scrollView
            Layout.leftMargin: parent.horizontalPadding
            Layout.rightMargin: parent.horizontalPadding
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                //  WARNING:    the only item inside ScrollView will implicitly
                //  determine the `Content Frame`'s size. It will be implicitly
                //  anchored to fill the parent ScrollView. Here we cannot use
                //  `anchors.fill` attached property, it may bacause of ScrollView
                //  processing it internally.
                //
                //  Maybe a Qt Bug?
                //
                //  So we have to use the `width` and `height` property to mimic
                //  the Layout behaviour. It works because the direct items inside
                //  `windowLayout` is correctly layouted, thus we can use other items'
                //  height to calculate the right height of the ScrollView (cannot use
                //  the parent's `anchors` property also ).
                width: windowLayout.width - 2 * windowLayout.horizontalPadding
                height:  {
                    //  specify a minimum height which makes the ScrollView scrolls
                    //  automatically when meets this minimum height.
                    var height = windowLayout.height /*- titleText.height
                                                     - .height*/
                    - titleHeader.height
                                                     - bottomButtonsLayout.height
                                                     - 15 * 2 - windowLayout.spacing * 3;
                    return height < 420 ? 420 : height
                }

                RowLayout {
                    Layout.minimumHeight: 84
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    BackgroundListView {
                        id: networkList

                        bkColor: "#F0F0F0"
                        borderColor: "#828790"

                        Layout.preferredWidth: 290
                        Layout.preferredHeight: 80

                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    Column {
                        //  fill height to make it horizontally aligned to the list view wrapper.
                        Layout.fillHeight: true

                        spacing: 5

                        Button {
                            text: qsTr("Rename")
                            iconSource: "qrc:///resources/NetworkRename.png"

                            enabled: Common.backgroundListNotEmpty(networkList)
                        }
                        Button {
                            text: qsTr("Add")
                            iconSource: "qrc:///resources/AddAccount.svg"

                            enabled: Common.backgroundListNotEmpty(networkList)
                        }
                        Button {
                            text: qsTr("Delete")
                            iconSource: "qrc:///resources/DeleteAccount.png"

                            enabled: Common.backgroundListNotEmpty(networkList)
                        }
                    }
                }
                GroupBox {
                    title: qsTr("Network Details")

                    //  fixed height
                    height: 317

                    Layout.leftMargin: parent.horizontalPadding
                    Layout.rightMargin: parent.horizontalPadding
                    Layout.fillWidth: true

                    ColumnLayout {
                        anchors.fill: parent

                        Row {
                            Layout.fillWidth: true

                            spacing: 10

                            CustomText {
                                id: idText
                                text: qsTr("Identification")

                                height: 20
                                verticalAlignment: Text.AlignVCenter
                            }

                            ComboBox {
                                id: idComboBox

                                width: (parent.width - idText.width - settingsButton.width) / 2
                                height: 20

                                model: ListModel {
                                    ListElement {
                                        //  @disable-check M16
                                        text: qsTr("AAAAA")
                                    }
                                }
                            }

                            Button {
                                id: settingsButton
                                iconSource: "qrc:///resources/NetworkSettings.svg"

                                height: 20
                            }
                        }
                        //  todo: reuse
                        TabView {
                            id: tabView
                            property real tabMargins: 10

                            currentIndex: 0

                            Layout.minimumWidth: 375
                            Layout.minimumHeight: 256
                            Layout.fillWidth: true

                            style: CustomTabViewStyle {}

                            Tab {
                                title: qsTr("Servers")

                                //  cannot use `parent.tabMargins` bacause Tabs are not
                                //  the direct children of TabViews
                                anchors.margins: tabView.tabMargins

                                RowLayout {
                                    id: rowInGroupBox
                                    anchors.fill: parent

                                    BackgroundListView {
                                        id: accountListView

                                        bkColor: "#F0F0F0"
                                        borderColor: "#828790"

                                        //  todo: this number needs to be decided later
                                        width: rowInGroupBox.width - buttonColumn.width

                                        Layout.fillWidth: true  //  make ListView expand not buttons when resizing
                                        Layout.fillHeight: true                                        
                                    }

                                    Column {
                                        id: buttonColumn
                                        Layout.alignment: Qt.AlignTop
                                        spacing: 8

                                        Button {
                                            text: qsTr("Edit")
                                            iconSource: "qrc:///resources/EditAccount.svg"

                                            enabled: accountListView.listViewItemCount !== 0

                                            onClicked: Common.showDialog("qrc:///components/FileMenu/AccountDialog.qml", connectToCoreDialog)
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
                                        Row {
                                            leftPadding: 6
                                            rightPadding: 6
                                            spacing: 5

                                            Button {
                                                iconSource: "qrc:///resources/ArrowUp.png"
                                                enabled: Common.backgroundListNotEmpty(accountListView) && !Common.currentIsFirstInBackgroundList(accountListView)
                                            }
                                            Button {
                                                iconSource: "qrc:///resources/ArrowDown.png"
                                                enabled: Common.backgroundListNotEmpty(accountListView) && !Common.currentIsLastInBackgroundList(accountListView)
                                            }
                                        }
                                    }
                                }

                            }
                            Tab {
                                title: qsTr("Comamnds")
                                anchors.margins: tabView.tabMargins


                                ColumnLayout {
                                    anchors.fill: parent

                                    CustomText {
                                        text: qsTr("Commands to execute after connected:")
                                    }
                                    Rectangle {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true

                                        border.width: 1
                                        border.color: "#7A7A7A"

                                        ScrollView {
                                            anchors.fill: parent

                                            TextEdit {
                                                id: textEdit

                                                focus: true
                                                //  a small padding to make TextEdit looks better
                                                textMargin: 3
                                            }
                                        }

                                    }
                                }

                            }
                            Tab {
                                title: qsTr("Connect")
                                anchors.margins: tabView.tabMargins

                                ColumnLayout {
                                    GroupBox {
                                        title: qsTr("Auto reconnect")

                                        checkable: true
                                        Layout.fillWidth: true

                                        ColumnLayout {
                                            anchors.left: parent.left
                                            anchors.right: parent.right

                                            Row {
                                                spacing: 5

                                                CustomText {
                                                    text: qsTr("Wait")

                                                    verticalAlignment: Text.AlignVCenter

                                                    height: secondSpin.height
                                                }
                                                SpinBox {
                                                    id: secondSpin

                                                    value: 60
                                                    minimumValue: 0
                                                    maximumValue: 3599

                                                    //  an space to make it looks better
                                                    suffix: qsTr(" Seconds")
                                                }
                                                CustomText {
                                                    text: qsTr("Between reconnect")

                                                    verticalAlignment: Text.AlignVCenter

                                                    height: secondSpin.height
                                                }
                                            }
                                            Row {
                                                spacing: 5

                                                CustomText {
                                                    text: qsTr("Reconnect times:")

                                                    verticalAlignment: Text.AlignVCenter

                                                    height: retrySpin.height
                                                }
                                                SpinBox {
                                                    id: retrySpin
                                                    enabled: !unlimitedRetryCheckBox.checked

                                                    minimumValue: 0
                                                    maximumValue: 999
                                                    value: 20

                                                    suffix: qsTr(" ")
                                                }
                                                CheckBox {
                                                    id: unlimitedRetryCheckBox
                                                    text: qsTr("No limit")
                                                    checked: false

                                                    height: retrySpin.height
                                                }
                                            }
                                            CheckBox {
                                                text: qsTr("Join all channels after reconnect")
                                                checked: false
                                            }
                                        }
                                    }
                                    //  an empty item to full fill the remained space
                                    Item {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                    }
                                }
                            }
                            Tab {
                                title: qsTr("Auto recognize")
                                anchors.margins: tabView.tabMargins

                                Column {
                                    anchors.fill: parent
                                    spacing: 5

                                    FormLayoutInGroupBox {
                                        //  only fill width to parent remaining the height unscaled
                                        anchors.left: parent.left
                                        anchors.right: parent.right

                                        title: qsTr("Auto recognize")
                                        checkable: true

                                        labelBuddyPairs: [
                                            CustomText { text: qsTr("Services:") },
                                            TextField {
                                                placeholderText: qsTr("Nick services")
                                                Layout.fillWidth: true
                                            },

                                            CustomText { text: qsTr("Passwords") },
                                            TextField {
                                                Layout.fillWidth: true
                                                echoMode: TextInput.Password
                                            }
                                        ]
                                    }

                                    FormLayoutInGroupBox {
                                        //  only fill width to parent remaining the height unscaled
                                        anchors.left: parent.left
                                        anchors.right: parent.right

                                        title: qsTr("Auth using SASL")
                                        checkable: true

                                        labelBuddyPairs: [
                                            CustomText { text: qsTr("Account") },
                                            TextField {
                                                placeholderText: qsTr("")
                                                Layout.fillWidth: true
                                            },

                                            CustomText { text: qsTr("Passwords") },
                                            TextField {
                                                Layout.fillWidth: true
                                                echoMode: TextInput.Password
                                            }
                                        ]
                                    }
                                }
                            }
                            Tab {
                                title: qsTr("Encoding")
                                anchors.margins: tabView.tabMargins

                                Column {
                                    anchors.fill: parent

                                    FormLayoutInGroupBox {
                                        anchors.left: parent.left
                                        anchors.right: parent.right

                                        title: qsTr("Use Custom encodings:")

                                        labelBuddyPairs: [
                                            CustomText { text: qsTr("Format for sending text:") },
                                            ComboBox {
                                                Layout.fillWidth: true
                                            },

                                            CustomText { text: qsTr("消息接收回退：") },
                                            ComboBox {
                                                Layout.fillWidth: true
                                            },

                                            CustomText { text: qsTr("Server encoding formats:") },
                                            ComboBox {
                                                Layout.fillWidth: true
                                            }

                                        ]

                                    }
                                }
                            }
                        }
                    }
                }

            }
        }


        RowLayout {
            id: bottomButtonsLayout

            Layout.bottomMargin: 15
            Layout.leftMargin: parent.horizontalPadding
            Layout.rightMargin: parent.horizontalPadding
            Layout.fillWidth: true

            //  todo:"add data binding on Reset and Apply button"
            Button {
                text: qsTr("Reset")

            }
            Button {
                text: qsTr("Restore default")
            }

            //  an empty item to fill the remained space
            Item {
                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Ok")
            }
            Button {
                text: qsTr("Cannel")
            }
            Button {
                text: qsTr("Apply")
            }
        }
    }
}
