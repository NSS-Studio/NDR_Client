import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

ApplicationWindow {
    id: root
    objectName: "roott"
    width: 330
    height: 350
    visible: true

    flags: Qt.Window | Qt.FramelessWindowHint



    ComboBox {
        x: 0
        y: 190
        currentIndex: 2
        textRole: "text"
        model: ListModel {
            id: cbItems
            ListElement { text: "Banana"; color: "Yellow" }
            ListElement { text: "Apple"; color: "Green" }
            ListElement { text: "Coconut"; color: "Brown" }
        }
        width: 200
        enabled: true
        hoverEnabled: true
        onCurrentIndexChanged: console.debug(cbItems.get(currentIndex).text + ", " + cbItems.get(currentIndex).color)
    }

Text{
    id: text1
    width:300
    height:20
    x: 0
    y: 0
    text: qsTr("ttt")

}
Text{
    id: text2
    width:300
    height:20
    x: 0
    y: 114
    text: qsTr("ttt")
}
//    MouseArea{

//        anchors.fill: parent
//        property int rootx
//        property int rooty
//        property int mousex
//        property int mousey
//        property bool flag: false
//        onPressed: {

//            rootx = root.x
//            rooty = root.y
//            mousex = mouseX
//            mousey = mouseY
//            if(mouseY <= 50){
//                flag = true
//            }
//            else{
//                flag = false
//            }
//            text1.text = qsTr(rootx.toString())
//            text2.text = qsTr(rooty.toString())
//        }

//        onClicked: {
//            var x = mouseX
//            var y = mouseY
//            if (x >= 320 && x <= 340 && y >= 10 && y <= 20)
//                Qt.quit()
//            else if (x >= 300 && x < 340 && y >= 10 && y <= 20)
//                root.visibility = Window.Minimized
//        }
//        onPositionChanged: {
//            if(mouseY <= 50 && flag){
//                root.x = root.x + (mouseX - mousex)
//                root.y = root.y + (mouseY - mousey)
//                rootx = root.x
//                rooty = root.y
//            }
//        }
//    }
}
