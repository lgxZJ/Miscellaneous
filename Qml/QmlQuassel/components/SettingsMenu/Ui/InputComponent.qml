import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4
import QtQuick 2.7

import "../../Common"

ColumnLayout {
    TitleHeader {
        Layout.fillWidth: true

        title: qsTr("Input components")
    }
    ColumnLayout {
        Layout.fillWidth: true
        Layout.margins: 5

        FontEditLine {
            Layout.fillWidth: true

            checkBoxText: qsTr("Custom fonts:")
            textFieldText: qsTr("SimSun 9pt")
            buttonText: qsTr("Selects...")
        }
        CheckBox { text: qsTr("Enable each chat's history") }
        CheckBox { text: qsTr("Show nick selector") }
        CheckBox { text: qsTr("Show style  icons") }
        CheckBox { text: qsTr("Emacs key bindings") }
        CheckBox { text: qsTr("Line Wrapping") }
        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Multiline editing")

            ColumnLayout {
                RowLayout {
                    CustomText { text: qsTr("Maximum display") }
                    SpinBox { value: 5 }
                    CustomText { text: qsTr("line") }
                }
                CheckBox { text: qsTr("Enable scrollbar") }
            }
        }
        GroupBox {
            Layout.fillWidth: true

            title: qsTr("Tab补全")

            ColumnLayout {
                RowLayout {
                    CustomText { text: qsTr("Complete suffix") }
                    TextField { text: qsTr(":") }
                }
                CheckBox { text: qsTr("Add space after nick when completing mid-sentence") }
            }
        }
    }
}
