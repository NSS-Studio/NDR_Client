import QtQuick 2.0

import "./" as MyStyle

Item {
    width: parent.width
    height: parent.width - 150

    signal accountChange(string account)
    signal selectDeviceClicked()
    signal repaireNET()

    Rectangle {
        anchors.fill: parent
        color: "#FFFFFF"
        opacity: 0.6
        MyStyle.Combobox {
            objectName: "pack_info"
            id: pack_info
            x: 80
            y: 30
        }
        MyStyle.Combobox {
            id: account
            objectName: "account"
            x: 80
            y: 80
            editable: true
            onActivated: {
                emit: accountChange(account.editText)
            }
        }
        MyStyle.LoginPassWord {
            id: password
            objectName: "password"
            x: 80
            y: 130
        }
        MyStyle.LoginCheckbox {
            objectName: "rember"
            id: remberPass
            x: 300
            y: 80
            checkState: Qt.Unchecked
            text: qsTr("记住密码")
        }
        MyStyle.LoginCheckbox {
            id: autoLogin
            objectName: "autologin"
            x: 300
            y: 120
            text: qsTr("自动登录")
        }
        MyStyle.SelectDevice {
            x: 300
            y: 40
            onClicke: {
                emit: selectDeviceClicked()
            }
        }
        MyStyle.LoginStatus{
            objectName: "status"
            id: status
            x: 80
            y: 150
        }

        MyStyle.LoginTittle{
            objectName: "tittle"
            x: 330
            y: 195
            color: "#7c6e6e"
            font.pixelSize: 14
        }

        MyStyle.LoginDialogRepaireNet{
            x: 20
            y: 195
            onClick: {
                emit: repaireNET()
            }
        }
    }
}
