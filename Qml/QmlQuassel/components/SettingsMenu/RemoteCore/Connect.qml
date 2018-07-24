import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import "../../Common"

ColumnLayout {
    TitleHeader {
        Layout.fillWidth: true

        title: qsTr("Connect")
    }
    ColumnLayout {
        Layout.margins: 5

        GroupBox {
            Layout.fillWidth: true

            checkable: false
            title: qsTr("Network status detection")

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5

                ColumnLayout {
                    RadioButton { text: qsTr("Automatic") }
                    Row {
                        spacing: 5

                        RadioButton { text: qsTr("Automatic") }
                        SpinBox { value: 60; suffix: qsTr(" 秒") }
                    }
                    RadioButton { text: qsTr("Never try active timeout") }
                }
            }
        }
        CheckBox { text: qsTr("Auto reconnect after dump") }
        RowLayout {
            spacing: 5
            Layout.leftMargin: 20

            CustomText { text: qsTr("Retry interval") }
            SpinBox { value: 60; suffix: qsTr("seconds") }
        }
    }
}
