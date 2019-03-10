import QtQuick 2.0

Image {
    signal click()
    height: 78
//    color: "#448AFF"
    width: 78
    visible: true
    source: "qrc:/mac_forms/login_bg.png"
    Rectangle{
        id: btn
        x:6
        y:6
        radius: 65
        width: 66
        height: 66
        color: "#448AFF"
        MouseArea{
            x: 25
            anchors.fill: parent
            onClicked: {
                emit: click()
                btn.color = "#448AFF"
            }
            onExited: {
                btn.color = "#448AFF"
            }

            onPressed: {
                btn.color = "#424dff"
            }
        }
    }
    Image {
        x: 28
        y: 25
        width: 30
        height: 30
        source: "qrc:/mac_forms/login_logo.png"
    }
}
