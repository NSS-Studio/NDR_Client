import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.3

ApplicationWindow {
    id: loginDialog
    objectName: "roott"
    width: 500
    height: 370
    x: (Screen.desktopAvailableWidth - width) / 2
    y: (Screen.desktopAvailableHeight - height) / 2
    visible: true

    Material.theme: Material.Dark
    Material.accent: Material.Purple


    flags: Qt.Window | Qt.FramelessWindowHint

    signal login(string username,string passwd,string pack_info,string NIC_info)

    Item {
        id: signal

        //        signal login(String username,String passwd,String pack_info,String NIC_info)

    }

    property int xmouse: 0
    property int ymouse: 0

    function addPost (arg1, arg2){
        mod1.append({
                        "text": arg1.toString(),
                        "data": arg2.toString()
                    })
    }

    function addDevice(arg1) {
        combDeviceMod.append({"wk":arg1})
    }

    function getVersion (msg){
        tittle.text = "NDR 东软校园网络认证 ver"+msg.toString()
        tittle1.text = "NDR 东软校园网络认证 ver"+msg.toString()
    }

    function def_windows() {
        select.visible = false
        status.x = 300
        status.y = 45
    }


    style: ApplicationWindowStyle{
        background: Rectangle {
            anchors.fill: parent
            radius: 100
            Image {
                anchors.fill: parent
                source: "qrc:/qmlforms/image 2.1.png"
            }
        }
    }

    Image {
        anchors.rightMargin: 0
        anchors.fill: parent
        source: "qrc:/qmlforms/image 2.1.png"
    }




    Text {
        id: tittle
        x: 12
        y: 13
        width: 185
        height: 50
        color: "#ffffff"
        text: "NDR 东软校园网络认证 ver0.76"
        font.bold: true
    }



    MouseArea{
        id: rootMouseArea
        x: 0
        y: 0
        height: 50
        width: parent.width

        onPressed: {
            xmouse = mouseX
            ymouse = mouseY
        }

        onPositionChanged: {
            loginDialog.x = loginDialog.x + (mouseX - xmouse)
            loginDialog.y = loginDialog.y + (mouseY - ymouse)
        }

        onClicked: {
            var x = mouseX
            var y = mouseY
        }
    }
    Image {
        id: image6
        x: 475
        y: 11
        width: 15
        height: 15
        source: "qrc:/qmlforms/close.png"
    }

    Rectangle {
        id: close
        x: 475
        y: 11
        width: 15
        height: 15
        color: "#e13a3a"
        opacity: 0
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                image6.source = "qrc:/qmlforms/style/close.png"
            }
            onExited: {
                image6.source = "qrc:/qmlforms/close.png"
            }

            onClicked: {
                Qt.quit()
            }
        }
    }

    Image {
        id: image5
        x: 450
        y: 21
        width: 15
        height: 3
        source: "qrc:/qmlforms/minmax.png"
    }

    Rectangle {
        id: minmax
        x: 450
        y: 11
        width: 15
        height: 15
        color: "#df3333"
        opacity: 0
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                image5.source = "qrc:/qmlforms/style/minmax.png"
            }
            onExited: {
                image5.source = "qrc:/qmlforms/minmax.png"
            }
            onClicked: {
                loginDialog.visibility = Window.Minimized
            }
        }
    }

    Rectangle {
        id: panel1
        x: 0
        y: 150
        width: parent.width
        height: parent.height - 150
        color: "#ffffff"
        visible: true
        border.color: "#000000"
        ComboBox {
            id: combMod
            x: 80
            y: 30
            width: 150
            height: 30
            textRole: "text"
            editable: false

            model: ListModel{
                id: mod1
            }


        }

        ComboBox {
            id: account
            x: 80
            y: 80
            height: 30
            textRole: "history"
            editable: true
            width: 150
            model: ListModel{
                id: accountModel
            }

        }

        TextField {
            id: password
            x: 80
            y: 130
            width: 150
            height: 30
            echoMode: TextInput.Password


        }

        CheckBox {
            id: remPassword
            x: 299
            y: 83
            width: 107
            height: 27
            text: qsTr("记住密码")

            style: CheckBoxStyle{
                indicator: Rectangle {
                    implicitWidth: 16
                    implicitHeight: 16
                    radius: 3
                    border.color: control.activeFocus ? "darkblue" : "gray"
                    border.width: 1
                    Rectangle {
                        visible: control.checked
                        color: "#555"
                        border.color: "#333"
                        radius: 1
                        anchors.margins: 4
                        anchors.fill: parent
                    }
                }
            }
        }

        CheckBox {
            id: autoLogin
            x: 299
            y: 121
            width: 102
            height: 26
            text: qsTr("自动登录")

            style: CheckBoxStyle{
                indicator: Rectangle {
                    implicitWidth: 16
                    implicitHeight: 16
                    radius: 3
                    border.color: control.activeFocus ? "darkblue" : "gray"
                    border.width: 1
                    Rectangle {
                        visible: control.checked
                        color: "#555"
                        border.color: "#333"
                        radius: 1
                        anchors.margins: 4
                        anchors.fill: parent
                    }
                }
            }
        }

        MouseArea{

            onClicked: {}

            onReleased: {}

            onPressed: {}
        }

        Text {
            id: select
            objectName: "selectDevice"
            x: 303
            y: 40
            visible: true
            color: "#0b82f9"
            text: qsTr("选择网卡")
            font.pixelSize: 16
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    panel2.visible = true
                    panel1.visible = false
                }

                onEntered: {
                    select.font.underline = true
                }
                onExited: {
                    select.font.underline = false
                }
            }
        }

        Text {
            id: status
            objectName: "status"
            x: 80
            y: 170
            width: 126
            height: 27
            text: qsTr("")
            font.pixelSize: 12
        }
    }

    Rectangle {
        id: panel2
        x: 0
        y: 150
        width: 500
        height: 220
        color: "#ffffff"
        visible: false

        ComboBox{
            id: combDevice
            x: 135
            y: 43
            height: 32
            width: 193
            textRole: "wk"
            model: ListModel{
                id: combDeviceMod
                objectName: "deviceName"
            }
        }

        Rectangle {
            id: rectangle
            x: 186
            y: 124
            width: 91
            height: 40
            color: "#448aff"
            radius: 55

            Text {
                id: text4
                x: 32
                y: 11
                width: 27
                height: 18
                color: "#ffffff"
                text: qsTr("确定")
                font.pixelSize: 15
            }
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    rectangle.color = "#424dff"
                }
                onClicked: {
                    rectangle.color = "#448aff"
                    panel2.visible = false
                    panel1.visible = true
                }
            }
        }

    }

    Rectangle {
        id: panel3
        x: 0
        y: 150
        width: 500
        height: 221
        color: "#ffffff"
        visible: false

        Image {
            id: image
            x: 161
            y: 60
            width: 36
            height: 26
            source: "qrc:/qmlforms/logining.png"
        }

        Image {
            id: image1
            x: 86
            y: 156
            width: 328
            height: 5
            source: "qrc:/qmlforms/logining_bar.png"
        }

        Text {
            id: loging
            x: 237
            y: 60
            width: 122
            height: 28
            text: qsTr("正在登录网络...")
            font.pixelSize: 18
        }

        Text {
            id: login_status
            x: 196
            y: 112
            color: "#999999"
            text: qsTr("正在尝试第一次拨号")
            font.pixelSize: 12
        }
    }

    Rectangle {
        id: panel4
        x: 0
        y: 150
        width: 500
        height: 220
        color: "#ffffff"
        visible: false

        Image {
            id: error_img
            x: 84
            y: 45
            width: 50
            height: 50
            visible: true
            source: "qrc:/qmlforms/field.png"
        }

        Text {
            id: text3
            x: 179
            y: 45
            text: qsTr("拨号失败! (错误码:651)")
            font.pixelSize: 15
        }

        Text {
            id: error_651
            x: 179
            y: 84
            width: 266
            height: 16
            color: "#999999"
            text: qsTr("651：调制解调器报告了一个错误请检查网络线路是否正常并确保网线或交换机连接正常")
            font.pixelSize: 12
            wrapMode: Text.WrapAnywhere
        }

        Rectangle {
            id: ok_btn
            x: 327
            y: 137
            width: 78
            radius: 15
            height: 31
            color: "#448AFF"

            Text {
                id: text1
                x: 31
                y: 8
                color: "#ffffff"
                text: qsTr("OK")
                font.pixelSize: 12
            }
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    ok_btn.color = "#424dff"
                }
                onClicked: {
                    ok_btn.color = "#448AFF"
                }
            }
        }

        Text {
            id: text2
            x: 55
            y: 145
            width: 66
            height: 23
            color: "#0b82f9"
            text: qsTr("更多帮助")
            font.pixelSize: 16
            MouseArea{
                hoverEnabled: true
                onEntered: {
                    text2.font.underline = true
                }
                onExited: {
                    text2.font.underline = false
                }
            }
        }
    }

    Text {
        id: repaire
        x: 12
        y: 342
        width: 66
        height: 19
        color: "#7c6e6e"
        text: qsTr("修复网络")
        font.underline: false
        font.pixelSize: 14

        MouseArea{
            hoverEnabled: true

            anchors.fill: parent

            onEntered: {
                repaire.font.underline = true
            }
            onExited: {
                repaire.font.underline = false
            }
        }
    }
    Text {
        id: tittle1
        x: 303
        y: 342
        width: 185
        height: 50
        color: "#7c6e6e"
        text: "NDR 东软校园网络认证 ver0.76"
    }

    Rectangle {
        id: login_button
        objectName: "loginButton"
        x: 351
        y: 129
        height: 55
        color: "#448AFF"
        width: 55
        radius: 100
        visible: true
        border.color: "#448AFF"

        Image {
            x: 16
            y: 14
            width: 29
            height: 26
            source: "qrc:/qmlforms/login_logo.png"
        }

        MouseArea{
            id: login_button_mouse
            anchors.fill: parent
            onClicked: {
                login_button.color = "#448AFF"
                if (account.editText === "" || password.text === "") {
                    status.color = "red"
                    status.text = "账号或密码为空!"
                }
                else {
                    login_button.visible = false
                    repaire.visible = false
                    tittle1.visible = false
                    panel1.visible = false
                    panel2.visible = false
                    panel4.visible = false
                    panel3.visible = true
                    emit: loginDialog.login(account.editText,password.text,combMod.currentText,combDevice.currentText)
                }
            }
            onPressed: {
                login_button.color = "#424dff"

            }
        }
    }





}
