import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.7

import "../Common/Common.js" as Common

Menu {
  title: qsTr("Settings(&S)")

  MenuItem {
      text: qsTr("Configure shortcuts(&S)...")
      iconSource: "qrc:///resources/ShortcutSettings.svg"
      onTriggered: Common.showDialog("qrc:///components/SettingsMenu/ConfigureShortcutsWindow.qml", appWindow)
  }
  MenuItem {
      text: qsTr("Configure Quassel(&C)...")
      shortcut: "F7"
      iconSource: "qrc:///resources/QuasselSettings.svg"
      onTriggered: Common.showDialog("qrc:///components/SettingsMenu/ConfigureQuasselWindow.qml", appWindow)
  }

}
