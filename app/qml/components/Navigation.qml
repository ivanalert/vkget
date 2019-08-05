import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Drawer {
    property alias currentIndex: list.currentIndex
    property alias userName: name.text
    property alias userAvatar: image.source

    ColumnLayout {
        anchors.fill: parent

        Image {
            id: image
            width: 200
            height: 200
            cache: false
        }

        Text {
            id: name
            Layout.fillWidth: true
            font.pixelSize: 20
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ListView {
            id: list
            focus: true
            highlightMoveDuration: 0
            boundsBehavior: Flickable.StopAtBounds
            Layout.fillWidth: true
            Layout.fillHeight: true

            delegate: delegate
            Component {
                id: delegate

                Item {
                    id: item
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 30

                    Text {
                        text: title
                        font.pixelSize: 20
                        leftPadding: 10
                        rightPadding: 10
                        anchors.centerIn: parent
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            item.ListView.view.currentIndex = index
                        }
                    }
                }
            }

            highlight: highlight
            Component {
                id: highlight

                Rectangle {
                    color: root.palette.mid
                }
            }

            model: ListModel {
                ListElement { title: qsTr("Friends") }
                ListElement { title: qsTr("Groups") }
                ListElement { title: qsTr("Music") }
                ListElement { title: qsTr("Video") }
                ListElement { title: qsTr("Downloads") }
                ListElement { title: qsTr("Settings") }
            }
        }
    }
}
