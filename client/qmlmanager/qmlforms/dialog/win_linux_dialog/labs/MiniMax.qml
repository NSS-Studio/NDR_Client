import QtQuick 2.0

Item {
    width: 15
    height: 15

    signal mouseClicked()

    Image {
        id: minmax_img
        width: parent.width
        y: parent.height - 3
        height: 3
        source: "qrc:/mac_forms/minmax.png"
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            minmax_img.source = "qrc:/mac_forms/change_forms/minmax.png"
        }
        onExited: {
            minmax_img.source = "qrc:/mac_forms/minmax.png"
        }
        onClicked: {
            emit: mouseClicked()
        }
    }
}
