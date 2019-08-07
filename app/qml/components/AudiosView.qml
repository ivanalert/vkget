import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

ListView {
    enum Status {
        Null,
        Loading,
        Ready,
        Error
    } property int status: AudiosView.Null
    property alias error: error.text

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

    Connections {
        target: playbackControl.backwardButton
        onClicked: { audioPlaylist.moveBackward() }
    }

    Connections {
        target: playbackControl.playPauseButton
        onClicked: {
            if (audioPlayer.playbackState === Audio.PlayingState)
                audioPlayer.pause()
            else
                audioPlayer.play()
        }
    }

    Connections {
        target: playbackControl.forwardButton
        onClicked: { audioPlaylist.moveForward() }
    }

    Connections {
        target: audioPlayer
        onStopped: {
            if (audioPlayer.status === Audio.EndOfMedia && audioPlaylist.next > -1)
                audioPlaylist.moveForward()
        }
    }

    Action {
        id: downloadAudio
        objectName: "downloadAudio"
        text: qsTr("Download")
    }

    Action {
        id: urlToClipboard
        objectName: "urlToClipboard"
    }

    delegate: Rectangle {
        readonly property url link: source
        readonly property int linkStatus: sourceStatus
        property int status: AudiosView.Null

        id: row
        anchors.left: parent.left
        anchors.right: parent.right
        height: 80
        enabled: { linkStatus != 2 }
        color: {
            if (!ListView.isCurrentItem && audioPlaylist.current === index)
                return root.palette.mid
            else
                return "transparent"
        }

        Keys.onReturnPressed: { audioPlaylist.current = ListView.view.currentIndex }

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
            onDoubleClicked: { audioPlaylist.current = row.ListView.view.currentIndex }

            Menu {
                id: contextMenu

                MenuItem { action: downloadAudio }
                MenuItem { action: urlToClipboard }

                onAboutToShow: {
                    var str = qsTr("Copy URL")
                    switch (row.linkStatus) {
                    case 0:
                        downloadAudio.enabled = true
                        urlToClipboard.enabled = true
                        urlToClipboard.text = str
                        break
                    case 1:
                    case 2:
                        downloadAudio.enabled = false
                        urlToClipboard.enabled = false
                        urlToClipboard.text = str
                        break
                    default:
                        downloadAudio.enabled = false
                        urlToClipboard.enabled = true
                        urlToClipboard.text = qsTr("Request URL")
                    }
                }
            }
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 20
            spacing: 10

            Image {
                width: 80
                height: 80
                sourceSize.width: 80
                sourceSize.height: 80
                cache: false
                source: cover
            }

            Column {
                Layout.fillWidth: true

                Label {
                    text: display
                    elide: Text.ElideMiddle
                    anchors.left: parent.left
                    anchors.right: parent.right
                }

                Label {
                    id: statusText
                    text: {
                        switch (row.linkStatus) {
                        case 0:
                            return qsTr("Ready")
                        case 1:
                            return qsTr("Loading...")
                        case 2:
                            return qsTr("Unavailable")
                        default:
                            return qsTr("Not processed")
                        }
                    }
                }
            }

            Label {
                text: metadata
            }
        }
    }

    highlight: Rectangle {
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
