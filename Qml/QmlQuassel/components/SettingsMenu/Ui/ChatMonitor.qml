import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import "../../Common"

ColumnLayout {
    TitleHeader {
        Layout.fillWidth: true
        title: qsTr("Chat monitor")
    }

    RowLayout {
        Layout.fillWidth: true
        CustomText {
            text: qsTr("Operation mode")
        }
        ComboBox {
            model: [ "", "Opt In", "Opt Out" ]

            onActivated: {
                switch (index) {
                case 0:     //  intended fall through
                case 1:     secondList.headerText = qsTr("Show:");    break;

                case 2:     secondList.headerText = qsTr("Ignore：");    break;

                default:    throw new "unexpected list index:" + index;
                }
            }
        }
    }

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true

        ListViewWithHeader {
            id: firstList
            Layout.fillHeight: true
            Layout.fillWidth: true

            headerText: qsTr("Usable")
        }
        ColumnLayout {
            Button { iconSource: "qrc:///resources/ArrowLeft.svg" }
            Button { iconSource: "qrc:///resources/ArrowRight.svg" }
        }
        ListViewWithHeader {
            id: secondList
            Layout.fillHeight: true
            Layout.fillWidth: true

            headerText: qsTr("Show:")
        }
    }

    CheckBox {
        Layout.fillWidth: true

        text: qsTr("Always hightligh informations")
    }
    CheckBox {
        Layout.fillWidth: true

        text: qsTr("Show my infos")
    }
    CheckBox {
        Layout.fillWidth: true

        text: qsTr("show message from backlog")
    }
    CheckBox {
        Layout.fillWidth: true
        Layout.leftMargin: 10

        text: qsTr("Include read messages")
    }
}
