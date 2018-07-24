import QtQuick 2.0
import QtQuick.Controls 1.4

import lgxZJ.CppLanguageHolder 1.0

Menu {
    title: qsTr("Languages(&G)")

    ExclusiveGroup { id: exclusiveGroup }

    MenuItem {
        id: chineseLanguageMenu
        text: qsTr("&Chinese")
        checkable: true
        checked: false
        exclusiveGroup: exclusiveGroup

        onTriggered:{ cppLanguageHolderObject.changeTo(CppLanguageHolder.Chinese) }
    }
    MenuItem {
        text: qsTr("&English")
        checkable: true
        checked: false
        exclusiveGroup: exclusiveGroup

        onTriggered: { cppLanguageHolderObject.changeTo(CppLanguageHolder.English) }
    }

    Component.onCompleted: chineseLanguageMenu.trigger()
}
