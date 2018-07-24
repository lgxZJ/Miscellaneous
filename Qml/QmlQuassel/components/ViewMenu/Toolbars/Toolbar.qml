import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.7

Menu {
    title: qsTr("Toolbars(&T)")

    MenuItem {
        text: qsTr("Main window toolbars")
        checkable: true
        checked: true
    }
}
