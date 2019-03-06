import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

import QtQuick.Controls.Material 2.3
import Qt.labs.platform 1.0

Item {

    property int dailTime: 0

    signal stopConnect()

    function getVersion (msg) {
        title.text = "NDR 东软校园网络认证 ver"+msg.toString()
    }



    function doMinimized() {
        flags = Qt.Window | Qt.WindowFullscreenButtonHint | Qt.CustomizeWindowHint | Qt.WindowMinimizeButtonHint
        visibility = Window.Minimized
    }



    Rectangle {
        id: mainPanel
        x: 0
        y: 33
        width: 350
        height: 467
        color: '#FFFFFF'
        visible: true

        Image {
            id: image2
            x: 125
            y: 196
            width: 100
            height: 100
            source: "qrc:/qmlforms/moon.png"
        }







}
