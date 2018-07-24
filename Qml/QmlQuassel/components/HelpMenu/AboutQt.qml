import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick 2.7
import QtQuick.Controls 1.4

import "../Common"

import "../Common/Common.js" as Common

Window {
    id: aboutQtWindow
    property int margins: 15

    width: 585
    height: contentRow.height + buttonRectangle.height + column.spacing + margins * 2

    minimumWidth: width
    maximumWidth: width

    minimumHeight: height
    maximumHeight: height

    color: cppThemeHolderObject.commonWindowBkColor

    flags: Qt.Window
            | Qt.WindowTitleHint
            | Qt.WindowCloseButtonHint
            | Qt.MSWindowsFixedSizeDialogHint

    Item {
        focus: true
        anchors.fill: parent

        Keys.onPressed: {
            if (event.key === Qt.Key_Escape)
                aboutQtWindow.close()
        }

        ColumnLayout {
            id: column
            //  must use `parent` not `aboutQtWindow`, cannot anchor to a non-item
            anchors.fill: parent

            spacing: aboutQtWindow.margins

            Row {
                id: contentRow

                //  WARNING:    Layout children cannot use `anchors.margins` which conflicts to Layout.margins
                Layout.leftMargin: aboutQtWindow.margins

                Layout.fillWidth: true
                spacing: 10

                Image {
                    id: image
                    source: "qrc:///resources/Qt.svg"
                    width: 64
                    height: 64
                    fillMode: Image.Stretch
                }

                EnhancedTextEdit {
                    id: enhancedTextEdit
                    //  cannot calculte from ColumnLayout's width
                    width: aboutQtWindow.width - image.width - contentRow.spacing - aboutQtWindow.margins * 2

                    text: qsTr("<b>  About Qt</b><br><br>"
                                + "This program uses Qt version 5.2.1.<br><br>"
                                + "Qt is a C++ toolkit for cross-platform application development.<br><br>"
                                + "Qt provides single-source portability across all major desktop operating systems. It is also available for embedded Linux and other embedded and mobile operating systems.<br><br>"
                                + "Qt is available under three different licensing options designed to accommodate the needs of our various users.<br><br>"
                                + "Qt licensed under our commercial license agreement is appropriate for development of proprietary/commercial software where you do not want to share any source code with third parties or otherwise cannot comply with the terms of the GNU LGPL version 2.1 or GNU GPL version 3.0.<br><br>"
                                + "Qt licensed under the GNU LGPL version 2.1 is appropriate for the development of Qt applications provided you can comply with the terms and conditions of the GNU LGPL version 2.1.<br><br>"
                                + "Qt licensed under the GNU General Public License version 3.0 is appropriate for the development of Qt applications where you wish to use such applications in combination with software subject to the terms of the GNU GPL version 3.0 or where you are otherwise willing to comply with the terms of the GNU GPL version 3.0.<br><br>"
                                + "Please see " + Common.linkWordToUrl("qt.io/licensing", "http://qt.io/licensing/") + " for an overview of Qt licensing.<br><br>"
                                + "Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies) and other contributors.<br><br>"
                                + "Qt and the Qt logo are trademarks of Digia Plc and/or its subsidiary(-ies).<br><br>"
                                + "Qt is developed as an open source project on qt-project.org.<br><br>"
                                + "Qt is a Digia product. See " + Common.linkWordToUrl("qt.digia.com", "http://qt.io/") + " for more information.")
                }
            }
            Rectangle {
                id: buttonRectangle

                Layout.fillWidth: true
                height: buttonRow.height + aboutQtWindow.margins * 2
                anchors.margins: aboutQtWindow.margins
                color: cppThemeHolderObject.buttonBlockColor

                Row {
                    id: buttonRow
                    x: -aboutQtWindow.margins   //  a negative value since its layout is mirrored
                    y: aboutQtWindow.margins
                    height: okButton.height
                    width: parent.width

                    LayoutMirroring.enabled: true

                    Button {
                        id: okButton
                        anchors.margins: aboutQtWindow.margins

                        text: qsTr("Ok")

                        isDefault: aboutQtWindow.active
                        onClicked: aboutQtWindow.close()
                    }
                }
            }
        }

    }
}

