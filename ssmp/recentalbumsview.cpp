#include "recentalbumsview.h"

RecentAlbumsView::RecentAlbumsView(QWidget *parent) : AlbumView(parent)
{
    //TODO: This should probably have a scrollbar of it's own at some point.
    //Ideally, one that loads more recent albums the more you scroll
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

RecentAlbumsView::~RecentAlbumsView()
{
}

void RecentAlbumsView::update()
{
    addAlbs(db->getNRecentAlbums(5));
    resizeEvent();
}
