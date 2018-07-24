import QtQuick.Controls 1.4
import QtQml 2.2

import "../../Common/Common.js" as Common

Menu {
    title: qsTr("Debug")
    iconSource: "qrc:///resources/Bug.svg"

    MenuItem {
        text: qsTr("Debug network model(&N)")
        iconSource: "qrc:///resources/Bug.svg"
        onTriggered: Common.showDialog("qrc:///components/HelpMenu/DebugMenu/DebugNetworkModel.qml", null)
    }
    MenuItem {
        text: qsTr("Debug buffer view cover(&B)")
        iconSource: "qrc:///resources/Bug.svg"
        onTriggered: Common.showDialog("qrc:///components/HelpMenu/DebugMenu/DebugCacheView.qml", null)
    }
    MenuItem {
        text: qsTr("Debug msg model(&M)")
        iconSource: "qrc:///resources/Bug.svg"
        onTriggered: Common.showDialog("qrc:///components/HelpMenu/DebugMenu/DebugMsgModel.qml", null)
    }
    MenuItem {
        text: qsTr("Debug hot lists(&H)")
        iconSource: "qrc:///resources/Bug.svg"
        onTriggered: Common.showDialog("qrc:///components/HelpMenu/DebugMenu/DebugHotList.qml", null)
    }
    MenuItem {
        text: qsTr("Debug logs(&L)")
        iconSource: "qrc:///resources/Bug.svg"
        onTriggered: Common.showDialog("qrc:///components/HelpMenu/DebugMenu/DebugLog.qml", null)
    }

    MenuSeparator {}

    MenuItem {
        text: qsTr("Reload style sheets")
        iconSource: "qrc:///resources/Reload.svg"
        shortcut: "F5"
    }
}
