import QtQuick 2.0

Item {
    width: 400
    height: 300

    Text {
        id: textEdit
        x: 196
        y: 43
        width: 100
        height: 20
        text: "233333"
        font.italic: true
        font.family: "Times New Roman"
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle {
        x: 122
        y: 82
        width: 200
        height: 200
        MouseArea {
            width: 200
            height: 200
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
            onPressed: {
                textEdit.text = "Pressed"
            }
            onReleased: {
                textEdit.text = "Release"
            }
        }
    }
}
