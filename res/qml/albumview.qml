import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    ListModel {
        id: albummodel
        ListElement {
            alname: "Bonobo"
            alcover: "file:../res/album_example.jpg"
            tracks: [
                ListElement {
                    song: "First Fires"
                    track: "1"
                },
                ListElement {
                    song: "Emkay"
                    track: "2"
                },
                ListElement {
                    song: "Cirrus"
                    track: "3"
                },
                ListElement {
                    song: "Heaven For the Sinner"
                    track: "4"
                },
                ListElement {
                    song: "Sapphire"
                    track: "5"
                }
            ]
        }
    }
    function addAlbum(almodel) {
        alview.model.append(almodel);
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
