import QtQuick 2.0

Item {
    width: parent.width
    height: parent.width - 150

    property int currBig: 0
    property int flag: 0
    property int flag1: 0

    Timer {
        id: tim1 // control logining panel
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
        id: tim2 // control logining panel
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

    Timer { // contral logining panel
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

    Timer { // control logining panel
        interval: 2
        running: true
        repeat: true
        onTriggered: {
            if(bar1.width === 330) {
                flag1 = 1
            }
            if (bar1.x === 330 + 85) {
                flag1 = 0
                bar1.x = 85
                bar1.width = 0
            }
            if (flag1 === 0){
                bar1.width = bar1.width + 1
            }
            else {
                bar1.x = bar1.x + 1
                bar1.width = bar1.width - 1
            }

        }
    }
    Rectangle {
        anchors.fill: parent
        color: "#ffffff"
        opacity: 0.6

        Text {
            id: logingText
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
            text: qsTr("正在尝试拨号")
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
                source: "qrc:/mac_forms/logining.png"
            }

            Image {
                id: login_2
                x: 20
                y: 25
                width: 30
                height: 20
                source: "qrc:/mac_forms/logining.png"
            }

            Image {
                id: login_3
                x: 30
                y: 25
                width: 30
                height: 20
                source: "qrc:/mac_forms/logining.png"
            }

            Image {
                id: login_4
                x: 40
                y: 25
                width: 30
                height: 20
                source: "qrc:/mac_forms/logining.png"
            }

            Image {
                id: login_5
                x: 50
                y: 25
                width: 30
                height: 20
                source: "qrc:/mac_forms/logining.png"
            }
        }

        Rectangle {
            id: bar1
            x: 85
            y: 155
            width: 0
            height: 5
            color: "#4d7fff"
            radius: 15
        }


    }

}
