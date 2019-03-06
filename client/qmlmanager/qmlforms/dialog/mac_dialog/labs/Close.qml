import QtQuick 2.0

Item {
    width: 15
    height: 15

    signal mouseClicked

    Image {
        id: close_img
        anchors.fill: parent
        source: "qrc:/mac_forms/close.png"
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            emit: mouseClicked()
        }
        onEntered: {
               close_img.source = "qrc:/mac_forms/change_forms/close.png"
        }
        onExited: {
            close_img.source = "qrc:/mac_forms/close.png"
        }
    }
}
