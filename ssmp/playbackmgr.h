#ifndef PLAYBACKMGR_H
#define PLAYBACKMGR_H

#include <QObject>
#include <irrKlang.h>
class ssmp;

class PlaybackMgr : public QObject
{
    Q_OBJECT
public:
    explicit PlaybackMgr(ssmp *parent = 0);
    enum Playstate {PLAYING, STOPPED, PAUSED};
    
public slots:
    void changeSong(QString path);
    void changeState(Playstate s);

private:
    ssmp* parent;
    irrklang::ISoundEngine* eng;
    irrklang::ISound* cursong;

};

#endif // PLAYBACKMGR_H
