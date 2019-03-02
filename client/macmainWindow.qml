import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.3
import Qt.labs.platform 1.0

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"

    x: (Screen.desktopAvailableWidth - width) / 2
    y: (Screen.desktopAvailableHeight - height) / 2
    visible: true

    flags:  Qt.FramelessWindowHint

    height: 500
    color: "#ffffff"
    width: 350

    property int xmouse: 0
    property int ymouse: 0

    property int dailTime: 0

    signal stopConnect()

    function getVersion (msg) {
        title.text = "NDR 东软校园网络认证 ver"+msg.toString()
    }

    function addTime() {
        var hour = Math.floor(dailTime / 3600)
        var min = Math.floor((dailTime % 3600) / 60)
        var sec = dailTime % 60
        var sHour = hour < 10 ? ("0" + hour.toString()) : hour.toString()
        var sMin = min < 10 ? ("0" + min.toString()) : min.toString()
        var sSec = sec < 10 ? ("0" + sec.toString()) : sec.toString()
        time.text = sHour + ":" + sMin + ":" + sSec
        console.log (dailTime % 3600)
    }

    function doMinimized() {
        flags = Qt.Window | Qt.WindowFullscreenButtonHint | Qt.CustomizeWindowHint | Qt.WindowMinimizeButtonHint
        visibility = Window.Minimized
    }

    Timer {
        objectName: "startTime"
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            dailTime = dailTime + 1
            addTime()
        }
    }

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: parent.width
        height: 35
        color: "#448aff"

        Text {
            id: title
            x: 8
            y: 10
            color: "#ffffff"
            text: qsTr("Text")
            lineHeight: 0.9
            font.pixelSize: 14
        }

        Image {
            id: image
            x: 293
            y: 22
            width: 15
            height: 3
            source: "qrc:/qmlforms/minmax.png"
        }

        Image {
            id: image4
            x: 270
            y: 13
            width: 15
            height: 15
            source: "qrc:/qmlforms/setting.png"
        }

        Image {
            id: image1
            x: 318
            y: 11
            width: 15
            height: 15
            source: "qrc:/qmlforms/close.png"
        }

        MouseArea{
            id: mainWinMouseArea
            anchors.fill: parent

            onPressed: {
                xmouse = mouseX
                ymouse = mouseY
            }

            onPositionChanged: {
                mainWindow.x = mainWindow.x + (mouseX - xmouse)
                mainWindow.y = mainWindow.y + (mouseY - ymouse)
            }

            onClicked: {
                var x = mouseX
                var y = mouseY
            }
        }

        Rectangle {
            id: minmax
            x: 293
            y: 11
            width: 15
            height: 15
            color: "#ffffff"
            opacity: 0
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    image.source = "qrc:/qmlforms/style/minmax.png"
                }
                onExited: {
                    image.source = "qrc:/qmlforms/minmax.png"
                }
                onClicked: {
                    doMinimized()
                }
            }
        }
        Rectangle {
            id: close
            x: 318
            y: 11
            width: 15
            height: 15
            color: "#ffffff"
            opacity: 0
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    image1.source = "qrc:/qmlforms/style/close.png"
                }
                onExited: {
                    image1.source = "qrc:/qmlforms/close.png"
                }
                onClicked: {
                    doMinimized()
                }
            }
        }
        Rectangle {
            id: setting
            x: 270
            y: 11
            width: 15
            height: 15
            color: "#ffffff"
            opacity: 0
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                    image4.source = "qrc:/qmlforms/style/setting.png"
                }
                onExited: {
                    image4.source = "qrc:/qmlforms/setting.png"
                }
                onClicked: {
                    if (panel1.visible === true){
                        panel1.visible = false
                        panel2.visible = true
                    }
                    else {
                        panel2.visible = false
                        panel1.visible = true
                    }
                }
            }
        }

    }

    SystemTrayIcon {
        visible: true
        iconSource: "qrc:/qmlforms/logo.png"

        onActivated: {
            window.show()
            window.raise()
            window.requestActivate()
            if(reason.active === SystemTrayIcon.Trigger) {
                console.log("123")
            }
        }
        menu: Menu{
            MenuItem{
                text: "退出"
                onTriggered: {
                    Qt.quit()
                }
            }
        }
    }



    Rectangle {
        id: panel1
        x: 0
        y: 33
        width: 350
        height: 467
        color: "#ffffff"
        visible: true

        Image {
            id: image2
            x: 125
            y: 196
            width: 100
            height: 100
            source: "qrc:/qmlforms/moon.png"
        }

        Rectangle {
            id: rectangle1
            x: 0
            y: 1
            width: 350
            height: 158
            color: "#ffffff"

            Image {
                id: image3
                x: 40
                y: 40
                width: 50
                height: 50
                source: "qrc:/qmlforms/user.png"
            }

            Text {
                id: username
                objectName: "userName"
                x: 130
                y: 40
                width: 120
                height: 20
                text: qsTr("Text")
                font.pixelSize: 14
            }

            Text {
                id: net
                objectName: "net"
                x: 130
                y: 65
                width: 120
                height: 20
                text: qsTr("Text")
                font.pixelSize: 14
            }

            Text {
                id: ip
                objectName: "ipText"
                x: 130
                y: 90
                width: 120
                height: 20
                text: qsTr("Text")
                font.pixelSize: 14
            }

            Text {
                id: time
                objectName: "time"
                x: 131
                y: 115
                width: 120
                height: 16
                text: qsTr("Text")
                font.pixelSize: 12
            }

            Rectangle {
                id: rectangle2
                x: 241
                y: 113
                width: 93
                height: 28
                color: "#448aff"
                radius: 15

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        rectangle2.color = "#424dff"
                    }
                    onClicked: {
                        rectangle2.color = "#448aff"
                        emit: stopConnect()
                    }
                }

                Text {
                    id: text2
                    x: 20
                    y: 6
                    color: "#ffffff"
                    text: qsTr("断开网络")
                    font.family: "Arial"
                    font.pixelSize: 12
                }
            }
        }

        Text {
            id: text1
            x: 144
            y: 319
            color: "#adadad"
            text: qsTr("加载中...")
            font.pixelSize: 16
        }
    }


    Rectangle {
        id: panel2
        x: 0
        y: 35
        width: 350
        height: 465
        color: "#ffffff"
        visible: false

        Rectangle {
            id: rectangle3
            x: 0
            y: 0
            width: 350
            height: 44
            color: "#5796ff"

            Image {
                id: image5
                x: 20
                y: 13
                width: 17
                height: 17
                source: "qrc:/qmlforms/return.png"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        image5.source = "qrc:/qmlforms/style/return.png"
                    }
                    onExited: {
                        image5.source = "qrc:/qmlforms/return.png"
                    }

                    onClicked: {
                        panel1.visible = true
                        panel2.visible = false
                    }
                }
            }

            Text {
                id: text3
                x: 59
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


}







/*##^## Designer {
    D{i:24;invisible:true}
}
 ##^##*/
