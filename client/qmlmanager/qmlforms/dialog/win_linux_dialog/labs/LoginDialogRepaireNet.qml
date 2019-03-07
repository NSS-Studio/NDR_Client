import QtQuick 2.0

Text {

    signal click()

    id: text
    x: 12
    y: 342
    width: 66
    height: 19
    color: "#7c6e6e"
    text: qsTr("修复网络")
    font.underline: false
    font.pixelSize: 14

    MouseArea{
        hoverEnabled: true

        anchors.fill: parent

        onClicked: {
            emit: click()
        }

        onEntered: {
            text.font.underline = true
        }
        onExited: {
            text.font.underline = false
        }
    }
}
