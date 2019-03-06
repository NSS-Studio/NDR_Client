import QtQuick 2.0

Item {
    signal clicke()

    Text {
        id: text
        visible: true
        color: "#0b82f9"
        text: qsTr("选择网卡")
        font.pixelSize: 16
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                emit: clicke()
            }

            onEntered: {
                text.font.underline = true
            }
            onExited: {
                text.font.underline = false
            }
        }
    }
}
