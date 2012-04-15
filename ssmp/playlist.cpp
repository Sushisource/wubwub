#include "ssmp.h"

Playlist::Playlist(QObject *par)
{
    db = &DBI::getInstance();
    cursong = NULL;
    songs = QMap<QString, QString>();
    playingIcon = QPixmap(":/imgs/play");
    this->setIconSize(QSize(8,8));

    connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(dblClkRedirect(QListWidgetItem*)));
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
    if(cursong != NULL) //Reset old color
        cursong->setIcon(QIcon());
    cursong = i;
    cursong->setIcon(playingIcon);
    emit songChange(i->data(Qt::WhatsThisRole).toString());
}

Playlist::~Playlist()
{
}
