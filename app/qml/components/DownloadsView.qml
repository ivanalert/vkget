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
        enabled: false
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
        enabled: false
    }

    Action {
        id: resumeDownload
        objectName: "resumeDownload"
        text: qsTr("Resume download")
        enabled: false
    }

    delegate: Item {
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
                }

                MenuItem {
                    action: pauseDownload
                }

                MenuItem {
                    action: stopDownload
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
                Label {
                    text: display
                    elide: Text.ElideMiddle
                }

                Label {
                    text: "Here are will be error messages"
                }
            }

            ProgressBar {
                Layout.alignment: Qt.AlignRight
                value: downloadProgress
            }
        }
    }

    highlight: Rectangle {
        color: root.palette.highlight
    }

    ScrollBar.vertical: ScrollBar { }
}
