import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.4
import QtQuick 2.7
import QtQuick.Layouts 1.3

MenuStyle {
    font { family: qsTr("微软雅黑"); pixelSize: 15; capitalization: Font.MixedCase }

    itemDelegate.background: Rectangle { color:  '#F0F0F0' }
//    itemDelegate.label: Label {
//        id: label
//        text: styleData.text
//        font { family: qsTr("微软雅黑"); pixelSize: 15; capitalization: Font.MixedCase }
//        width: 60
//    }

//    itemDelegate.submenuIndicator:Rectangle {
//        width: 30;
//        height: 30;
//        border.width: 1;
//        radius: 8;
//        color:"red"
//    }
}
