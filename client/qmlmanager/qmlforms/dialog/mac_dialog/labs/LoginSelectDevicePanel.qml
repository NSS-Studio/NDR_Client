import QtQuick 2.0

import "./" as MyStyle

Item {
    width: parent.width
    height: parent.width - 150

    signal okBtnClicked

    Rectangle {
        anchors.fill: parent
        color: "#FFFFFF"
        opacity: 0.6
        MyStyle.DeviceCombobox {
            objectName: "device_name"
            id: device_name
            x: 140
            y: 50
        }
        MyStyle.LoginSelectDeviceOk{
            x: 200
            y: 130
            onOkClicked: {
                emit: okBtnClicked()
            }
        }

    }
}
