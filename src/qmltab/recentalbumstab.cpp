#include "recentalbumstab.h"

RecentAlbumsTab::RecentAlbumsTab(QWidget* parent, QString qmlfile) :
    QmlTab(qmlfile, parent)
{
    //TODO: This should probably have a scrollbar of it's own at some point.
    //Ideally, one that loads more recent albums the more you scroll
}

RecentAlbumsTab::~RecentAlbumsTab()
{
}

void RecentAlbumsTab::update()
{
    //addAlbs(db->getNRecentAlbums(5));
}

void RecentAlbumsTab::newAlbs(QList<Alb> albs)
{
    //addAlbs(albs);
}
