import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

RowLayout {
    property alias checkBoxText: checkBox.text
    property alias textFieldText: textField.text
    property alias buttonText: button.text

    CheckBox {
        id: checkBox
        text: qsTr("Custom Fonts:")
    }
    TextField {
        id: textField

        Layout.fillWidth: true

        text: qsTr("SimSun 9pt")
    }
    Button {
        id: button

        text: qsTr("Select...")
    }
}
