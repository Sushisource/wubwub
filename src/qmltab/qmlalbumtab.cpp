#include "qmlalbumtab.h"

QmlAlbumTab::QmlAlbumTab(QString qmlfile, QWidget *parent) :
    QmlTab(qmlfile, parent)
{
    db = &DBI::getInstance();
}

void QmlAlbumTab::addAlbum(Alb album)
{
    QVariantMap alrecord;
    QString displayName = album.name + " - " + album.artist;;
    if(album.year != "0")
        displayName += " [" + album.year + "]";
    alrecord["alname"] = displayName;
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
    alrecord["alid"] = album.alid.toInt();
    QMetaObject::invokeMethod(
                root.get(), "addAlbum",
                Q_ARG(QVariant, QVariant::fromValue(alrecord)));
}

void QmlAlbumTab::addAlbums(QList<Alb> albums)
{
    foreach(Alb a, albums)
    {
        addAlbum(a);
    }
}
