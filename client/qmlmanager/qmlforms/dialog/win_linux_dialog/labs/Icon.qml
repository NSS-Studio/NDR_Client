import QtQuick 2.9
import Qt.labs.platform 1.0

SystemTrayIcon {
    visible: true
    iconSource: "qrc:/qmlforms/logo.png"

    signal showWindowClick()

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
            text: "显示窗口"
            onTriggered: {
                emit: showWindowClick()
            }
        }

        MenuItem{
            text: "退出"
            onTriggered: {
                Qt.quit()
            }
        }
    }
}
