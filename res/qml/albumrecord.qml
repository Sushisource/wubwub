import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    height: 110
    Image {
        id: alimg
        source: alcover
        anchors.top: parent.top
        anchors.right: parent.right
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
        samples: 15.0
        horizontalOffset: 0
        verticalOffset: 0
    }
    Text {
        id: title
        text: alname;
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
        MouseArea {
            anchors.fill: parent
            onClicked: root.albumAdd(alid)
        }
    }
    Image {
        id: view
        source: "../../imgs/eye"
        anchors.top: add.bottom
        anchors.topMargin: 5
        anchors.right: alimg.left
        anchors.rightMargin: 5
        MouseArea {
            anchors.fill: parent
            onClicked: root.albumView(alid)
        }
    }

    Component {
        id: songDel
        Item {
            width: 180; height: 15
            Text { text: track + '. ' + song }
        }
    }

    ListView {
        anchors.top: title.bottom
        anchors.left: title.left
        anchors.bottom: parent.bottom
        anchors.topMargin: 5
        model: tracks
        delegate: songDel
    }
}
