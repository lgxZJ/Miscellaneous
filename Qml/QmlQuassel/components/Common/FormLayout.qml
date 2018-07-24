import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick 2.7

/////////////////////////////////////////////////////////////////////////////
//  NOTE:
//      FormLayout is a custome similar to QFormLayout, users have to declare
//  Text and Item pairs by turns. If the count of `labelBuddyPairs` is odd,
//  it will throw an exception.
Item {
    property alias labelBuddyPairs : gridLayout.children

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
