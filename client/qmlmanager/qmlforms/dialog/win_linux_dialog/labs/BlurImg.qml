import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: parent.width; height: parent.height - 150
    Image {
        id: sourceImage
        anchors.fill: parent
        source: "qrc:/mac_forms/BlurImg.png"
        sourceSize: Qt.size(parent.width, parent.height)
        smooth: true
    }

    FastBlur {
        anchors.fill: parent
        source: sourceImage
        radius: 35
    }
}
