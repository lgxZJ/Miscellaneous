import QtQuick 2.7

import "./"

import "../Common/Common.js" as Common

CustomText {
    id: root

    onEnabledChanged: {
        if (enabled)    root.color = cppThemeHolderObject.originTextColor
        else            root.color = cppThemeHolderObject.grayedTextColor
    }
}
