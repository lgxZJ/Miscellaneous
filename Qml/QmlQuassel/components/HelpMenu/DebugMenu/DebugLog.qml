import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick 2.7

Window {
    id: window
    title: qsTr("Debug logs")
    color: cppThemeHolderObject.commonWindowBkColor

    width: 400
    height: 300

    minimumWidth: 150
    minimumHeight: 115

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        TextArea {
            Layout.fillHeight: true
            Layout.fillWidth: true

            //  NOTE:   We cannot simply implement the key navigation functionality,
            //  so it is abandoned.

            wrapMode: TextEdit.WordWrap
            readOnly: true
            textColor: cppThemeHolderObject.commonTextBkColor
        }
        Button {
            Layout.alignment: Qt.AlignCenter

            text: qsTr("Close")

            onClicked: window.close()
        }
    }
}
