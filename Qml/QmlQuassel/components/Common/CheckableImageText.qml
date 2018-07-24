import QtQuick 2.0
import QtQuick.Controls 1.4

import "./"

Row {
    property alias checked: emptyCheckBox.checked
    property alias source: image.source
    property alias text: textt.text

    spacing: 5

    CheckBox {
        id: emptyCheckBox

        anchors.verticalCenter: parent.verticalCenter
    }
    Image {
        id: image
        anchors.verticalCenter: parent.verticalCenter

        width: 16
        height: 16
        source: "qrc://resources/Qt.svg"
    }
    CustomText {
        id: textt
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("lgxZJ")
    }
}
