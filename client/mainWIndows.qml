import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

ApplicationWindow {
    id: root
    objectName: "roott"
    width: 500
    height: 370
    visible: true

    flags: Qt.Window | Qt.FramelessWindowHint



    Image {
        id: backgrand
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        source: "file:/home/wenner/workspace/nriui/image 2.1.png"
    }
    Image {
        id: close
        x: parent.width - 25
        y: 11
        width: 13
        height: 13
        source: "file:/home/wenner/workspace/nriui/close.png"
    }
    Image {
        id: minmax
        x: parent.width - 45
        y: 21
        width: 13
        height: 2
        source: "file:/home/wenner/workspace/nriui/minmax.png"
    }


    Text {
        id: tt
        x: 15
        y: 36
        width: 50
        height: 50

        text: "aaa"
    }

    Rectangle {
        id: rectangle
        x: 0
        y: 150
        width: parent.width
        height: parent.height - 150
        color: "white"
        ComboBox {
            id: combMod
            x: 80
            y: 30
            width: 150
            height: 30
            editable: true
            font.bold: false
            hoverEnabled: true
            enabled: true
            focusPolicy: Qt.StrongFocus
            displayText: qsTr("Banana")
            currentIndex: 0
            textRole: "text"
            model: ListModel {
                id: comTcMod
                ListElement { text: "Banana"; color: "Yellow" }
                ListElement { text: "Apple"; color: "Green" }
                //                ListElement { text: "Coconut"; color: "Brown" }
            }

            transformOrigin: Item.Center

            onCurrentIndexChanged: console.debug(cbItems.get(currentIndex).text + ", " + cbItems.get(currentIndex).color)

            indicator: Canvas {
                id: canvMod
                x: combMod.width - width - combMod.rightPadding
                y: combMod.topPadding + (combMod.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"

                Connections {
                    target: combMod
                    onPressedChanged: canvMod.requestPaint()
                }

                onPaint: {
                    context.reset();
                    context.moveTo(0, 0);
                    context.lineTo(width, 0);
                    context.lineTo(width / 2, height);
                    context.closePath();
                    context.fillStyle = "black"
                    context.fill();
                }
            }


            background: Rectangle {
                radius: 2
            }
        }
        ComboBox {
            id: account
            x: 80
            y: 80
            height: 30
            displayText: qsTr("")
            textRole: "history"
            font.bold: false
            editable: true
            width: 150

            //            editable: true

            model: ListModel {
                id: comAccMod
            }
            indicator: Canvas {
                id: canvAccount
                x: account.width - width - account.rightPadding
                y: account.topPadding + (account.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"

                Connections {
                    target: account
                    onPressedChanged: canvAccount.requestPaint()
                }

                onPaint: {
                    context.reset();
                    context.moveTo(0, 0);
                    context.lineTo(width, 0);
                    context.lineTo(width / 2, height);
                    context.closePath();
                    context.fillStyle = "black"
                    context.fill();
                }
            }
            background: Rectangle {
                radius: 2
            }
        }

        TextInput {
            id: password
            x: 80
            y: 130
            width: 150
            height: 30
            color: "#ffffff"
            text: qsTr("Text Input")
            autoScroll: false
            echoMode: TextInput.Password
            font.family: "Arial"
            font.pixelSize: 12
        }



    }



    MouseArea{
        id: rootMouseArea
        anchors.fill: parent
        property int rootx
        property int rooty
        property int mousex
        property int mousey
        property bool flag: false
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        hoverEnabled: true
        onPressed: {

            rootx = root.x
            rooty = root.y
            mousex = mouseX
            mousey = mouseY
            if(mouseY <= 50){
                flag = true
            }
            else{
                flag = false
            }
        }

        onPositionChanged: {
            var x = mouseX
            var y = mouseY
            if (x >= 320 && x <= 340 && y >= 10 && y <= 20)
                tt.text = qsTr ("ssssddd")
            else if (x >= 300 && x < 320 && y >= 10 && y <= 20)
                tt.text = qsTr ("close")
            if (mouse.buttons === 1) {
                if(mouseY <= 50 && flag){
                    root.x = root.x + (mouseX - mousex)
                    root.y = root.y + (mouseY - mousey)
                    rootx = root.x
                    rooty = root.y
                }
            }


        }

        onClicked: {
            var x = mouseX
            var y = mouseY
            if (x >= (parent.width - 25) && x <= (parent.width - 12) && y >= 11 && y <= 24)
                Qt.quit()
            else if (x >= (parent.width - 45) && x <= (parent.width - 32) && y >= 11 && y <= 24)
                root.visibility = Window.Minimized
        }

        CheckBox {
            id: remPassword
            x: 289
            y: 241
            width: 107
            height: 27
            text: qsTr("记住密码")
        }

        CheckBox {
            id: autoLogin
            x: 290
            y: 276
            width: 102
            height: 26
            text: qsTr("自动登录")
        }

        Text {
            id: text1
            x: 295
            y: 186
            color: "#266efe"
            text: qsTr("选择网卡")
            font.family: "Courier"
            font.pixelSize: 18
        }
    }

}
