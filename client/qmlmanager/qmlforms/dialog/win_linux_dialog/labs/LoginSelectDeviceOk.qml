import QtQuick 2.0

Rectangle {
    id: btn
    width: 90
    height: 40
    color: "#448aff"
    radius: 55

    signal okClicked()

    Text {
        x: 32
        y: 11
        width: 27
        height: 18
        color: "#ffffff"
        text: qsTr("确定")
        font.pixelSize: 15
    }
    MouseArea {
        anchors.fill: parent
        onPressed: {
            btn.color = "#424dff"
        }
        onExited: {
            btn.color = "#448aff"
        }

        onClicked: {
            btn.color = "#448aff"
            emit: okClicked()
        }
    }
}
