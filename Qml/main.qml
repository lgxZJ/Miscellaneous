import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtQuick.Extras 1.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtCharts 2.1

import lgxZJ 1.0

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Personal data monitor")

    SqlDb {
        id:sql
    }

    ColumnLayout {
        id: columnLayout
        width:calendar.width
        anchors.verticalCenter: parent.verticalCenter

        Calendar {
            id: calendar

            onClicked: {
                chartView.changeToDate(date.getFullYear(), date.getMonth() + 1, date.getDate())
            }
        }

        TextField {
            id: weightTextField

            enabled: addWeightButton.enabled
            placeholderText: qsTr("Your weight(kg) : 65.0")

            anchors.left: addWeightButton.left
            anchors.right: addWeightButton.right
        }

        Button {
            id: addWeightButton
            text: qsTr("Add today's weight")

            anchors.left: calendar.left
            anchors.right: calendar.right

            enabled: enabledBinding()

            onClicked: {
                sql.addWeight(new Date().getFullYear(), new Date().getMonth() + 1, new Date().getDate(), weightTextField.text)
                enabled = Qt.binding(enabledBinding)
            }

            function enabledBinding() {
                return !sql.containsDataOfDay(new Date().getFullYear(), new Date().getMonth() + 1, new Date().getDate())
            }
        }
    }

    ChartView {
        id: chartView
        x: 105
        y: 0
        title: qsTr("Data Line Chart")

        width: parent.width - columnLayout.width
        height: parent.height
        anchors.left: columnLayout.right

        function changeToDate(year, month, day)  {
            //  month is 1-based index
            //  todo:
            console.log(year, month, day)
        }

        axes: [
            ValueAxis {
                id: yAxis
                min: 65.0
                max: 80.0
                tickCount: 15
            },

            DateTimeAxis {
                id: xAxis

                format: 'd'
                min: {
                    var date = new Date
                    date.setDate(1)
                    return date
                }
                max: {
                    var date = new Date
                    date.setDate(getDaysInMonth())
                    return date
                }
                tickCount: getDaysInMonth()

                function getDaysInMonth() {
                    var date = new Date()
                    return (new Date(date.getFullYear(), date.getMonth() + 1, 0)).getDate()
                }
            }
        ]

        Component.onCompleted: {
            var series = chartView.createSeries(ChartView.SeriesTypeLine, "Weight", xAxis, yAxis);
            series.pointsVisible = true;
            series.color = Qt.rgba(Math.random(),Math.random(),Math.random(),1);
            series.hovered.connect(function(point, state){ console.log(point); }); // connect onHovered signal to a function

            var currentDate = new Date()
            var weights = sql.getMonthWeight(currentDate.getFullYear(), currentDate.getMonth() + 1)
            for (var prop in weights) {

                currentDate.setDate(prop)
                series.append(currentDate.getTime(), weights[prop])

                console.log("Object item:", prop, "=", weights[prop])
            }
        }

//        LineSeries {
//            id:lineSeries
//            name: "Weight"

//            axisX: xAxis
//            axisY: yAxis

//            Component.onCompleted: {

//            }

//            function dateDecreaseFromNow(decreaseDay) {
//                var currentDate = new Date()
//                currentDate.setDate(currentDate.getDate() - decreaseDay)

//                console.log(currentDate)
//                return currentDate
//            }
//        }
    }
}
