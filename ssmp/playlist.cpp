#include "ssmp.h"

Playlist::Playlist(QObject *par)
{
    db = &DBI::getInstance();
    songs = QMap<QString, QString>();
    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(dblClkRedirect(QListWidgetItem*)));
}

Playlist::~Playlist()
{
}

void Playlist::addSongs(QList<int> songIds)
{
    foreach(int sid, songIds)
    {
        QString name = db->getSongNameFromId(sid);
        //Insert pathname , user name
        songs.insert(db->getTrackColFromSong(sid, 6), name);
    }
    refresh();
}

void Playlist::addAlbums(QList<int> alids)
{
    foreach(int alid, alids)
    {
        addSongs(db->getTrackIdsFromAlbum(alid));
    }
}

void Playlist::refresh()
{
    this->clear();
    QMap<QString,QString>::iterator i;
    for(i=songs.begin(); i!=songs.end(); ++i)
    {
        QListWidgetItem* lwi = new QListWidgetItem(i.value());
        lwi->setData(Qt::WhatsThisRole, i.key());
        this->addItem(lwi);
    }
}

void Playlist::dblClkRedirect(QListWidgetItem *i)
{
    emit songChange(i->data(Qt::WhatsThisRole).toString());
}
