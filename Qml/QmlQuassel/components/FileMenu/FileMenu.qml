import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.7

import "../Common/Common.js" as Common

Menu {
    title: qsTr("Files(&F)")

    MenuItem {
        text: qsTr("Connect to cores(&C)")
        iconSource: "qrc:///resources/ConnectToCore.png"

        onTriggered: Common.showDialog("qrc:///components/FileMenu/ConnectToCoreDialog.qml", appWindow)
    }
    MenuItem {
        text: qsTr("Diconnect from Core(&D)")
        iconSource: "qrc:///resources/DisconnectFromCore.png"
    }
    MenuItem {
        text: qsTr("Change passwords(&P)")
        iconSource: "qrc:///resources/ChangePassword.svg"
    }
    MenuItem {
        text: qsTr('Core information(&I)')
        iconSource: "qrc:///resources/CoreInfo.png"
    }

    MenuSeparator {}

    Menu{
        title: qsTr("Networks(&N)")

        MenuItem {
            text: qsTr("Configure Networks(&N)")
            iconSource: "qrc:///resources/NetworkSettings.svg"

            onTriggered: Common.showDialog("qrc:///components/FileMenu/ConfigureNetworksDialog.qml", appWindow)
        }
    }

    MenuSeparator {}

    MenuItem {
        text: qsTr("Quit(&Q)")
        iconSource: "qrc:///resources/ExitApp.png"
    }
}
