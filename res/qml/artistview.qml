import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
    function addAlbum(almodel) {
        alview.model.insert(0, almodel);
    }
    signal albumAdd(int alid)
    signal albumView(int alid)

    ListModel {
        id: albummodel
    }
    ListView {
        model: albummodel
        anchors.fill: parent
        id: alview
        objectName: "arview"
        spacing: 10
        delegate: AlbumRecord {
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }
}
