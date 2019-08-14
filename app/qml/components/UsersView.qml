import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

GridView {
    id: gridView
    enum Status {
        Null,
        Loading,
        Ready,
        Error
    } property int status: UsersView.Null
    property alias error: error.text
    signal itemRequested

    cellWidth: 120
    cellHeight: 130
    highlightMoveDuration: 0
    currentIndex: 0

    onStatusChanged: {
        switch (status) {
        case UsersView.Loading:
            busy.running = true
            error.visible = false
            break
        case UsersView.Ready:
            busy.running = false
            error.visible = false
            break
        case UsersView.Error:
            busy.running = false
            error.visible = true
            break
        default:
            busy.running = false
            error.visible = false
        }
    }

    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_Home:
            positionViewAtBeginning()
            currentIndex = 0
            break
        case Qt.Key_End:
            positionViewAtEnd()
            currentIndex = count - 1
            break
        }
    }

    delegate: Item {
        id: cell
        width: 120
        height: 130
        Keys.onReturnPressed: { GridView.view.itemRequested() }

        Column {
            anchors.fill: parent
            padding: 10

            Image {
                width: 100
                height: 100
                sourceSize.width: 100
                sourceSize.height: 100
                cache: false
                source: decoration
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                width: 100
                text: display
                elide: Text.ElideRight
                font: root.font
                horizontalAlignment: Text.AlignHCenter
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                rootStackFocus.focus = true
                cell.GridView.view.currentIndex = index
            }
            onDoubleClicked: {
                cell.GridView.view.itemRequested()
            }
        }
    }

    highlight: Rectangle {
        radius: 10
        color: root.palette.highlight
    }

    ScrollBar.vertical: ScrollBar { }

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        running: false
    }

    Text {
        id: error
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: false
        font: root.font
    }
}
