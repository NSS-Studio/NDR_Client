import QtQuick 2.0

Rectangle {

    signal moreHelp()
    signal okClicked()

    width: parent.width
    height: parent.height
    color: "#ffffff"
    opacity: 0.6

    Image {
        id: error_img
        x: 84
        y: 45
        width: 50
        height: 50
        visible: true
        source: "qrc:/mac_forms/field.png"
    }

    Text {
        id: text3
        x: 179
        y: 45
        text: qsTr("拨号失败!")
        font.pixelSize: 15
    }

    Text {
        id: error_651
        objectName: "errorDescribe"
        x: 179
        y: 84
        width: 266
        height: 16
        color: "#999999"
        text: qsTr("error describe")
        font.pixelSize: 12
        wrapMode: Text.WrapAnywhere
    }

    Rectangle {
        id: ok_btn
        x: 327
        y: 137
        width: 78
        radius: 15
        height: 31
        color: "#448AFF"

        Text {
            id: text1
            x: 31
            y: 8
            color: "#ffffff"
            text: qsTr("OK")
            font.pixelSize: 12
        }
        MouseArea {
            anchors.fill: parent
            onPressed: {
                ok_btn.color = "#424dff"
            }
            onClicked: {
                ok_btn.color = "#448AFF"
                emit: okClicked()
            }
            onExited: {
                ok_btn.color = "#448AFF"
            }
        }
    }

    Text {
        id: text2
        x: 55
        y: 145
        width: 66
        height: 23
        color: "#0b82f9"
        text: qsTr("更多帮助")
        font.pixelSize: 16
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                text2.font.underline = true
            }
            onExited: {
                text2.font.underline = false
            }
            onClicked: {
                emit: moreHelp()
            }
        }
    }
}
