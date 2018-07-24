import QtQuick 2.0
import QtQuick 2.7

import "./"

import "Common.js" as Common

ListView {
    id: treeView
    focus: true

    signal itemClicked(int index)

    property int itemLeftPadding: 5
    property int itemHeight: 20
    property int maxWidth: 200
    property int minWidth: 35

    //////////////////////////////////////////////////////////////////////////////
    //  NOET:   Due to tech difficulties expanding/unexpanding is not implemented.
    //////////////////////////////////////////////////////////////////////////////
    delegate: Rectangle {
        id: delegateComponent

        //  an item is focused if it is hovered or current item.
        property color focusedItemBorderColor:  cppThemeHolderObject.simpleTreeView.focusedItemBorderColor

        property color currentItemColor: cppThemeHolderObject.simpleTreeView.currentItemBkColor
        property color hoveredItemColor: cppThemeHolderObject.simpleTreeView.hoveredItemBkColor
        property color normalItemBkColor: cppThemeHolderObject.simpleTreeView.normalItemBkColor

        function renderToNotSelected() {
            delegateComponent.border.color = Qt.binding(function() { return delegateComponent.color })
            delegateComponent.color = Qt.binding(function() {
                return delegateComponent.ListView.isCurrentItem ? currentItemColor : normalItemBkColor
            })
        }

        function renderToSelectedWithNoFocus() {
            delegateComponent.color = currentItemColor
            delegateComponent.border.color = Qt.binding(function() { return delegateComponent.color })
        }

        function renderToSelectedWithFocus() {
            delegateComponent.color = currentItemColor
            delegateComponent.border.color = focusedItemBorderColor
        }

        onFocusChanged: {
            if (!focus)
                renderToNotSelected()
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            onEntered: {
                if (delegateComponent.ListView.isCurrentItem)
                    delegateComponent.border.color = parent.focusedItemBorderColor
                else
                    delegateComponent.color = parent.hoveredItemColor
            }
            onExited: {
                if (delegateComponent.ListView.isCurrentItem)
                    parent.renderToSelectedWithNoFocus()
                else
                    parent.renderToNotSelected()
            }
            onClicked: {
                parent.renderToSelectedWithFocus()
                treeView.currentIndex = model.index

                itemClicked(model.index)
            }
        }

        enabled: !model.isDisabled
        width: treeView.width
        height: treeView.itemHeight
        border.width: 1
        border.color: color
        color:  delegateComponent.ListView.isCurrentItem ? currentItemColor : normalItemBkColor

        AutoGrayText {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            leftPadding: model.isSubEntry ? 20 : treeView.itemLeftPadding
            text: model.text
        }
    }

    header: Rectangle {
        width: treeView.width
        height: treeView.itemHeight
        color: cppThemeHolderObject.simpleTreeView.normalHeaderBkColor

        CustomText {
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            leftPadding: treeView.itemLeftPadding
            text: qsTr("Settings")
        }
    }
}
