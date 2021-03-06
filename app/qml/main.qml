import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12
import QtQuick.Dialogs 1.2

import "components"

ApplicationWindow {
    id: root
    objectName: "root"
    visible: true
    width: 840
    height: 600
    title: "VKGet"

    function sourceStatusText(status) {
        switch (status)
        {
        case 0: //VKItem::ReadyStatus
            return qsTr("Ready");
        case 1: //VKItem::LoadingStatus
            return qsTr("Loading...");
        case 2: //VKItem::UnavailableStatus
            return qsTr("Unavailable");
        case 3: //VKItem::NoStatus
            return qsTr("Not processed");
        case 4: //VKItem::DownloadingStatus
            return qsTr("Downloading...");
        case 5: //VKItem::DownloadPausedStatus
            return qsTr("Paused");
        case 6: //VKItem::PendingStatus
            return qsTr("Pending");
        case 7: //VKItem::ErrorStatus
            return qsTr("Error");
        default:
            return ""
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: StandardKey.Find
        onActivated: {
            filter.forceActiveFocus()
            filter.selectAll()
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+1"
        onActivated: {
            navigation.currentIndex = 0
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+2"
        onActivated: {
            navigation.currentIndex = 1
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+3"
        onActivated: {
            navigation.currentIndex = 2
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+4"
        onActivated: {
            navigation.currentIndex = 3
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+5"
        onActivated: {
            navigation.currentIndex = 4
        }
    }

    Shortcut {
        autoRepeat: false
        sequence: "Ctrl+6"
        onActivated: {
            navigation.currentIndex = 5
        }
    }

    menuBar: ToolBar {
        id: toolBar

        RowLayout {
            anchors.fill: parent

            ToolButton {
                id: rootMenuButton
                checkable: true
                icon.name: "application-menu"
                icon.source: "icons/breeze/actions/24/application-menu.svg"
                onToggled: { checked ? navigation.open() : navigation.close() }
                Keys.onReturnPressed: {
                    toggle()
                    navigation.open()
                }
                KeyNavigation.tab: previousMenuButton
            }

            ToolButton {
                id: previousMenuButton
                icon.name: "go-previous"
                icon.source: "icons/breeze/actions/24/go-previous.svg"
                Keys.onReturnPressed: { action.trigger() }
                action: Action {
                    objectName: "previousAction"
                    enabled: { navigationLog.hasPrevious && navigation.currentIndex < 5 }
                    shortcut: StandardKey.Back
                }
                KeyNavigation.tab: nextMenuButton
            }

            ToolButton {
                id: nextMenuButton
                icon.name: "go-next"
                icon.source: "icons/breeze/actions/24/go-next.svg"
                Keys.onReturnPressed: { action.trigger() }
                action: Action {
                    objectName: "nextAction"
                    enabled: { navigationLog.hasNext && navigation.currentIndex < 5 }
                    shortcut: StandardKey.Forward
                }
                KeyNavigation.tab: refreshMenuButton
            }

            ToolButton {
                id: refreshMenuButton
                icon.name: "view-refresh"
                icon.source: "icons/breeze/actions/24/view-refresh.svg"
                Keys.onReturnPressed: { action.trigger() }
                action: Action {
                    objectName: "refreshAction"
                    enabled: { navigation.currentIndex < 3 }
                    shortcut: StandardKey.Refresh
                }
                KeyNavigation.tab: homeMenuButton
            }

            ToolButton {
                id: homeMenuButton
                icon.name: "go-home"
                icon.source: "icons/breeze/actions/24/go-home.svg"
                Keys.onReturnPressed: { action.trigger() }
                action: Action {
                    objectName: "homeAction"
                    enabled: { navigation.currentIndex < 4 }
                    shortcut: "Alt+Home"
                }
                KeyNavigation.tab: filter
            }

            TextField {
                property string friendsFilter
                property string groupsFilter
                property string audiosFilter
                property string downloadsFilter

                id: filter
                objectName: "filter"
                width: contentItem.width
                placeholderText: qsTr("Filter")
                Layout.fillWidth: true
                enabled: { navigation.currentIndex < 5 }
                KeyNavigation.tab: goToUserMenuButton

                onTextChanged: {
                    switch (navigation.currentIndex) {
                    case 0:
                        friendsFilter = text;
                        break;
                    case 1:
                        groupsFilter = text;
                        break;
                    case 2:
                        audiosFilter = text;
                        break;
                    case 4:
                        downloadsFilter = text;
                        break;
                    }
                }

                Button {
                    flat: true
                    icon.name: "edit-clear"
                    icon.source: "icons/breeze/actions/24/edit-clear.svg"
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    width: 40
                    enabled: filter.length

                    onClicked: {
                        filter.clear()
                        filter.textEdited()
                        filter.forceActiveFocus()
                    }
                }

                Connections {
                    target: navigation
                    onCurrentIndexChanged: {
                        switch (navigation.currentIndex) {
                        case 0:
                            filter.text = filter.friendsFilter
                            break;
                        case 1:
                            filter.text = filter.groupsFilter
                            break;
                        case 2:
                            filter.text = filter.audiosFilter
                            break;
                        case 4:
                            filter.text = filter.downloadsFilter
                            break;
                        }
                    }
                }
            }

            /*Switch {
                text: "Search"
                enabled: false
            }*/

            ToolButton {
                id: goToUserMenuButton
                objectName: "goToUserMenuButton"
                text: qsTr("Go to user's page")
                Keys.onReturnPressed: { clicked() }
                KeyNavigation.tab: loginMenuButton
            }

            ToolButton {
                id: loginMenuButton
                Keys.onReturnPressed: { action.trigger() }
                action: Action {
                    objectName: "loginAction"
                    text: "Log in"
                }
                KeyNavigation.tab: rootStackFocus
            }
        }
    }

    Navigation {
        id: navigation
        objectName: "navigation"
        y: menuBar.height
        width: 200
        height: root.height - menuBar.height;
        onClosed: {
            if (rootMenuButton.checked === true)
                rootMenuButton.checked = false
        }
    }

    FocusScope {
        id: rootStackFocus
        focus: true
        anchors.fill: parent
        KeyNavigation.tab: playbackControl.backwardButton

        StackLayout {
            id: rootStack
            objectName: "rootStack"
            anchors.fill: parent
            currentIndex: navigation.currentIndex

            UsersView {
                id: friendsView
                objectName: "friendsView"
                focus: true
                onVisibleChanged: { focus = visible }
                model: friends

                header: Text {
                    width: contentItem.width
                    font.bold: true
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                    text: { navigation.userName + " " + qsTr("Friends") }
                }
            }

            UsersView {
                id: groupsView
                objectName: "groupsView"
                onVisibleChanged: { focus = visible }
                model: groups

                header: Text {
                    width: contentItem.width
                    font.bold: true
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                    text: { navigation.userName + " " + qsTr("Groups") }
                }
            }

            AudiosView {
                id: audiosView
                objectName: "audiosView"
                onVisibleChanged: { focus = visible }
                model: audios

                Action {
                    id: downloadAllAudio
                    objectName: "downloadAllAudio"
                }

                header: RowLayout {
                    width: contentItem.width

                    Row {
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignCenter
                        spacing: 10

                        Text {
                            font.bold: true
                            font.pixelSize: 20
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            verticalAlignment: Text.AlignVCenter
                            text: navigation.userName + " " + qsTr("Audios")
                        }

                        Button {
                            text: qsTr("Download all")
                            enabled: audiosView.count
                            onClicked: {
                                downloadAllAudioDialog.open();
                            }
                        }
                    }
                }

                MessageDialog {
                    id: downloadAllAudioDialog
                    title: "VKGet" + " - " + qsTr("Download")
                    text: qsTr("Download") + " " + audiosView.count + " " + qsTr("files")
                    standardButtons: StandardButton.Ok | StandardButton.Cancel
                    icon: StandardIcon.Question
                    onAccepted: {
                        downloadAllAudio.trigger()
                    }
                }
            }

            Text {
                text: "VIDEO"
                font: root.font
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            DownloadsView {
                id: downloadsView
                objectName: "downloadsView"
                onVisibleChanged: { focus = visible }
                model: downloads

                header: RowLayout {
                    width: contentItem.width

                    Row {
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignCenter
                        spacing: 10

                        Text {
                            font.bold: true
                            font.pixelSize: 20
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            verticalAlignment: Text.AlignVCenter
                            text: qsTr("Downloads")
                        }

                        Button {
                            id: stopAllDownloads
                            objectName: "stopAllDownloads"
                            text: qsTr("Stop all")
                            enabled: downloadManager.downloadCount
                        }

                        Button {
                            id: clearAllDownloads
                            objectName: "clearAllDownloads"
                            text: qsTr("Clear all")
                            enabled: downloadsView.count
                        }
                    }
                }
            }

            Column {
                topPadding: 10
                spacing: 10

                RowLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    spacing: 10

                    Text {
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: root.font.pixelSize
                        text: qsTr("Path for audio downloads:")
                    }

                    TextField {
                        id: downloadsPathText
                        Layout.fillWidth: true
                        readOnly: true
                        text: downloadManager.path
                    }

                    Button {
                        id: downloadFileDialogButton
                        text: "..."
                        onClicked: {
                            downloadFileDialog.open()
                        }
                    }

                    FileDialog {
                        id: downloadFileDialog
                        title: qsTr("Choose directory")
                        selectFolder: true
                        //Add scheme.
                        folder: { "file:" + downloadManager.path }

                        onAccepted: {
                            //Remove scheme.
                            var str = fileUrl.toString().slice(7)
                            //Check for windows path.
                            if (str.charAt(2) === ':')
                                str = str.slice(1)
                            downloadManager.path = str
                        }
                    }
                }

                RowLayout {
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    spacing: 10

                    CheckBox {
                        text: qsTr("Overwite existing files")
                        checked: downloadManager.overwrite
                        onToggled: { downloadManager.overwrite = checked }
                    }
                }
            }
        }
    }

    footer: Pane {
        topPadding: 0
        bottomPadding: 0

        PlaybackControl {
            id: playbackControl
            objectName: "playbackControl"
            anchors.fill: parent
        }
    }

    Audio {
        id: audioPlayer
        objectName: "audioPlayer"
        audioRole: Audio.MusicRole
        autoLoad: false
    }
}
