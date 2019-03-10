import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.5

Rectangle {
    width: parent.width; height: parent.height - 150
    Image {
        id: sourceImage
        anchors.fill: parent
        source: "qrc:/mac_forms/BlurImg.png"
        sourceSize: Qt.size(parent.width, parent.height)
        smooth: true
    }
    gradient: Gradient {
            GradientStop { position: 0.0; color: "red" }
            GradientStop { position: 0.33; color: "yellow" }
            GradientStop { position: 1.0; color: "green" }
        }
    FastBlur {
        anchors.fill: parent
        source: sourceImage
        radius: 35
    }
}
