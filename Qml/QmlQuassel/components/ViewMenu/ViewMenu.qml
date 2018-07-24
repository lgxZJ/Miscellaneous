import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.7

import "./ChatListMenu"
import "./Toolbars"

Menu {
    title: qsTr("Look(&V)")

    ChatListMenu {}
    Toolbar {}

    MenuSeparator {}

    MenuItem {
        text: qsTr("Show menu bars(&M)")
        iconSource: "qrc:///resources/MenuBar.svg"
        checkable: true
        shortcut: "Ctrl+M"
    }
    MenuItem {
        text: qsTr("Show status bars(&B)")
        checkable: true
    }
    MenuItem {
        text: qsTr("Show Search bars(&S)")
        iconSource: "qrc:///resources/Search.svg"
        checkable: true
        shortcut: "Ctrl+F"
    }
    MenuItem {
        text: qsTr("Show logs")
    }

    MenuSeparator {}

    MenuItem {
        text: qsTr("Lock layouts(&L)")
        checkable: true
    }
    MenuItem {
        text: qsTr("Show topic lines")
        checkable: true
    }
    MenuItem {
        text: qsTr("Show nick lists")
        checkable: true
    }
    MenuItem {
        text: qsTr("Show input lines")
        checkable: true
    }
    MenuItem {
        text: qsTr("Show chat monitors")
        checkable: true
    }

    MenuSeparator {}

    MenuItem {
        text: qsTr("Fullscreen display(&F)")
        checkable: true
        iconSource: "qrc:///resources/Fullscreen.svg"
    }
}
