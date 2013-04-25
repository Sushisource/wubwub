import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    width: 600; height: 150
    Image {
        id: alimg
        source: "file:../../res/album_example.jpg"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        fillMode: Image.PreserveAspectFit
        height: parent.height - anchors.margins*2
        sourceSize.width: 512
        sourceSize.height: 512
    }
    DropShadow {
        anchors.fill: alimg
        source: alimg
        radius: 7.0
        color: "black"
        samples: 14
        horizontalOffset: 0
        verticalOffset: 0
    }
    Text {
        id: title
        text: "The North Borders - Bonobo [2013]";
        font.bold: true;
        font.pointSize: 11
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 5
    }
    Image {
        id: add
        source: "../../imgs/plus"
        anchors.top: alimg.top
        anchors.topMargin: (alimg.height - alimg.paintedHeight)/2
        anchors.right: alimg.left
        anchors.rightMargin: 5
    }
    Image {
        id: view
        source: "../../imgs/eye"
        anchors.top: add.bottom
        anchors.topMargin: 5
        anchors.right: alimg.left
        anchors.rightMargin: 5
    }

    ListModel {
        id: songModel
        ListElement {
            song: "First Fires"
            track: "1"
        }
        ListElement {
            song: "Emkay"
            track: "2"
        }
        ListElement {
            song: "Cirrus"
            track: "3"
        }
        ListElement {
            song: "Heaven For the Sinner"
            track: "4"
        }
        ListElement {
            song: "Sapphire"
            track: "5"
        }
    }
    Component {
        id: songDel
        Item {
            width: 180; height: 20
            Text { text: track + '. ' + song }
        }
    }

    ListView {
        anchors.top: title.bottom
        anchors.left: title.left
        anchors.bottom: parent.bottom
        anchors.topMargin: 5
        model: songModel
        delegate: songDel
    }
}
