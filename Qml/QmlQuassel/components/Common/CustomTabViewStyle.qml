import QtQuick 2.0
import QtQuick.Controls.Styles 1.4

import "./"

TabViewStyle {
    property int horizontalTabTextMargin: 15
    property int verticalTabTextMargin: 5

    tab: Rectangle {
        property int nonSelectedWidth: tabTitle.width + horizontalTabTextMargin * 2
        property int nonSelectedHeight: tabTitle.height + verticalTabTextMargin * 2

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

        CustomText {
            id: tabTitle
            text: styleData.title
            anchors.centerIn: parent
        }

        Rectangle {
            color: "white"

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
        border.color: cppThemeHolderObject.tabView.frameBorderColor

        color: "white"
    }
}
