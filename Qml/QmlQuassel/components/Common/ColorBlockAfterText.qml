import QtQuick 2.0
import QtQuick.Layouts 1.3

import "../Common"

RowLayout {
    property alias text: innerText.text
    property alias color: innerColorBlock.innerColor

    CustomText {
        id: innerText
        text: qsTr("lgxZJ")
    }
    ColorBlock {
        id: innerColorBlock
    }
}
