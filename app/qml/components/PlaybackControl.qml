import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

RowLayout {
    property alias backwardButton: previousButton
    property alias playPauseButton: playPauseButton
    property alias forwardButton: nextButton
    property alias title: playingTitle.text
    property alias currentDuration: currentDuration.text
    property alias duration: duration.text

    Button {
        id: previousButton
        icon.name: "media-skip-backward"
        icon.source: "qrc:/icons/breeze/actions/32/media-skip-backward.svg"
        enabled: { audioPlaylist.previous > -1 }
        KeyNavigation.tab: playPauseButton
    }

    Button {
        id: playPauseButton
        icon.name: {
            if (audioPlayer.playbackState === Audio.PlayingState)
                return "media-playback-pause"
            else
                return "media-playback-start"
        }
        icon.source: {
            if (audioPlayer.playbackState === Audio.PlayingState)
                return "qrc:/icons/breeze/actions/32/media-playback-pause.svg"
            else
                return "qrc:/icons/breeze/actions/32/media-playback-start.svg"
        }
        enabled: { audioPlaylist.current > -1 }
        KeyNavigation.tab: nextButton
    }

    Button {
        id: nextButton
        icon.name: "media-skip-forward"
        icon.source: "qrc:/icons/breeze/actions/32/media-skip-forward.svg"
        enabled: { audioPlaylist.next > -1 }
        KeyNavigation.tab: seekSlider
    }

    Column {
        Layout.fillWidth: true

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                id: currentDuration
            }

            Label {
                id: playingTitle
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideMiddle
                text: qsTr("Nothing plays")
                Layout.fillWidth: true
            }

            Label {
                id: duration
            }
        }

        Slider {
            id: seekSlider
            anchors.left: parent.left
            anchors.right: parent.right
            enabled: { audioPlayer.status === Audio.Buffered }
            from: 0
            to: audioPlayer.duration
            KeyNavigation.tab: rootMenuButton

            onPressedChanged: {
                if (!pressed)
                    audioPlayer.seek(valueAt(position))
            }

            Connections {
                target: audioPlayer
                onPositionChanged: {
                    if (!seekSlider.pressed)
                        seekSlider.value = audioPlayer.position
                }
            }
        }
    }
}
