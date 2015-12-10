import QtQuick 2.0
import QtSpriter 1.0

Rectangle {
    width: 800
    height: 600
    color: "red"
    SpriterModel {
        id: modelId
        file: "GreyGuy/player.scml"
    }
    Repeater {
        model:100
        EntityInstance {
            x: 50+Math.random()*700
            y: Math.random()*400+200
            model: modelId
            name: "Player"
            animation: "walk"
            scale: {var a = Math.random();Qt.point(a,a)}
        }
    }
    Rectangle {
        anchors.centerIn: parent
        width: 100
        height: 150
        color: "white"
        EntityInstance {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 20
            model: modelId
            name: "Player"
            animation: "idle"
            scale: "0.5,0.5"
        }
    }
}
