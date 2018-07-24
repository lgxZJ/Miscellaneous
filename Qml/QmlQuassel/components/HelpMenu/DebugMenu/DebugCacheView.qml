import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQml 2.2

import "../../Common"

Window {
    title: qsTr("Debug buffer view cover")
    color: cppThemeHolderObject.commonWindowBkColor
    width: 580
    height: 550

    minimumWidth: 260
    minimumHeight: 470

    //  todo: reuse
    TabView {
        id: tabView
        anchors.fill: parent

        style: CustomTabViewStyle {}
        focus: true

        Tab {
            id: viewTab
            title: qsTr("Cover view")

            TreeView {
                id: treeView
                anchors.fill: parent
                alternatingRowColors: false

                property int nonLastColumnWidth: 250
                property int lastColumnImplicitWidth: 70


                TableViewColumn {
                    title: qsTr("Chat")
                    role: "AAA"
                    width: treeView.nonLastColumnWidth
                }
                TableViewColumn {
                    title: qsTr("Theme")
                    width: treeView.nonLastColumnWidth
                }
                TableViewColumn {
                    title: qsTr("Nick account")
                    width: {
                        //  substract two border pixel to get the real width
                        var actualWidth = treeView.width - (treeView.columnCount - 1) * treeView.nonLastColumnWidth - 2
                        return actualWidth > treeView.lastColumnImplicitWidth
                                    ? actualWidth
                                    : treeView.lastColumnImplicitWidth
                    }
                }

                //  todo: model
            }
        }
        Tab {
            id: propertyTab
            title: qsTr("Cover properties")

            Column {
                id: column
                anchors.fill: parent
                anchors.margins: 15

                property int maxFormLayoutHeight: 775

                FormLayout {
                    id: formLayout
                    width: column.width

                    height: column.height > column.maxFormLayoutHeight ? column.maxFormLayoutHeight : column.height

                    labelBuddyPairs: [
                        CustomText {
                            text: qsTr("Buffer views")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        TextField {
                            Layout.fillWidth: true
                            text: qsTr("0")
                        },

                        CustomText {
                            text: qsTr("All networks")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        CustomText {
                            text: qsTr("yes")
                            Layout.fillWidth: true
                        },

                        CustomText {
                            text: qsTr("Networks")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        TextArea {
                            text: qsTr("0")
                            textColor: cppThemeHolderObject.commonTextBkColor
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        },

                        CustomText {
                            text: qsTr("Buffers")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        TextArea {
                            text: qsTr("5, 6")
                            textColor: cppThemeHolderObject.commonTextBkColor
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        },

                        CustomText {
                            text: qsTr("Removed Buffers")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        TextArea {
                            text: qsTr("5, 6")
                            textColor: cppThemeHolderObject.commonTextBkColord
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        },

                        CustomText {
                            text: qsTr("Temporarily removed buffers")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        TextArea {
                            text: qsTr("5, 6")
                            textColor: cppThemeHolderObject.commonTextBkColor
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        },

                        CustomText {
                            text: qsTr("Submitted buffer types:")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        CustomText {
                            text: qsTr("15")
                            Layout.fillWidth: true
                        },

                        CustomText {
                            text: qsTr("Minimum activities:")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        CustomText {
                            text: qsTr("0")
                            Layout.fillWidth: true
                        },

                        CustomText {
                            text: qsTr("Initialized")
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                        },
                        CustomText {
                            text: qsTr("yes")
                            Layout.fillWidth: true
                        }
                    ]
                }
                //  an empty Item to fullfill the space when the height of `formLayout` exceeds `column.maxFormLayoutHeight`
                Item {
                    height: column.height > column.maxFormLayoutHeight ? column.height - column.maxFormLayoutHeight : 0
                    width: column.width
                }
            }
        }
    }

}
