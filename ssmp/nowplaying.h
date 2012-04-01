#ifndef NOWPLAYING_H
#define NOWPLAYING_H
#include <QObject>
#include <QMap>
class ssmp; //TODO: Fucking circular depends

class NowPlaying : public QObject
{
    Q_OBJECT
public:
    NowPlaying(ssmp* parent);
    ~NowPlaying();
    void addSongs(QList<int> songIds);
signals:
    void sendSongList(QMap<int, QString> sl);
private:
    ssmp* parent;
    QMap<int, QString> songs;
};

#endif // NOWPLAYING_H
