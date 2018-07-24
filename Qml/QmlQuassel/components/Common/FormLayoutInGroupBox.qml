import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick 2.7

/////////////////////////////////////////////////////////////////////////////
//  NOTE:
//      FormLayoutInGroupBox wraps a GroupBox outside FormLayout.
GroupBox {
    property alias labelBuddyPairs : gridLayout.children

    title: qsTr("lgxZJ")
    checkable: true
    checked: false

    Component.onCompleted: {
        if (labelBuddyPairs.length % 2 != 0)
            throw "count of `labelBuddyPairs` is not even!";
    }

    GridLayout {
        id: gridLayout
        columns: 2
        anchors.fill: parent

        //  NOTE: Alignment of children hve to be declared separately.
    }
}
