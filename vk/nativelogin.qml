import QtQuick 2.12
import QtQuick.Window 2.12
import QtWebView 1.1

Window {
    id: root
    objectName: "dialogWindow"
    title: qsTr("VKontakte")
    modality: Qt.ApplicationModal
    flags: Qt.Dialog
    visible: true
    width: 650
    height: 500

    WebView {
        id: webView
        objectName: "webView"
        anchors.fill: parent
    }
}
