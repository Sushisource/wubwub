#include "recentalbumstab.h"

RecentAlbumsTab::RecentAlbumsTab(QWidget* parent, QString qmlfile) :
    AlbumView(qmlfile, parent)
{
    mostrecentAlb = -1;
}

RecentAlbumsTab::~RecentAlbumsTab()
{
}

void RecentAlbumsTab::addAlbum(Alb album)
{
    //Update the most recent album
    int alid = album.alid.toInt();
    if(alid > mostrecentAlb)
        mostrecentAlb = alid;
    AlbumView::addAlbum(album);
}

void RecentAlbumsTab::update()
{
    if(mostrecentAlb < 0)
        addAlbums(db->getNRecentAlbums(5));
    else
    {
        addAlbums(db->getNewAlbumsSince(mostrecentAlb));
    }
}
