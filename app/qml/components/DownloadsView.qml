import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ListView {
    highlightMoveDuration: 0
    currentIndex: 0

    Action {
        id: startDownload
        objectName: "startDownload"
        text: qsTr("Start download")
    }

    Action {
        id: pauseDownload
        objectName: "pauseDownload"
        text: qsTr("Pause download")
        enabled: false
    }

    Action {
        id: stopDownload
        objectName: "stopDownload"
        text: qsTr("Stop download")
    }

    Action {
        id: resumeDownload
        objectName: "resumeDownload"
        text: qsTr("Resume download")
        enabled: false
    }

    delegate: Item {
        readonly property int linkStatus: sourceStatus

        id: row
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                rootStackFocus.focus = true
                row.ListView.view.currentIndex = index
                if (mouse.button & Qt.RightButton) {
                    contextMenu.popup()
                }
            }

            Menu {
                id: contextMenu

                MenuItem {
                    action: startDownload
                    enabled: {
                        switch (row.linkStatus) {
                        case 0:
                        case 3:
                            return true
                        case 1:
                        case 2:
                        case 4:
                            return false
                        }
                    }
                }

                MenuItem {
                    action: pauseDownload
                }

                MenuItem {
                    action: stopDownload
                    enabled: {
                        switch (row.linkStatus) {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                            return false
                        case 4:
                            return true
                        }
                    }
                }

                MenuItem {
                    action: resumeDownload
                }
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 20
            spacing: 10

            Column {
                Layout.fillWidth: true

                Label {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: display
                    elide: Text.ElideMiddle
                }

                Label {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    text: { root.sourceStatusText(row.linkStatus) }
                    elide: Text.ElideRight
                }
            }

            ProgressBar {
                value: downloadProgress
            }
        }
    }

    highlight: Rectangle {
        color: root.palette.highlight
    }

    ScrollBar.vertical: ScrollBar { }
}
