import QtQuick 2.12
import QtQuick.Window 2.12
import QtWebEngine 1.8

Window {
    id: root
    objectName: "root"
    title: qsTr("VKontakte")
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    visible: true
    width: 650
    height: 500

    WebEngineView {
        id: webView
        objectName: "webView"
        anchors.fill: parent
    }
}
