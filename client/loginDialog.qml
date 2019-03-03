import QtQuick 2.9
import QtQuick.Controls 2.5
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


    flags: Qt.Window | Qt.FramelessWindowHint

    signal login(string username,string passwd,string pack_info,string NIC_info)
    signal change_account_select(string account)

    property int xmouse: 0
    property int ymouse: 0

    property int currBig: 0
    property int flag: 0

    function addPost (postfit){
        mod1.append({
                        "text": postfit.toString()
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

    function addUsernameInfo (msg) {
        accountModel.append({"history":msg.toString()})
    }

    Timer {
        id: tim1
        interval: 15
        running: false
        repeat: true
        onTriggered: {
            if(currBig == 0){
                login_1.y = login_1.y - 1
                login_1.height = login_1.height + 2
                login_1.width = login_1.width + 2
                login_1.x = login_1.x - 1
            }
            else if(currBig == 1){
                login_2.y = login_2.y - 1
                login_2.height = login_2.height + 2
                login_2.width = login_2.width + 2
                login_2.x = login_2.x - 1
            }
            else if(currBig == 2){
                login_3.y = login_3.y - 1
                login_3.height = login_3.height + 2
                login_3.width = login_3.width + 2
                login_3.x = login_3.x - 1
            }
            else if(currBig == 3){
                login_4.y = login_4.y - 1
                login_4.height = login_4.height + 2
                login_4.width = login_4.width + 2
                login_4.x = login_4.x - 1
            }
            else{
                login_5.y = login_5.y - 1
                login_5.height = login_5.height + 2
                login_5.width = login_5.width + 2
                login_5.x = login_5.x - 1
            }
        }
    }

    Timer {
        id: tim2
        interval: 15
        running: false
        repeat: true
        onTriggered: {
            if(currBig == 0){
                login_1.y = login_1.y + 1
                login_1.height = login_1.height - 2
                login_1.width = login_1.width - 2
                login_1.x = login_1.x + 1
            }
            else if(currBig == 1){
                login_2.y = login_2.y + 1
                login_2.height = login_2.height - 2
                login_2.width = login_2.width - 2
                login_2.x = login_2.x + 1
            }
            else if(currBig == 2){
                login_3.y = login_3.y + 1
                login_3.height = login_3.height - 2
                login_3.width = login_3.width - 2
                login_3.x = login_3.x + 1
            }
            else if(currBig == 3){
                login_4.y = login_4.y + 1
                login_4.height = login_4.height - 2
                login_4.width = login_4.width - 2
                login_4.x = login_4.x + 1
            }
            else{
                login_5.y = login_5.y + 1
                login_5.height = login_5.height - 2
                login_5.width = login_5.width - 2
                login_5.x = login_5.x + 1
            }
        }
    }

    Timer {
        interval: 150
        running: true
        repeat: true
        onTriggered: {
            if (tim1.running == true) {
                tim1.stop()
            }
            if (tim2.running == true) {
                tim2.stop()
                login_1.x = 10
                login_1.y = 25
                login_1.width = 30
                login_1.height = 20
                login_2.x = 20
                login_2.y = 25
                login_2.width = 30
                login_2.height = 20
                login_3.x = 30
                login_3.y = 25
                login_3.width = 30
                login_3.height = 20
                login_4.x = 40
                login_4.y = 25
                login_4.width = 30
                login_4.height = 20
                login_5.x = 50
                login_5.y = 25
                login_5.width = 30
                login_5.height = 20
            }

            currBig = currBig + 1 - flag

            if (currBig >= 5){
                currBig = 0
            }
            if (flag == 0){
                tim1.restart()
            }
            else {
                tim2.restart()
            }
            flag = flag == 0 ? 1 : 0


        }
    }

    Timer {
        interval: 2
        running: true
        repeat: true
        onTriggered: {
            if (hide.width < bar.width / 2){
                hide.width = hide.width + 1
            }

            if (hide.width === bar.width / 2){
                hide.x = hide.x + 1
            }
            if (hide.x === bar.x + bar.width / 2){
                hide.x = hide.x + 1
                hide.width = hide.width - 1
            }
            if (hide.x === bar.x + bar.width){
                hide.x = bar.x
                hide.width = 0
            }
        }
    }

    Image {
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
                visibility = Window.Minimized
            }
        }
    }

    Rectangle {
        id: loginPanel
        objectName: "panel1"
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
            width: 160
            height: 30
            textRole: "text"
            editable: false
            currentIndex: 0

            model: ListModel{
                id: mod1
            }


        }

        ComboBox {
            id: account
            objectName: "account"
            x: 80
            y: 80
            height: 30
            textRole: "history"
            editable: true
            width: 160
            model: ListModel{
                id: accountModel
            }
            onActivated: {
                emit: change_account_select(account.currentText)
            }

        }

        TextField {
            id: password
            objectName: "password"
            x: 80
            y: 130
            width: 160
            height: 30
            echoMode: TextInput.Password


        }

        CheckBox {
            id: remPassword
            objectName: "remPassword"
            x: 299
            y: 83
            width: 107
            height: 27
            text: qsTr("记住密码")
        }

        CheckBox {
            id: autoLogin
            objectName: "autoLogin"
            x: 299
            y: 121
            width: 102
            height: 26
            text: qsTr("自动登录")
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
                    selectDevicePanel.visible = true
                    loginPanel.visible = false
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
        id: selectDevicePanel
        objectName: "panel2"
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
            currentIndex: 0
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
                    selectDevicePanel.visible = false
                    loginPanel.visible = true
                }
            }
        }

    }

    Rectangle {
        id: loginingPanel
        objectName: "panel3"
        x: 0
        y: 150
        width: 500
        height: 221
        color: "#ffffff"
        visible: false

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

        Item {
            id: logining
            x: 121
            y: 34
            width: 87
            height: 72

            Image {
                id: login_1
                x: 10
                y: 25
                width: 30
                height: 20
                source: "qrc:/qmlforms/logining.png"
            }

            Image {
                id: login_2
                x: 20
                y: 25
                width: 30
                height: 20
                source: "qrc:/qmlforms/logining.png"
            }

            Image {
                id: login_3
                x: 30
                y: 25
                width: 30
                height: 20
                source: "qrc:/qmlforms/logining.png"
            }

            Image {
                id: login_4
                x: 40
                y: 25
                width: 30
                height: 20
                source: "qrc:/qmlforms/logining.png"
            }

            Image {
                id: login_5
                x: 50
                y: 25
                width: 30
                height: 20
                source: "qrc:/qmlforms/logining.png"
            }
        }

        Rectangle {
            id: bar
            x: 85
            y: 155
            width: 330
            height: 5
            color: "#4d7fff"
            radius: 15
        }

        Rectangle {
            id: hide
            x: 85
            y: 145
            width: 0
            height: 25
            color: "#ffffff"
        }

    }

    Rectangle {
        id: errorPanel
        objectName: "panel4"
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
                    loginPanel.visible = false
                    selectDevicePanel.visible = false
                    errorPanel.visible = false
                    loginingPanel.visible = true
                    emit: loginDialog.login(account.editText,password.text,combMod.currentText,combDevice.currentText)
                }
            }
            onPressed: {
                login_button.color = "#424dff"

            }
        }
    }

}
