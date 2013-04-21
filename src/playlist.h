#ifndef NOWPLAYING_H
#define NOWPLAYING_H
#include <QObject>
#include <QListWidget>
#include <QMap>
#include "dbi/dbi.h"
class wubwub; //TODO: Fucking circular depends

class Playlist : public QListWidget
{
    Q_OBJECT

public:
    Playlist(QObject*);
    ~Playlist();

public slots:
    void addSongs(QList<int> songIds);
    void addSong(int songid);
    void addAlbums(QList<int> alids);
    void nextSong();
    void prevSong();
    void playSongWithSid(int sid);
    void playSongFromAlbum(int alid, int sid);

signals:
    void sendSongList(QList<QString> sl);
    void songChange(int sid);

private:
    DBI* db;
    //Current playing song
    QListWidgetItem* cursong;
    QIcon playingIcon;

private slots:
    void dblClkRedirect(QListWidgetItem* i);
};

#endif // NOWPLAYING_H
