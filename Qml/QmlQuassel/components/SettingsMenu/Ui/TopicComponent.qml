import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import "../../Common"

ColumnLayout {
    TitleHeader {
        Layout.fillWidth: true

        title: qsTr("Topic components")
    }
    ColumnLayout {
        Layout.fillWidth: true

        FontEditLine { Layout.fillWidth: true }
        CheckBox { text: qsTr("Dynamic change sizes") }
        CheckBox {
            Layout.leftMargin: 10
            text: qsTr("Only on mouse hovers")
        }
    }
}
