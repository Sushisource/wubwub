#include "ssmp.h"

NowPlaying::NowPlaying(QObject *par)
{
    db = &DBI::getInstance();
    songs = QList<QPair<int,QString>>();
}

NowPlaying::~NowPlaying()
{
}

void NowPlaying::addSongs(QList<int> songIds)
{
    foreach(int sid, songIds)
    {
        QString name = db->getSongNameFromId(sid);
        songs.append(qMakePair(sid, name));
    }
    refresh();
}

void NowPlaying::addAlbums(QList<int> alids)
{
    foreach(int alid, alids)
    {
        addSongs(DBI::extractIds(db->getTracksFromAlbum(alid)));
    }
}

void NowPlaying::refresh()
{
    this->clear();
    foreach(QString songname, DBI::extractTracks(songs))
    {
        this->addItem(songname);
    }
}
