import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    color: "lightgrey";
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

    ListView {
        model: albummodel
        anchors.fill: parent
        id: alview
        delegate: AlbumRecord {anchors.fill: parent}
    }
}
