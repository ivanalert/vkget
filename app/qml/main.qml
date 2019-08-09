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
                    enabled: { navigation.currentIndex < 4 }
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
                KeyNavigation.tab: loginMenuButton

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

            Switch {
                text: "Search"
                enabled: false
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

        Keys.onPressed: {
            if (event.key === Qt.Key_F && (event.modifiers & Qt.ControlModifier))
                filter.forceActiveFocus()
        }

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
                    text: { qsTr("Friends ") + navigation.userName }
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
                    text: { qsTr("Groups ") + navigation.userName }
                }
            }

            AudiosView {
                id: audiosView
                objectName: "audiosView"
                onVisibleChanged: { focus = visible }
                model: audios

                header: Text {
                    width: contentItem.width
                    font.bold: true
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: { qsTr("Audios ") + navigation.userName }
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

                header: Text {
                    width: contentItem.width
                    font.bold: true
                    font.pixelSize: 20
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Downloads")
                }
            }

            Column {
                topPadding: 10

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
                        folder: "file://" + downloadManager.path;

                        onAccepted: {
                            //Remove scheme.
                            downloadManager.path = fileUrl.toString().slice(7);
                        }
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
