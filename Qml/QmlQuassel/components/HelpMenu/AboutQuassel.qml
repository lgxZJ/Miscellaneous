import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "../Common"

import "../Common/Common.js" as Common

Window {
    id: aboutQuasselWindow
    title: qsTr("关于Quassel")
    color: cppThemeHolderObject.commonWindowBkColor

    //  set a minimual width to contains at least all children contents (more spaces are accepted).
    minimumWidth: contentRow.width
                    + innerRectangle.border.width * 2
                    + outerRectangle.border.width * 2
                    + outestColumnLayout.anchors.margins * 2
    minimumHeight: 260

    width: 860
    height: 630

    //  Qt flags have strange behaviours, searched this for a lot of time. Qt.WindowContextHelpButtonHint
    //  and Qt.WindowSystemMenuHint works together.
    flags: Qt.Window
            | Qt.WindowTitleHint
            | Qt.WindowCloseButtonHint
            | Qt.WindowSystemMenuHint
            | Qt.WindowContextHelpButtonHint

    //  An extra Item full filling the outer Window to attach the `Keys` property
    //  to the window.
    Item {
        anchors.fill: parent

        Keys.onPressed: {
            if (event.key === Qt.Key_Escape)
                aboutQuasselWindow.close()
        }

        ColumnLayout {
            id: outestColumnLayout

            anchors.fill: parent
            anchors.margins: 10

            Rectangle {
                id: outerRectangle

                Layout.fillWidth: true
                height: innerRectangle.height + 2   //  two more pixels for self borders

                border.width: 1
                border.color: cppThemeHolderObject.aboutQuassel.ircBlockOuterBorderColor

                //  todo: try reuse this border container
                Rectangle {
                    id: innerRectangle

                    anchors.margins: 1
                    //  the y-axis starts with the value of parent.border.width
                    y: parent.border.width
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: contentRow.height  //  scale to `imageLayout`'s height

                    border.width: 1
                    border.color: cppThemeHolderObject.aboutQuassel.ircBlockInnerBorderColor

                    color: cppThemeHolderObject.aboutQuassel.ircBlockBackgroundColor

                    //  use `Row` not `RowLayout` since RowLayout scales its children by default which is not we wanted.
                    Row {
                        id: contentRow

                        padding: 10
                        spacing: 10

                        Image {
                            source: "qrc:///resources/quassel.ico"

                            height: textColumn.height
                            width: textColumn.height
                        }

                        Column {
                            id: textColumn

                            CustomText {
                                text: qsTr("Quassel IRC")

                                bottomPadding: 10
                                font.pointSize: 20
                                font.bold: true
                            }

                            Row{
                                CustomText {
                                    text: qsTr("Version: ")
                                    font.bold: true
                                }
                                CustomText {
                                    text: qsTr("v0.0.1 (github.com/lgxZJ)")
                                }
                            }

                            Row {
                                CustomText {
                                    text: qsTr("Version date: ")
                                    font.bold: true
                                }
                                CustomText {
                                    text: qsTr("周二 4月 24 05:44:48 2018")
                                }
                            }
                            Row {
                                CustomText {
                                    text: qsTr("Protocol version: ")
                                    font.bold: true
                                }
                                CustomText {
                                    text: qsTr("0")
                                }
                            }
                        }
                    }
                }
            }

            TabView {
                id: tabView
                property int paddingBeforeTabTitle: 15
                property int tabMargins: 15

                property var pseudoTabMnemonics: [ 'A', 'U', 'C', 'T' ]
                property bool altPressed : false

                Layout.fillHeight: true
                Layout.fillWidth: true

                style: TabViewStyle {
                    tab: Rectangle {
                        id: tabRectangle

                        property int nonSelectedWidth: tabTitle.width + 2 * tabView.paddingBeforeTabTitle
                        property int nonSelectedHeight: tabTitle.height + 10

                        property int selectedWidth: nonSelectedWidth + 2
                        property int selectedHeight: nonSelectedHeight + 2

                        implicitWidth: styleData.selected ? selectedWidth : nonSelectedWidth
                        implicitHeight: styleData.selected ? selectedHeight : nonSelectedHeight

                        /////////////////////////////////////////////////////////////////////////
                        //  NOTE:   The Tab's internal draw method overlaps ONE pixel by defualt,
                        //  to make it looks better we donot change the `x` property here.

                        //  grow height upwrads when this tab is selected
                        y: styleData.selected ? 0 : 2

                        border.width: 1
                        border.color:  cppThemeHolderObject.tabView.tabviewBorderColor

                        //  TODO:
                        //      This Text is centered in Rectangle below visualy, but we cannot directly get this
                        //  hierarchy information from code level, possibly can make the code better.

                        //  Use some 'Text's to mimic the behaviour and presentation of Mnemonics since TabView do not support it.
                        Row {
                            anchors.centerIn: parent

                            id: tabTitle

                            CustomText {
                                text: qsTr(styleData.title + '(')
                            }
                            CustomText {
                                font.underline: tabView.altPressed
                                text: qsTr(tabView.pseudoTabMnemonics[styleData.index])
                            }
                            CustomText {
                                text: qsTr(")")
                            }
                        }

                        Rectangle {
                            color: styleData.selected
                                       ? cppThemeHolderObject.tabView.selectedGradientBottomColor
                                       : cppThemeHolderObject.tabView.nonSelectedGradientBottomColor

                            x:1
                            y: parent.height - 1

                            //  skip one pixel in two horizontal directions
                            width: parent.width - 2
                            //  only overlap when this tab is selected
                            height: styleData.selected ? 1 : 0
                        }

                        gradient: Gradient {
                            GradientStop {
                                property color selectedTabTopColor: cppThemeHolderObject.tabView.selectedGradientTopColor
                                property color nonSelectedTopColor: cppThemeHolderObject.tabView.nonSelectedGradientTopColor

                                position: 0.0
                                color: styleData.selected ? selectedTabTopColor :nonSelectedTopColor
                            }
                            GradientStop {
                                id: bottomGradientStop
                                property color selectedTabBottomColor: cppThemeHolderObject.tabView.selectedGradientBottomColor
                                property color nonSelectedBottomColor: cppThemeHolderObject.tabView.nonSelectedGradientBottomColor

                                position: 1.0
                                color: styleData.selected ? selectedTabBottomColor : nonSelectedBottomColor
                            }
                        }
                    }

                    frameOverlap: 1
                    frame: Rectangle {
                        border.width: 1
                        border.color:  cppThemeHolderObject.tabView.frameBorderColor

                        color: cppThemeHolderObject.tabView.frameBkColor
                    }
                }

                focus: true
                Keys.onPressed: {
                    if (event.modifiers & Qt.AltModifier) {
                        altPressed = true

                        if (event.key === Qt.Key_A)
                            currentIndex = 0
                        if (event.key === Qt.Key_U)
                            currentIndex = 1
                        if (event.key === Qt.Key_C)
                            currentIndex = 2
                        if (event.key === Qt.Key_T)
                            currentIndex = 3
                    }
                }
                Keys.onReleased: {
                    //  when Alt key is released, the bits in `modifiers` is cleared
                    if (!(event.modifiers & Qt.AltModifier)) {
                        altPressed = false;
                    }
                }

                Tab {
                    id: aboutTab
                    title: qsTr("About")

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: tabView.tabMargins

                        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

                        //  NOTE:   Must use ColumnLayout because we want texts in Text to wrap automatically.
                        //  if using Column, we have to keep the maximum width manually which will cause a lot
                        //  work. The `anchors.margins` property not work inside ScrollView, so we decide to
                        //  set the ScrollView's instead.
                        ColumnLayout {
                            //  set a fixed value to make ScrollView show vertical scroll bars when needed
                            height: aboutTabTextEdit.height + tabView.tabMargins * 2
                            width: aboutTab.width - tabView.tabMargins * 2

                            EnhancedTextEdit {
                                id: aboutTabTextEdit
                                Layout.fillWidth: true

                                text: qsTr("<p style='line-height:130%;'>"
                                           + "<b>A modern, distributed IRC Client</b><br><br>"
                                           + "©2005-2018 by the Quassel Project<br>"
                                           + Common.linkWordToUrl("http://quassel-irc.org", "http://quassel-irc.org") + "<br>"
                                               + Common.linkWordToUrl("#quassel", "irc://irc.freenode.net/quassel") + " on " + Common.linkWordToUrl("Freenode", "http://www.freenode.net") + "<br><br>"
                                           + "Quassel IRC is dual-licensed under "
                                               + Common.linkWordToUrl("GPLv2", "http://www.gnu.org/licenses/gpl-2.0.txt")
                                               + " and "
                                               + Common.linkWordToUrl("GPLv3", "http://www.gnu.org/licenses/gpl-3.0.txt")
                                               + ".<br>"
                                           + Common.linkWordToUrl("Breeze icon theme", "https://api.kde.org/frameworks/breeze-icons/html")
                                               + " © Uri Herrera and others, licensed under the "
                                               + Common.linkWordToUrl("LGPLv3", "https://github.com/KDE/breeze-icons/blob/21ffd9b/COPYING-ICONS")
                                               + ".<br>"
                                           + Common.linkWordToUrl("Oxygen icon theme", "https://api.kde.org/frameworks/oxygen-icons5/html")
                                               + " © Nuno Pinheiro and others, licensed under the "
                                               + Common.linkWordToUrl("LGPLv3", "https://github.com/KDE/oxygen-icons/blob/master/COPYING")
                                               + ".<br><br>"
                                           + "Please use "
                                               + Common.linkWordToUrl("https://github.com/lgxZJ/QmlQuassel", "https://github.com/lgxZJ/QmlQuassel")
                                               + " to report bugs.</p>")
                            }
                            //  an empty item to get the remained vertical spaces
                            Item {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
                Tab {
                    id: authorTab
                    title: qsTr("Author")

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: tabView.tabMargins

                        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

                        ColumnLayout {
                            //  set a fixed value to make ScrollView show vertical scroll bars when needed
                            height: authorTabTextEdit.height + tabView.tabMargins * 2
                            width: authorTab.width - tabView.tabMargins * 2

                            EnhancedTextEdit {
                                id: authorTabTextEdit
                                Layout.fillWidth: true

                                text: qsTr("<p style='line-height:130%;'>"
                                            + "Quassel IRC 主要由下人员开发：<br><br>"
                                            + "<b>Gaoxin Lin(lgxZJ)</b><br>"
                                            + Common.htmlTab() + Common.linkWordToUrl("https://github.com/lgxZJ/QmlQuassel<br>", "https://github.com/lgxZJ/QmlQuassel")
                                            + Common.htmlTab() + "<i>Project Founder, Lead Developer</i></p>")
                            }
                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
                Tab {
                    id: contributorTab
                    title: qsTr("Contributor")

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: tabView.tabMargins

                        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

                        ColumnLayout {
                            //  set a fixed value to make ScrollView show vertical scroll bars when needed
                            height: contributorTabTextEdit.height + tabView.tabMargins * 2
                            width: contributorTab.width - tabView.tabMargins * 2

                            EnhancedTextEdit {
                                id: contributorTabTextEdit
                                Layout.fillWidth: true

                                text: qsTr("我们感谢下列贡献者(按字母顺序排列)和所有我们忘记在这里提及的人们：<br><br>"
                                            + "<b>Empty</b>")
                            }
                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                        }

                    }
                }
                Tab {
                    id: acknowledgementTab
                    title: qsTr("Acknowledgements")

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: tabView.tabMargins

                        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

                        ColumnLayout {
                            spacing: 20

                            //  set a fixed value to make ScrollView show vertical scroll bars when needed
                            height: acknowledgementTabText.height
                                        + acknowledgementTabFormOne.height
                                        + acknowledgementTabFormTwo.height
                                        + spacing * 3
                                        + tabView.tabMargins * 2
                            width: acknowledgementTab.width - tabView.tabMargins * 2

                            CustomText {
                                id: acknowledgementTabText
                                //  NOTE: No need two `<br>` only one, QML Items are block elements.
                                text: qsTr("Special thanks goes to:")

                                Layout.fillWidth: true
                                wrapMode: Text.WordWrap
                            }

                            //  Use Row not RowLayout to keep child Items' as them are, because of this we have
                            //  to manage the EnhancedTextEdit's width mannually to make its word wrap functionality
                            //  works. We cannot use the `anchors` property inside Row, it conflicts.
                            Form {
                                id: acknowledgementTabFormOne
                                Layout.fillWidth: true

                                spacing: 10

                                labelBuddyPairs: [
                                    Image {
                                        id: iconImageOne
                                        fillMode: Image.Stretch

                                        //  necessary width and height values to make Image stretch
                                        width: 32
                                        height: 32

                                        source: "qrc:///resources/quassel.ico"
                                    },
                                    EnhancedTextEdit {
                                        //  needed to make wrapMode work
                                        width: parent.width - iconImageOne.width - 10

                                        //todo: extract <p style='line-height:130%;'>
                                        text: qsTr("<p style='line-height:130%;'>"
                                                    + "<b>John \"nox\" Hand</b><br>"
                                                    + "<i>for the original Quassel logo - The All-Seeing Eye</i>")
                                        wrapMode: Text.WordWrap
                                    }
                                ]
                            }

                            Form {
                                id: acknowledgementTabFormTwo
                                Layout.fillWidth: true
                                spacing: 10

                                labelBuddyPairs: [
                                    Image {
                                        id: iconImageTwo
                                        fillMode: Image.Stretch
                                        width: 32
                                        height: 32
                                        source: "qrc:///resources/Qt.svg"
                                    },
                                    EnhancedTextEdit {
                                        width: parent.width - iconImageTwo.width - 10

                                        //todo: extract <p style='line-height:130%;'>
                                        text: qsTr("<p style='line-height:130%;'>"
                                                    + "<b>" + Common.linkWordToUrl("The Qt Company", "https://www.qt.io/") + "</b> (formerly known as Qt Software, Nokia, Trolltech)<br>"
                                                    + "<i>for creating an awesome framework, and for sponsoring development with Greenphones, N810s, N950s and more</i>")
                                        wrapMode: Text.WordWrap
                                    }
                                ]
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
            }
            Row {
                Layout.fillWidth: true

                LayoutMirroring.enabled: true

                Button {
                    text: qsTr("Close")
                    isDefault: Window.active

                    //  NOTE:  Cannot call close() function from `Window` attached property
                    onClicked: aboutQuasselWindow.close()
                }
            }
        }

    }
}
