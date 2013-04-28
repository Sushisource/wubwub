import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    ListModel {
        id: albummodel
    }
    function addAlbum(almodel) {
        alview.model.insert(0, almodel);
        if(alview.count > 5)
            alview.model.remove(alview.count - 1);
    }

    ListView {
        model: albummodel
        anchors.fill: parent
        id: alview
        objectName: "alview"
        spacing: 15
        delegate: AlbumRecord {
            anchors.left: parent.left
            anchors.right: parent.right
        }
    }
}
