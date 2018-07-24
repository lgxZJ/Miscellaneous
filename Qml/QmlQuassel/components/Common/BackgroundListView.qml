import QtQuick 2.7

import "./"

Rectangle {
    id: backgroundListView

    property color  bkColor: cppThemeHolderObject.listView.bkColor
    property int    borderWidth: 1
    property color  borderColor: cppThemeHolderObject.listView.borderColor

    /////////////////////////////////////////////////////////////////////////////
    //  NOTE: Expose the inner ListView to the publics. We cannot use `children`
    //  or `visibleChildren` properties as the value of 'listView' property, Qt's
    //  documentation says nothing and it really can't.
    property alias listView: innerListView

    color: bkColor
    border.width: borderWidth
    border.color: borderColor

    //  NOTE:   We need the list view to be in selected state when there is
    //  at least one item, other controls related to this BackgroundListView
    //  my have properties binded to it which depends on this prerequisite.
    //  Fortunately this is the default behaviour of the ListView quick control.
    ListView {
        id: innerListView
        anchors.fill : parent
        //  one pixel for the Rectangle border, another for the real margin
        anchors.margins: 2

        //  todo: make it non-flickable
        contentWidth: width
        contentHeight: height

        flickDeceleration: Flickable.AutoFlickIfNeeded

        property color selectedBkColor: cppThemeHolderObject.listView.selectedBkColor
        property color selectedBorderColor: cppThemeHolderObject.listView.selectedBorderColor

        highlight: Rectangle {
            //  STRANGE: here we donot need to use `height` and `width` properties,
            //  but delegate need.

            border.width: 1
            border.color: innerListView.selectedBorderColor

            color: innerListView.selectedBkColor
        }

        model: ListModel {
            ListElement {
                //  @disable-check M16
                name: qsTr("Default element1")
            }
            ListElement {
                //  @disable-check M16
                name: qsTr("Default element2")
            }
        }

        delegate: Rectangle {
            id: delegateRectangle

            color: "transparent"
            border.width: 1
            border.color: "transparent"

            y: index * 20
            width: innerListView.width
            height: 20  //  todo: change fixed height, height = text.height + 2, but when contains
                        //  icons make the icon decide the height(they are often heigher than texts)

            CustomText {
                anchors.fill: parent
                anchors.margins: 3

                //  todo: use model data provider
                text: name
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    innerListView.currentIndex = index

                    parent.border.color = innerListView.selectedBorderColor
                    parent.color = innerListView.selectedBkColor
                }
                onEntered: {
                    //  when already clicked, we donot update the colors
                    if (delegateRectangle.ListView.isCurrentItem)
                        return;

                    parent.border.color = cppThemeHolderObject.listView.enteredBorderColor
                    parent.color = cppThemeHolderObject.listView.enteredBkColor
                }
                onExited:  {
                    //  NOTE: The `hightlight` property make sure that the selected
                    //  item is always lighlighted even when we change that item's
                    //  color to a different value !

                    parent.color = "transparent"
                    parent.border.color = "transparent"
                }
            }
        }
    }
}
