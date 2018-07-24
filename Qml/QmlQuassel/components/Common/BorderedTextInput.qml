import QtQuick 2.7

//  WARNING:
//      This custome compoment lacks a proper anchor width and height which
//      is not used by now. Fix it in the future.
Rectangle {
    property string textt: qsTr("")
    property int echoModee: TextInput.Normal

    implicitWidth: 60
    implicitHeight: 20

    //  default gray color with one-pixel border
    border.color: "#ABADB3"
    border.width: 1

    TextInput {
        id: innerInput
        text: parent.textt

        //  add paddings outside of TextInput to make it looks like a normal QLineEdit
        padding: 2
        x: 2
        y: 2
        width: parent.width - 4
        height: parent.height - 4

        echoMode: parent.echoModee
        selectByMouse: true
    }
}
