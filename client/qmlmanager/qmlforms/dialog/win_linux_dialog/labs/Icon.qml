import QtQuick 2.9
import Qt.labs.platform 1.0

SystemTrayIcon {
    visible: true
    iconSource: "qrc:/win_linux_forms/icon.png"

    signal showWindowClick()
    signal aboutClick()
    signal exitClick()

    menu: Menu{
        MenuItem{
            text: "显示窗口"
            onTriggered: {
                emit: showWindowClick()
            }
        }
        MenuItem{
            text: "关于"
            onTriggered: {
                emit: aboutClick()
            }
        }

        MenuItem{
            text: "退出"
            onTriggered: {
                emit: exitClick()
            }
        }
    }
}
