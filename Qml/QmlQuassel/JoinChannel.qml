import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import "./components/Common"

Window {
    id: root
    minimumWidth: 180
    minimumHeight: 120

    title: qsTr("Join Channel")
    color: cppThemeHolderObject.commonWindowBkColor
    modality: Qt.ApplicationModal
    //  Qt flags have strange behaviours, searched this for a lot of time. Qt.WindowContextHelpButtonHint
    //  and Qt.WindowSystemMenuHint works together.
    flags: Qt.Window
            | Qt.WindowTitleHint
            | Qt.WindowCloseButtonHint
            | Qt.WindowSystemMenuHint
            | Qt.WindowContextHelpButtonHint

    GridLayout {
        anchors.fill: parent
        anchors.margins:10
        columns: 1

        RowLayout {
            CustomText { text: qsTr("Networks:") }
            ComboBox { Layout.fillWidth: true }
        }

        RowLayout {
            CustomText { text: qsTr("Channels:") }
            TextField{ Layout.fillWidth: true }
        }

        RowLayout {
            CustomText { text: qsTr("Password:") }
            TextField{ Layout.fillWidth: true }
        }

        RowLayout {
            Layout.fillWidth: true

            Item { Layout.fillWidth: true }
            Button { text: qsTr("Ok") }
            Button { text: qsTr("Cancel") }
        }
    }
}
