import QtQuick.Controls 1.4

Menu {
    property alias enableCopy: copyMenu.enabled
    property alias enableCopyLinkLocation: copyLinkLocationMenu.enabled
    property alias enableSelectAll: selectAllMenu.enabled

    //  Because Menu does not inherits from QtObject for which we cannot use
    //  Connections to connect this menu's signals to outer scopes. Thus we
    //  provide a property to pass the needed text into this Component.
    property string copiedText: ""
    property string copiedLinkUrl: ""

    onCopiedTextChanged: console.log("copiedText changed to:", copiedText)
    onCopiedLinkUrlChanged:  console.log("copiedLinkUrl changed to:", copiedLinkUrl)

    MenuItem {
        id: copyMenu
        text: qsTr("Copy")
    }
    MenuItem {
        id: copyLinkLocationMenu
        text: qsTr("Copy Link Location")
    }

    MenuSeparator {}

    MenuItem {
        id: selectAllMenu
        text: qsTr("Select All")
    }
}
