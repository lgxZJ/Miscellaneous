import QtQuick 2.0
import QtQuick.Controls 1.4

import lgxZJ.CppThemeHolder 1.0

Menu {
    title: qsTr("Theme(&G)")

    ExclusiveGroup { id: exclusiveGroup }

    MenuItem {
        text: qsTr("&Dark")
        checkable: true
        checked: false
        exclusiveGroup: exclusiveGroup

        onTriggered:{
            cppThemeHolderObject.changeToTheme(CppThemeHolder.ThemeDark)
        }
    }
    MenuItem {
        id: lightThemeMenu
        text: qsTr("&Light")
        checkable: true
        checked: false
        exclusiveGroup: exclusiveGroup

        onTriggered: {
            cppThemeHolderObject.changeToTheme(CppThemeHolder.ThemeLight)
        }
    }

    Component.onCompleted: lightThemeMenu.trigger()
}
