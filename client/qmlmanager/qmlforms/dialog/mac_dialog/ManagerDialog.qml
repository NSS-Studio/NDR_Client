import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.3
import QtGraphicalEffects 1.0

import "./labs/" as MyStyle

ApplicationWindow {
    id: managerDialog
    width: 500
    height: 370
    x: (Screen.desktopAvailableWidth - width) / 2
    y: (Screen.desktopAvailableHeight - height) / 2
    visible: true
    flags: Qt.Window | Qt.FramelessWindowHint

    // loginDialog signals
    signal login(string username,string passwd,string pack_info,string NIC_info)
    signal change_account_select(string account)

    // mainWindowDialog signal
    signal stopConnect()

    property int xmouse: 0
    property int ymouse: 0
    property bool change: false
    property int dailTime: 0

    // deal emit
    function fun_login (username, passwd, pack_info, NIC_info){
        emit: login(username,passwd,pack_info,NIC_info)
    }

    function addPackInfo (profix) {
        for (var i = 0;i < loginPanelChild.children.length;i++) {
            for (var j = 0;j < loginPanelChild.children[i].children.length;j++){
                if (loginPanelChild.children[i].children[j].objectName === "pack_info") {
                    loginPanelChild.children[i].children[j].addItem(profix.toString())
                }
            }
        }
    }
    function addDevice (device) {
        for (var i = 0;i < selectDevicePanelChild.children.length;++i){
            for (var j = 0;j < selectDevicePanelChild.children[i].children.length;++j){
                if (selectDevicePanelChild.children[i].children[j].objectName === "device_name"){
                    selectDevicePanelChild.children[i].children[j].addItem (device.toString())
                }
            }
        }
    }
    function setVersion (ver) {
        tittle.text = ver
        for (var i = 0;i < loginPanelChild.children.length;i++) {
            for (var j = 0;j < loginPanelChild.children[i].children.length;j++){
                if (loginPanelChild.children[i].children[j].objectName === "tittle") {
                    loginPanelChild.children[i].children[j].text = ver
                }
            }
        }
    }
    function addUsernameInfo(username){
        for (var i = 0;i < loginPanelChild.children.length;i++) {
            for (var j = 0;j < loginPanelChild.children[i].children.length;j++){
                console.log(loginPanelChild.children[i].children[j].objectName)
                if (loginPanelChild.children[i].children[j].objectName === "account") {
                    loginPanelChild.children[i].children[j].addItem(username.toString())
                }
            }
        }
    }

    FontLoader {
        source: "qrc:/font/ztgj.ttf"
    }

    Timer {
        id: tim_login_to_select
        running: false
        repeat: true
        interval: 1

        onTriggered: {
            loginPanel.x = loginPanel.x - 2
            selectDevicePanel.x = selectDevicePanel.x - 2
            if (selectDevicePanel.x === 0){
                tim_login_to_select.running = false
            }
        }
    }
    Timer {
        id: tim_select_to_login
        running: false
        repeat: true
        interval: 1

        onTriggered: {
            loginPanel.x = loginPanel.x + 2
            selectDevicePanel.x = selectDevicePanel.x + 2
            if (loginPanel.x === 0){
                tim_select_to_login.running = false
            }
        }
    }
    Timer {
        id: tim_login_to_logining
        running: false
        repeat: true
        interval: 1

        onTriggered: {
            loginPanel.x = loginPanel.x - 2
            loginingPanel.x = loginingPanel.x - 2
            if (loginingPanel.x === 0){
                tim_login_to_logining.running = false
            }
        }
    }
    Timer {
        id: tim_logining_to_login
        running: false
        repeat: true
        interval: 1

        onTriggered: {
            loginPanel.x = loginPanel.x + 2
            loginingPanel.x = loginingPanel.x + 2
            if (loginPanel.x === 0){
                tim_logining_to_login.running = false
            }
        }
    }
    Timer {
        id: tim_logining_to_error
        running: false
        repeat: true
        interval: 1

        onTriggered: {
            loginingPanel.x = loginingPanel.x - 2
            errorPanel.x = errorPanel.x - 2
            if (errorPanel.x === 0){
                tim_logining_to_error.running = false
            }
        }
    }
    Timer {
        id: tim_error_to_login
        running: false
        repeat: true
        interval: 1

        onTriggered: {
            loginPanel.x = loginPanel.x + 2
            errorPanel.x = errorPanel.x + 2
            if (loginPanel.x === 0){
                tim_error_to_login.running = false
            }
        }
    }

    Timer {
        id: tim_loginDialog_to_mainDialog
        running: false
        repeat: true
        interval: 1
        onTriggered: {
            managerDialog.width = managerDialog.width - 4
            managerDialog.x = managerDialog.x + 2
            managerDialog.height = managerDialog.height + 4
            managerDialog.y = managerDialog.y - 2
            if(managerDialog.width === 380){
                tim_loginDialog_to_mainDialog.running = false
                mainWindow.visible = true
            }
        }
    }

    Timer {
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
        objectName: "startTime"
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            dailTime = dailTime + 1
            addTime()
        }
    }





    Item {
        id: loginDialog
        anchors.fill: parent
        MyStyle.LoginBackground{anchors.fill: parent;}
        MyStyle.BlurImg {
            y: 150
        }
        Item {
            id: loginPanel
            x: 0
            y: 0
            width: parent.width
            height: parent.height - 150
            visible: true

            MyStyle.LoginLoginPanel {
                id: loginPanelChild
                y: 150;
                onSelectDeviceClicked: {
                    selectDevicePanel.x = 500
                    selectDevicePanel.visible = true
                    tim_login_to_select.running = true
                }
            }
            MyStyle.LoginBtn {
                x: 360
                y: 122
                onClick: {
                    var user,pass,pack,dev
                    for (var i = 0;i < loginPanelChild.children.length;i++) {
                        for (var j = 0;j < loginPanelChild.children[i].children.length;j++){
                            if (loginPanelChild.children[i].children[j].objectName === "account"){
                                user = loginPanelChild.children[i].children[j].currentText
                            }
                            else if (loginPanelChild.children[i].children[j].objectName === "password"){
                                pass = loginPanelChild.children[i].children[j].text
                            }
                            else if (loginPanelChild.children[i].children[j].objectName === "pack_info"){
                                pack = loginPanelChild.children[i].children[j].currentText
                            }
                        }
                    }
                    for (var ii = 0;ii < selectDevicePanelChild.children.length;++ii){
                        for (var jj = 0;jj < selectDevicePanelChild.children[ii].children.length;++jj){
                            if (selectDevicePanelChild.children[ii].children[jj].objectName === "device_name"){
                                dev = selectDevicePanelChild.children[ii].children[jj].currentText
                            }
                        }
                    }
                    console.log(user+pass+pack+dev)
                    if (user === "" || pass === ""){
                        for (var iii = 0;iii < loginPanelChild.children.length;iii++) {
                            for (var jjj = 0;jjj < loginPanelChild.children[iii].children.length;jjj++){
                                if (loginPanelChild.children[iii].children[jjj].objectName === "status") {
                                    loginPanelChild.children[iii].children[jjj].text = "账号或密码为空！"
                                    loginPanelChild.children[iii].children[jjj].color = "red"
                                    loginPanelChild.children[iii].children[jjj].x = 80
                                    loginPanelChild.children[iii].children[jjj].y = 170
                                }
                            }
                        }
                    }
                    else {
                        for (var iiii = 0;iiii < loginPanelChild.children.length;iiii++) {
                            for (var jjjj = 0;jjjj < loginPanelChild.children[iiii].children.length;jjjj++){
                                if (loginPanelChild.children[iiii].children[jjjj].objectName === "status") {
                                    loginPanelChild.children[iiii].children[jjjj].visible = false
                                }
                            }
                        }
//                        emit: login(user,pass,pack,dev)
//                        loginingPanel.visible = true
//                        loginingPanel.x = 500
//                        tim_login_to_logining.running = true
                        dailTime = 0

                        loginDialog.visible = false
                        change = true
                        tim_loginDialog_to_mainDialog.running = true

                    }
                }
            }
        }

        Item {
            id: selectDevicePanel
            x: 0
            y: 0
            width: parent.width
            height: parent.height - 150
            visible: false
            MyStyle.LoginSelectDevicePanel{
                id: selectDevicePanelChild
                y: 150
                onOkBtnClicked: {
                    tim_select_to_login.running = true
                }
            }
        }

        Item {
            id: loginingPanel
            x: 0
            y: 0
            height: parent.height - 150
            width: parent.width
            visible: false
            MyStyle.LoginIngPanel{
                y: 150
            }
        }

        Item {
            id: errorPanel
            x: 0
            y: 0
            width: parent.width
            height: parent.height - 150
            visible: false
            MyStyle.LoginErrorPanel {
                y: 150
                onOkClicked: {
                    errorPanel.visible = true
                    tim_error_to_login.running = true
                }
            }
        }
    }
    Item{
        id: mainWindow
        anchors.fill: parent
        visible: false
        MyStyle.MainWindowBar{
            x:0;y:0
        }
        MyStyle.Icon {}
        MyStyle.MainwindowMainPanel{
            id: mainWindowPanel
            y: 35
            visible: true
            height: parent.height - 35
            width: parent.width
        }

        MyStyle.MainWindowSettingPanel{
            id: mainWindowSettingPanel
            y: 35
            visible: false
            height: parent.height - 35
            width: parent.width
        }
    }
    MyStyle.Close{
        x: managerDialog.width - 20;
        y: 10;
        onMouseClicked: {
            Qt.quit();
        }
    }
    MyStyle.MiniMax{
        x: managerDialog.width - 43;
        y: 10;
        onMouseClicked: {
            flags = Qt.Window | Qt.WindowFullscreenButtonHint | Qt.CustomizeWindowHint | Qt.WindowMinimizeButtonHint
            visibility = Window.Minimized
        }
    }
    MyStyle.Setting {
        x: managerDialog.width - 65
        y: 10;
        onClick: {
            if(change === true){

            }
            else {
                console.log("122")
                mainWindowSettingPanel.visible = true
                mainWindowPanel.visible = false
            }
        }
    }
    MyStyle.LoginTittle {id: tittle;x: 5;y: 5;}

    MouseArea{
        height: 50
        width: parent.width - 80

        onPressed: {
            xmouse = mouseX
            ymouse = mouseY
        }

        onPositionChanged: {
            managerDialog.x = managerDialog.x + (mouseX - xmouse)
            managerDialog.y = managerDialog.y + (mouseY - ymouse)
        }
    }
}
