import QtQuick 2.0
import QtQuick.Controls 2.5

import "./" as MyStyle

Item {
    width: parent.width
    height: parent.width - 150

    signal okBtnClicked()
    signal clearAllConfig()

    Rectangle {
        anchors.fill: parent
        color: "#FFFFFF"
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        opacity: 0.6
        Text {
            x: 90
            y: 75
            height: 22
            width: 64
            text: qsTr("拨号网卡")
            font.pointSize: 12
        }
        MyStyle.DeviceCombobox {
            objectName: "device_name"
            id: device_name
            x: 166
            y: 67
        }
        Item {
            x: 30
            y: 30
            width: 50
            height: 30
            Image {
                id: return_img
                x: 0
                y: 0
                width: 20
                height: 20
                source: "qrc:/mac_forms/change_forms/return.png"
            }
            Text{
                x: 30
                y: 2
                width: 20
                height: 20
                text: qsTr("返回")
                font.pointSize: 14
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    emit: okBtnClicked()
                }
            }
        }
        CheckBox{
            x: 65
            y: 140
            text: "开机自动启动"
            font.pointSize: 12
        }
        Text {
            id: clear
            x: 330
            y: 150
            height: 40
            color: "#4879f7"
            text: "清空所有配置"
            font.pointSize: 12
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    clear.font.underline = true
                }
                onExited: {
                    clear.font.underline = false
                }
                onClicked: {
                    emit: clearAllConfig()
                }
            }
        }

    }
}
