#include "ssmp.h"

RecentAlbumsView::RecentAlbumsView(QWidget *parent, int listsize) : AlbumView(parent)
{
    db = &DBI::getInstance();
}

RecentAlbumsView::~RecentAlbumsView()
{
}

void RecentAlbumsView::update(int howmany)
{
    addAlbs(db->getNRecentAlbums(howmany));
    resizeEvent();
}
