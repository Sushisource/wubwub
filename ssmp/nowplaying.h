#ifndef NOWPLAYING_H
#define NOWPLAYING_H
#include <QObject>
#include <QListWidget>
#include <QPair>
#include <QMap>
#include "dbi.h"
class ssmp; //TODO: Fucking circular depends

class NowPlaying : public QListWidget
{
    Q_OBJECT
public:
    NowPlaying(QObject* parent);
    ~NowPlaying();
public slots:
    void addSongs(QList<int> songIds);
    void addAlbums(QList<int> alids);

signals:
    void sendSongList(QList<QString> sl);
private:
    DBI* db;
    void refresh();
    QList<QPair<int, QString>> songs;
};

#endif // NOWPLAYING_H
