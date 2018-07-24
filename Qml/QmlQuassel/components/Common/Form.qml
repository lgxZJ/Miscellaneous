import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick 2.7

/////////////////////////////////////////////////////////////////////////////
//  NOTE:
//      Form is similar to FormLayout except it respect children's size like
//      Row and Column. Seems cannot use Row as the outest wrapper here, it
//      decides its size from children's size, but the `height` property is
//      incorrect.
RowLayout {
    property alias labelBuddyPairs : grid.children
    property alias spacing: grid.spacing

    Component.onCompleted: {
        if (labelBuddyPairs.length % 2 != 0)
            throw "count of `labelBuddyPairs` is not even!";
    }

    Grid {
        Layout.fillWidth: true

        id: grid
        columns: 2

        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignHCenter
    }
}
