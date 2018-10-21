import QtQuick 2.4
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
//import QtQuick.Controls.Material 2.3
import QtQuick.Extras 1.4

Item {
    width: 340
    height: 260
    property alias rectangle: rectangle

    Image {
        id: image
        x: 0
        y: 0
        //        width: 340
        //        height: 260
        source: "qrc:/qmlforms/image.png"
        //        sourceSize.width: 340
        //        sourceSize.height: 260
        //        clip: true
        fillMode: Image.Pad
    }

    Rectangle {
        id: rectangle
        x: 0
        y: 120
        width: 340
        height: 140
        clip: true
        opacity: 0
        GaussianBlur {
            id: blur
            //        anchors.fill: image
            source: rectangle
            width: 340
            height: source.height
            x: 0
            y: image.height - source.height
            radius: 20
        }
    }
}
