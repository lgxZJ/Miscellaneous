import QtQuick.Layouts 1.3
import QtQuick 2.7

import "./"

ColumnLayout {
    property string title: qsTr("title")

    CustomText {
        text: parent.title
        font.bold: true
    }
    Separator {
        Layout.fillWidth: true
    }
}
