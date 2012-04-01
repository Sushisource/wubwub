#include "ssmp.h"

NowPlaying::NowPlaying(ssmp *par)
{
    parent = par;
    songs = QMap<int, QString>();
}

void NowPlaying::addSongs(QList<int> songIds)
{
    foreach(int sid, songIds)
    {

    }
    emit sendSongList(songs);
}

NowPlaying::~NowPlaying()
{
}
