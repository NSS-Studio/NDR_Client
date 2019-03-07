import QtQuick 2.0

Rectangle {
    signal stopConnect()
    Image {
        id: image3
        x: 40
        y: 40
        width: 50
        height: 50
        source: "qrc:/mac_forms/user.png"
    }

    Text {
        id: username
        objectName: "userName"
        x: 130
        y: 40
        width: 120
        height: 20
        text: qsTr("Text")
        font.pixelSize: 14
    }

    Text {
        id: net
        objectName: "net"
        x: 130
        y: 65
        width: 120
        height: 20
        text: qsTr("Text")
        font.pixelSize: 14
    }

    Text {
        id: ip
        objectName: "ipAddress"
        x: 130
        y: 90
        width: 120
        height: 20
        text: qsTr("Text")
        font.pixelSize: 14
    }

    Text {
        id: time
        objectName: "time"
        x: 131
        y: 115
        width: 120
        height: 16
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Rectangle {
        id: rectangle2
        x: 241
        y: 113
        width: 93
        height: 28
        color: "#448aff"
        radius: 15

        MouseArea {
            anchors.fill: parent
            onPressed: {
                rectangle2.color = "#424dff"
            }
            onClicked: {
                rectangle2.color = "#448aff"
                emit: stopConnect()
            }
        }

        Text {
            id: text2
            x: 20
            y: 6
            color: "#ffffff"
            text: qsTr("断开网络")
            font.family: "Arial"
            font.pixelSize: 12
        }
    }
}
