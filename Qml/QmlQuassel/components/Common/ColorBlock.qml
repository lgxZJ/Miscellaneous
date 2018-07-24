import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Dialogs 1.3

import "../Common"

import "../Common/Common.js" as Common

Rectangle {
    id: root

    property color innerColor: cppThemeHolderObject.colorBlock.originInerColor
    property color hoveredOuterBorderColor: cppThemeHolderObject.colorBlock.hoveredOuterBorderColor

    property color originOuterBorderColor: cppThemeHolderObject.colorBlock.originOuterBorderColor
    property color originOuterColor: cppThemeHolderObject.colorBlock.originOuterColor
    property color originInnerBorderColor:  cppThemeHolderObject.colorBlock.originInerBorderColor
    property color originInnerColor: innerColor

    property color grayOuterBorderColor: cppThemeHolderObject.colorBlock.grayOuterBorderColor
    property color grayOuterColor: cppThemeHolderObject.colorBlock.grayOuterColor
    property color grayInnerBorderColor:  cppThemeHolderObject.colorBlock.grayInnerBorderColor
    property color grayInnerColor: Common.moreGray(root.originInnerColor.r,
                                                   root.originInnerColor.g,
                                                   root.originInnerColor.b)

    //  Make ColorBlock's color automatically turn to gray when disabled. Must use Qt.binding(),
    //  for more details see Qt documentations.
    onEnabledChanged: {
        if (enabled) {
            root.border.color = Qt.binding(function() { return originOuterBorderColor })
            root.color = Qt.binding(function() { return originOuterColor })

            innerRectangle.border.color = Qt.binding(function() { return originInnerBorderColor})
            innerRectangle.color = Qt.binding(function() { return originInnerColor })
        } else {
            root.border.color = Qt.binding(function() { return grayOuterBorderColor })
            root.color = Qt.binding(function() { return grayOuterColor })

            innerRectangle.border.color = Qt.binding(function() { return grayInnerBorderColor })
            innerRectangle.color = Qt.binding(function() { return grayInnerColor })
        }
    }

    implicitHeight: 20
    implicitWidth: 20
    color: originOuterColor

    border.width: 1
    border.color: originOuterBorderColor

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            root.border.color = Qt.binding(function() { return root.hoveredOuterBorderColor })
        }
        onExited: {
            root.border.color = Qt.binding(function() { return root.originOuterBorderColor })
        }

        onClicked: {
            colorDialog.visible = true
        }

        ColorDialog {
            id: colorDialog

            visible: false
            title: "Select Color"
            modality: Qt.ApplicationModal

            onAccepted: {
                root.innerColor = Qt.binding(function() { return colorDialog.currentColor})
                console.log("color dialog:", root.innerColor)
                visible = false
            }
            onRejected: {
                console.log("on rejected")
                visible = false
            }
        }

        Rectangle {
            id: innerRectangle

            //  todo: the following margins seems not work
            anchors.fill: parent
            anchors.margins: 1

            border.width: 1
            border.color: originOuterColor

            color: root.innerColor
        }
    }
}
