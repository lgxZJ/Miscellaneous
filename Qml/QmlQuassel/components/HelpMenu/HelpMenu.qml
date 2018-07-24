import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.7

import "./DebugMenu"

import "../Common/Common.js" as Common

Menu {
    title: qsTr("Help(&H)")

    MenuItem {
        text: qsTr("About Quassel(&A)")
        iconSource: "qrc:///resources/quassel.ico"

        onTriggered: Common.showDialog("qrc:///components/HelpMenu/AboutQuassel.qml", appWindow)
    }
    MenuItem {
        text: qsTr("About &Qt")
        iconSource: "qrc:///resources/Qt.svg"

        onTriggered: Common.showDialog("qrc:///components/HelpMenu/AboutQt.qml", appWindow)
    }

    MenuSeparator {}

    DebugMenu {}
}
