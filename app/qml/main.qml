import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

import "components"

ApplicationWindow {
    id: root
    objectName: "root"
    visible: true
    width: 840
    height: 600
    title: "VKGet"

    Action {
        id: urlToClipboard
        objectName: "urlToClipboard"
        enabled: false
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
                KeyNavigation.tab: previousMenuButton
            }

            ToolButton {
                id: previousMenuButton
                icon.name: "go-previous"
                icon.source: "icons/breeze/actions/24/go-previous.svg"
                enabled: { navigationLog.hasPrevious }
                action: Action {
                    objectName: "previousAction"
                }
                KeyNavigation.tab: nextMenuButton
            }

            ToolButton {
                id: nextMenuButton
                icon.name: "go-next"
                icon.source: "icons/breeze/actions/24/go-next.svg"
                enabled: { navigationLog.hasNext }
                action: Action {
                    objectName: "nextAction"
                }
                KeyNavigation.tab: refreshMenuButton
            }

            ToolButton {
                id: refreshMenuButton
                icon.name: "view-refresh"
                icon.source: "icons/breeze/actions/24/view-refresh.svg"
                action: Action {
                    objectName: "refreshAction"
                }
                KeyNavigation.tab: homeMenuButton
            }

            ToolButton {
                id: homeMenuButton
                icon.name: "go-home"
                icon.source: "icons/breeze/actions/24/go-home.svg"
                action: Action {
                    objectName: "homeAction"
                }
                KeyNavigation.tab: loginMenuButton
            }

            TextField {
                width: contentItem.width
                placeholderText: qsTr("Search")
                Layout.fillWidth: true
                enabled: false
            }

            Switch {
                text: "Global"
                enabled: false
            }

            ToolButton {
                id: loginMenuButton
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
            rootStackFocus.focus = true
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
                Layout.fillWidth: true
                Layout.fillHeight: true

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

            Text {
                text: "DOWNLOADS"
                font: root.font
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            Text {
                text: "SETTINGS"
                font: root.font
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
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
