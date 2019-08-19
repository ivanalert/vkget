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

    function millisecondsToString(msecs) {
        var s = Math.floor(msecs / 1000)
        var m = 0
        var h = 0

        if (s > 60)
        {
            m = Math.floor(s / 60)
            s = Math.floor(s - m * 60)
        }
        if (m > 60)
        {
            h = Math.floor(m / 60)
            m = Math.floor(m - h * 60)
        }

        var result = ""
        if (h !== 0)
        {
            result += h < 10 ? "0" + parseInt(h) : parseInt(h)
            result += ":"
        }
        if (m !== 0 || result.length > 0)
        {
            result += m < 10 ?  "0" + parseInt(m) : parseInt(m)
            result += ":"
        }
        if (s !== 0 || result.length > 0)
        {
            result += s < 10 ? "0" + parseInt(s) : parseInt(s)
        }

        return result
    }

    Button {
        id: previousButton
        icon.name: "media-skip-backward"
        icon.source: "qrc:/icons/breeze/actions/32/media-skip-backward.svg"
        enabled: { audioPlaylist.previous > -1 }
        Keys.onReturnPressed: { clicked() }
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
        Keys.onReturnPressed: { clicked() }
        KeyNavigation.tab: nextButton
    }

    Button {
        id: nextButton
        icon.name: "media-skip-forward"
        icon.source: "qrc:/icons/breeze/actions/32/media-skip-forward.svg"
        enabled: { audioPlaylist.next > -1 }
        Keys.onReturnPressed: { clicked() }
        KeyNavigation.tab: seekSlider
    }

    Column {
        Layout.fillWidth: true

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right

            Label {
                id: currentDuration
                text: {
                    var str = ""
                    var charCount = duration.text.length
                    if (charCount > 0) {
                        str = millisecondsToString(seekSlider.value)
                        if (str.length === 0)
                            str = "00"
                        while (str.length < charCount)
                            str = "00:" + str
                    }
                    return str
                }
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
                text: { millisecondsToString(audioPlayer.duration) }
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
