import QtQuick 2.0

Rectangle {
    id: btn
    signal click()
    height: 56
    color: "#448AFF"
    width: 55
    radius: 100
    visible: true

    Image {
        x: 16
        y: 14
        width: 29
        height: 26
        source: "qrc:/mac_forms/login_logo.png"
    }

    MouseArea{
        anchors.fill: parent
        onClicked: {
            emit: click()
            btn.color = "#448AFF"
        }
        onExited: {
            btn.color = "#448AFF"
        }

        onPressed: {
            btn.color = "#424dff"
        }
    }
}
