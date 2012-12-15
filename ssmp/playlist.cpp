#include "ssmp.h"

Playlist::Playlist(QObject *par)
{
    db = &DBI::getInstance();
    cursong = NULL;
    playingIcon = QPixmap(":/imgs/play");
    this->setIconSize(QSize(8,8));

    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(dblClkRedirect(QListWidgetItem*)));
}

void Playlist::addSongs(QList<int> songIds)
{
    foreach(int sid, songIds)
    {
        addSong(sid);
    }
}

void Playlist::addSong(int songid)
{
    QString name = db->getSongNameFromId(songid);
    //Insert pathname , user name
    QListWidgetItem* lwi = new QListWidgetItem(name);
    lwi->setData(Qt::WhatsThisRole, songid);
    this->addItem(lwi);
}

void Playlist::addAlbums(QList<int> alids)
{
    foreach(int alid, alids)
    {
        addSongs(db->getTrackIdsFromAlbum(alid));
    }
}

void Playlist::nextSong()
{
    this->setCurrentItem(cursong);
    int i = this->currentRow();
    if(i + 1 >= this->count()) //End of list
    {
        cursong->setIcon(QIcon());
        return;
    }
    this->setCurrentRow(i+1);
    dblClkRedirect(this->currentItem());
}

void Playlist::playSongWithSid(int sid)
{
    foreach(QListWidgetItem* song, findItems("*", Qt::MatchWildcard))
    {
        if(song->data(Qt::WhatsThisRole) == sid)
        {
            this->setCurrentItem(song);
            this->dblClkRedirect(song);
        }
    }
}

void Playlist::playSongFromAlbum(int alid, int sid)
{
    this->clear();
    cursong = NULL;
    this->addAlbums(QList<int>() << alid);
    this->playSongWithSid(sid);
}

void Playlist::dblClkRedirect(QListWidgetItem *i)
{
    if(cursong != NULL) //Reset old icon
        cursong->setIcon(QIcon());
    cursong = i;
    cursong->setIcon(playingIcon);
    emit songChange(i->data(Qt::WhatsThisRole).toInt());
}

Playlist::~Playlist()
{
}
