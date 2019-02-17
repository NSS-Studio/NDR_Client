import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2
import QtQuick.Controls.Material 2.3
import Qt.labs.platform 1.0

ApplicationWindow {
    id: mainWindow

    x: (Screen.desktopAvailableWidth - width) / 2
    y: (Screen.desktopAvailableHeight - height) / 2
    visible: true

    flags: Qt.Window | Qt.FramelessWindowHint

    height: 500
    color: "#f7f7f7"
    width: 350

    property int xmouse: 0
    property int ymouse: 0

    SystemTrayIcon {
        visible: true
        iconSource: "qrc:/qmlforms/logo.png"

        onActivated: {
            window.show()
            window.raise()
            window.requestActivate()
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
            text: qsTr("Text")
            lineHeight: 0.9
            font.pixelSize: 14
        }

        Image {
            id: image
            x: 293
            y: 22
            width: 13
            height: 2
            source: "qrc:/qmlforms/minmax.png"
        }

        Image {
            id: image1
            x: 318
            y: 11
            width: 13
            height: 13
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
                if (x >= (parent.width - 53) && x <= (parent.width - 40) && y >= 13 && y <= 26)
                    mainWindow.visibility = Window.Minimized
                else if (x >= (parent.width - 40) && x <= (parent.width - 27) && y >= 13 && y <= 26)
                    mainWindow.visibility = Window.Minimized
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
                x: 130
                y: 40
                width: 120
                height: 20
                text: qsTr("Text")
                font.pixelSize: 14

                Rectangle {
                    id: rectangle2
                    x: 111
                    y: 73
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
                id: net
                x: 130
                y: 65
                width: 120
                height: 20
                text: qsTr("Text")
                font.pixelSize: 14
            }

            Text {
                id: ip
                x: 130
                y: 90
                width: 120
                height: 20
                text: qsTr("Text")
                font.pixelSize: 14
            }

            Text {
                id: time
                x: 131
                y: 115
                width: 120
                height: 16
                text: qsTr("Text")
                font.pixelSize: 12
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


}
