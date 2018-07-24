import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import "./"

Item {
    id: root
    property string headerText: "lgxZJ"

    ColumnLayout {
        anchors.fill: parent

        CustomText {
            text: root.headerText
        }
        TableView {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
