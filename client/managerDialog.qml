import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.3

import "./" as MyStyle

ApplicationWindow {
    id: managerDialog
    objectName: "root"
    x: MyStyle.MacloginDialog.x
    y: MyStyle.MacloginDialog.y
    width: MyStyle.MacloginDialog.width
    height: MyStyle.MacloginDialog.height
    visible: true
    MyStyle.MacloginDialog{
        objectName: "loginDialog"
    }
}
