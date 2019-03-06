import QtQuick 2.0

Item {
    width: 15
    height: 15
    signal click()
    Image {
        id: setting_img
        anchors.fill: parent
        source: "qrc:/mac_forms/setting.png"
    }
    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            setting_img.source = "qrc:/mac_forms/change_forms/setting.png"
        }
        onExited: {
            setting_img.source = "qrc:/mac_forms/setting.png"
        }
        onClicked: {
            emit: click()
            console.log("111")
        }
    }
}
