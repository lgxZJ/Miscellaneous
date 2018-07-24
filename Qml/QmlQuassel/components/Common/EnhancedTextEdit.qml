//import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick 2.7

import "../Common"

//////////////////////////////////////////////////////////////////////////////
//  An echanced TextEdit which supports Selection and Link functinality.
TextEdit {
    property color selectedColor: cppThemeHolderObject.textEdit.normalSelectionColor

    text: ""
    color: cppThemeHolderObject.commonTextBkColor

    wrapMode: Text.WordWrap
    textFormat: Text.RichText
    selectionColor : selectedColor
    readOnly: true

    MouseArea {
        property bool selectionBegins: false
        property int selectionStart: -1
        property int selectionEnd: -1

        property Menu contextMenu: ContextMenu {}

        anchors.fill: parent

        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        ////////////////////////////////////////////////////////////////////////////////////////
        //  NOTE:   There is no such `pressAndHoldInterval` property as Qt Documentation says.
        //  We have to mimic that property using a custom Timer.
        Timer {
            id: pressedIntervalTimer
            interval: 200
            onTriggered:  parent.selectionBegins = true
        }

        function resetSelection() {
            selectionStart = selectionEnd = -1
            parent.deselect()
            parent.selectionColor = parent.selectedColor
            parent.selectedTextColor = "white"
        }

        function resetCursor() {
            cursorShape = Qt.ArrowCursor
        }

        onExited: {
            resetCursor()
        }

        onClicked: {
            if (mouse.button === Qt.LeftButton) {
                var linkUrl = parent.linkAt(mouse.x, mouse.y)
                if (linkUrl !== '')
                    if (Qt.openUrlExternally(linkUrl) === false)
                        console.log('openUrlExternally failed!')
            }
            if (mouse.button === Qt.RightButton) {

                if (parent.selectedText !== '') {
                    contextMenu.enableCopy = true
                    contextMenu.copiedText = parent.selectedText
                } else {
                    contextMenu.enableCopy = false
                }

                if (parent.linkAt(mouse.x, mouse.y) !== '') {
                    contextMenu.enableCopyLinkLocation = true
                    contextMenu.copiedLinkUrl = parent.linkAt(mouse.x, mouse.y)
                } else {
                    contextMenu.enableCopyLinkLocation = false
                }

                if (parent.text !== '') {
                    contextMenu.enableSelectAll = true
                } else {
                    contextMenu.enableSelectAll = false
                }

                contextMenu.popup()
            }
        }

        onPressed: {
            pressedIntervalTimer.start()

            if (mouse.button === Qt.LeftButton)
                resetSelection()
            else {
                parent.selectionColor = cppThemeHolderObject.textEdit.pressedSelectionColor
                parent.selectedTextColor = "black"
            }
        }
        onReleased: {
            pressedIntervalTimer.stop()

            if (mouse.button === Qt.LeftButton)
                selectionBegins = false
        }

        onPositionChanged: {
            if (selectionBegins) {
                if (selectionStart === -1) {
                    selectionStart = selectionEnd = parent.positionAt(mouse.x, mouse.y)
                } else {
                    selectionEnd = parent.positionAt(mouse.x, mouse.y)
                }

                parent.select(selectionStart, selectionEnd)
            }

            if (parent.linkAt(mouse.x, mouse.y) !== '') {
                //  hovered above an link
                cursorShape = Qt.PointingHandCursor
            } else {
                //  this not ensures the cursor shape will be reset when the mouse moves
                //  outside the TextEdit area (probably works because the TextEdit has paddings),
                //  call `resetCursor()` to ensures this.
                resetCursor()

                console.log("no link under pos:", mouse.x, mouse.y)
            }
        }
    }
}
