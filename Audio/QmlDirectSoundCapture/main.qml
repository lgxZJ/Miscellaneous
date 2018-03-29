import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3

import lgxZJ 1.0

Window {
    id: window1
    visible: true
    width: 400
    height: 200
    title: qsTr("Hello World")

    CppDsCapture {
        id: dsCapture
    }

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        rows: 20
        columns: 3

        RowLayout {
            id: rowLayout2
            width: 300
            height: 100
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            antialiasing: false
            transformOrigin: Item.Center
            Layout.fillWidth: true
            Layout.columnSpan: 3

            ComboBox {
                id: captureDeviceComboBox
                Layout.minimumHeight: 10
                Layout.minimumWidth: 40
                font.family: "Arial"
                font.bold: false
                font.capitalization: Font.Capitalize
                Layout.fillWidth: true

                model: dsCapture.avaiableDeviceNames
            }

            StatusIndicator {
                id: statusIndicator
                color: "#32f2bc"
                z: 1
                transformOrigin: Item.Center
                Layout.fillHeight: false
                Layout.fillWidth: false
                active: false

                enabled: (captureDeviceComboBox.count == 0) ? false : true
            }

            Switch {
                id: captureSwitch
                text: qsTr("Capture")
                Layout.fillWidth: false

                enabled: (captureDeviceComboBox.count == 0) ? false : true
                onToggled: {
                    statusIndicator.active = checked

                    checked ?
                        startCaptureAudio():
                        dsCapture.closeCurrentDevice();
                }

                function startCaptureAudio() {
                        dsCapture.openDevice(captureDeviceComboBox.currentIndex)
                        dsCapture.startCapture(checkBoxEnableAec.checked ? CppDsCapture.AcousticEchoCancellationMicrosoft : CppDsCapture.None,
                                               checkBoxEnableNs.checked,
                                               checkBoxNoiseFill.checked ? CppDsCapture.NoiseSuppressionMicrosoft : CppDsCapture.None,
                                               "lgxZJ.pcm")
                }
            }




        }

        GroupBox {
            id: groupBox
            padding: 8
            anchors.top: gridLayout.top
            anchors.topMargin: 48
            anchors.bottom: gridLayout.bottom
            anchors.bottomMargin: 0
            anchors.right: gridLayout.right
            anchors.rightMargin: 0
            font.family: "Courier"
            anchors.left: gridLayout.left
            anchors.leftMargin: 0
            title: qsTr("Capture Effects")

            //enabled: (captureDeviceComboBox.count == 0 || captureSwitch.checked) ? false : true
            enabled: false

            GroupBox {
                id: groupBoxAec
                width: 192
                enabled: true
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.top: parent.top
                anchors.right: groupBoxNs.left
                anchors.topMargin: 0
                title: qsTr("Acoustic Echo Cancellation(AEC)")

                Column {
                    anchors.fill: parent

                    CheckBox {
                        id: checkBoxEnableAec
                        text: qsTr("Enabled")
                    }

                    CheckBox {
                        id: checkBoxNoiseFill
                        text: qsTr("Noise Fill")
                    }
                }
            }

            GroupBox {
                id: groupBoxNs
                x: 198
                y: 9
                width: 186
                font.pixelSize: 8
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 0
                anchors.bottomMargin: 0
                title: qsTr("Noise Suppresion(NS)")

                CheckBox {
                    id: checkBoxEnableNs
                    text: qsTr("Enabled")
                    anchors.fill: parent

                    enabled: checkBoxEnableAec.checked
                }
            }
        }
    }
}
