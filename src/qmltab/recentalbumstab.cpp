#include "recentalbumstab.h"

RecentAlbumsTab::RecentAlbumsTab(QWidget* parent, QString qmlfile) :
    QmlTab(qmlfile, parent)
{
    db = &DBI::getInstance();
}

RecentAlbumsTab::~RecentAlbumsTab()
{
}

void RecentAlbumsTab::addAlbum(Alb album)
{
    QVariantMap alrecord;
    alrecord["alname"] = album.name;
    QVariantList trax;
    int i = 1;
    foreach(QString t, album.tracks)
    {
        QVariantMap track;
        track["track"] = QString::number(i);
        track["song"] = t;
        trax.append(track);
        ++i;
    }
    alrecord["alcover"] = "file:" + album.imguri;
    alrecord["tracks"] = trax;
    QMetaObject::invokeMethod(root.get(), "addAlbum",
                              Q_ARG(QVariant,
                                    QVariant::fromValue(alrecord)));
}

void RecentAlbumsTab::addAlbums(QList<Alb> albums)
{
    foreach(Alb a, albums)
    {
        addAlbum(a);
    }
}

void RecentAlbumsTab::update()
{
    addAlbums(db->getNRecentAlbums(5));
}

void RecentAlbumsTab::newAlbs(QList<Alb> albs)
{
    addAlbums(albs);
}
