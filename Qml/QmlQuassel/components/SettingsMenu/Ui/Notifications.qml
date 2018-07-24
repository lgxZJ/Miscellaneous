import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.7

import "../../Common"

ColumnLayout {
    TitleHeader {
        Layout.fillWidth: true

        title: qsTr("Notifications")
    }
    ColumnLayout {
        Layout.fillWidth: true
        Layout.margins: 10

        spacing:20

        Row {
            CheckableImageText {
                anchors.verticalCenter: parent.verticalCenter

                checked: true
                source: "qrc:///resources/Flag.svg"
                text: qsTr("Mark task bar, timeout:")
            }

            SpinBox {
                anchors.verticalCenter: parent.verticalCenter

                suffix: qsTr(" 秒")
            }
        }
        Row {
            CheckableImageText {
                checked: true
                source: "qrc:///resources/Info.svg"
                text: qsTr("Show msg in popups")
            }
        }
        GroupBox {
            Layout.fillWidth: true

            checkable: false

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                ColumnLayout {
                    id: snoreLayout

                    Layout.fillWidth: true
                    Layout.margins: 7

                    enabled: snoreCheckbox.checked

                    TabView {
                        id: tabView
                        property int horizontalTabTextMargin: 15

                        Layout.fillWidth: true

                        style: CustomTabViewStyle { horizontalTabTextMargin: tabView.horizontalTabTextMargin }

                        Tab {
                            title: qsTr("General")
                            anchors.margins: tabView.horizontalTabTextMargin

                            GridLayout {
                                anchors.fill: parent
                                columns: 2

                                CustomText { text: qsTr("Primary Backend:") }
                                ComboBox {
                                    Layout.fillWidth: true

                                    model: [ "Windows Toast" ]
                                }

                                CustomText { text: qsTr("Timeout:") }
                                SpinBox {
                                    Layout.fillWidth: true

                                    value: 10
                                    suffix: "s"
                                }

                                CustomText { text: qsTr("Disable Notification Sound:") }
                                CheckBox {  }
                            }
                        }
                        Tab {
                            title: qsTr("Primary Backends")
                        }
                        Tab {
                            title: qsTr("Secondary Backends")
                        }
                    }

                    Button {
                        Layout.fillWidth: true

                        text: qsTr("Display Test Notification")
                    }
                }
                Row {
                    spacing: 3

                    CustomText {
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Enable Snore")
                    }
                    CheckBox {
                        id: snoreCheckbox
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }
    Item {
        Layout.fillHeight: true
    }
}
