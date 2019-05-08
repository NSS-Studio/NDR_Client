import QtQuick 2.0

Item {
    width: 15
    height: 15
    property int flag: 0
    signal click()
    Image {
        objectName: "setting_img"
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

            if (flag === 0){
                setting_img.source = "qrc:/mac_forms/setting.png"
            }
        }
        onClicked: {
            emit: click()
            if (flag === 1) {
                flag = 0
                setting_img.source = "qrc:/mac_forms/setting.png"
            }
            else {
                flag = 1
            }
        }
    }
}
