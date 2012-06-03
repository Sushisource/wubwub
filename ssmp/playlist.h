#ifndef NOWPLAYING_H
#define NOWPLAYING_H
#include <QObject>
#include <QListWidget>
#include <QMap>
#include "dbi/dbi.h"
class ssmp; //TODO: Fucking circular depends

class Playlist : public QListWidget
{
    Q_OBJECT

public:
    Playlist(QObject* parent);
    ~Playlist();

public slots:
    void addSongs(QList<int> songIds);
    void addAlbums(QList<int> alids);
    void nextSong();

signals:
    void sendSongList(QList<QString> sl);
    void songChange(QString path);

private:
    DBI* db;
    //Current playing song
    QListWidgetItem* cursong;
    QIcon playingIcon;

private slots:
    void dblClkRedirect(QListWidgetItem* i);
};

#endif // NOWPLAYING_H
