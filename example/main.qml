import QtQuick 2.0
import Spriter 0.1

Rectangle {
    width: 800
    height: 600
    color: "red"
    SpriterModel {
        id: modelId
        file: "GreyGuy/player.scml"
        debug: true
        renderBones: true
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
            speedRatio: Math.random()
        }
    }
    Rectangle {
        anchors.centerIn: parent
        width: 200
        height: 300
        color: "white"
        EntityInstance {
            id: mainChar
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 20
            model: modelId
            name: "Player"
            animation: "walk"
            onLooped: {
                this.pause()
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: mainChar.startResume()
        }
    }
}
