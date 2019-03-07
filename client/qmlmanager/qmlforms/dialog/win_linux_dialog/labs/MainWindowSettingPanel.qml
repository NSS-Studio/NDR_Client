import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {

    Rectangle {
        id: settingBar
        width: parent.width
        height: 45
        color: "#5796ff"

        Text {
            id: text3
            x: 10
            y: 11
            color: "#ffffff"
            text: qsTr("设置")
            font.pixelSize: 16
        }
    }

    CheckBox {
        id: checkBox
        x: 45
        y: 78
        text: qsTr("登录之后自动拨号")
    }

    CheckBox {
        id: checkBox1
        x: 45
        y: 114
        text: qsTr("断线后自动重拨")
    }

    Rectangle {
        id: rectangle4
        x: 30
        y: 260
        width: 92
        height: 28
        color: "#ffffff"
        border.color: "#eaebed"
        radius: 5

        Text {
            id: text4
            x: 22
            y: 6
            text: qsTr("检查更新")
            font.pixelSize: 12
        }
        MouseArea {
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
            onClicked: {
                rectangle4.color = "#ffffff"
            }
            onPressed: {
                rectangle4.color = "#5796ff"
            }
        }
    }

    Rectangle {
        id: rectangle5
        x: 188
        y: 260
        width: 92
        height: 28
        color: "#ffffff"
        radius: 5
        Text {
            id: text5
            x: 10
            y: 6
            text: qsTr("访问NDR网站")
            font.pixelSize: 12
        }
        border.color: "#eaebed"
        MouseArea {
            anchors.fill: parent
            onClicked: {
                rectangle5.color = "#ffffff"
            }
            onPressed: {
                rectangle5.color = "#5796ff"
            }
        }
    }

    Text {
        id: text6
        x: 19
        y: 376
        width: 187
        height: 16
        color: "#aeaeb0"
        text: "NDR 东软校园网络认证客户端      @大连东软信息学院网络安全工作室 Neusoft Network Security Studio Created by: Wenner"
        font.pixelSize: 12
        wrapMode: Text.WrapAnywhere
    }

    CheckBox {
        id: checkBox2
        x: 45
        y: 149
        text: qsTr("自动关闭通知窗口")
    }

    CheckBox {
        id: checkBox3
        x: 45
        y: 185
        text: qsTr("随系统启动")
    }
}
