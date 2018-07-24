import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick 2.7
import QtQuick.Layouts 1.3

import "../Common/Common.js" as Common
import "../Common"

Window {
    id: connectToCoreDialog

    title: qsTr("Connect to Core")

    visible: true
    modality: Qt.ApplicationModal
    minimumHeight: 297
    minimumWidth: 262
    height: 400
    width: 430
    color: cppThemeHolderObject.commonWindowBkColor

    ConnectToCoreContent {
        parentObject: parent

        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 10
    }
}
