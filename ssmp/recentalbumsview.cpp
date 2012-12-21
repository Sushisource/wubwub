#include "recentalbumsview.h"

RecentAlbumsView::RecentAlbumsView(QWidget *parent) : AlbumView(parent)
{
}

RecentAlbumsView::~RecentAlbumsView()
{
}

void RecentAlbumsView::update()
{
    addAlbs(db->getNRecentAlbums(5));
    resizeEvent();
}
