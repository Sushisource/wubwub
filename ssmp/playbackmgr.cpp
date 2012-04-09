#include "ssmp.h"
using namespace irrklang;

PlaybackMgr::PlaybackMgr(ssmp *parent) : QObject(NULL)
{
    this->parent = parent;
    //Setup Audio output
    eng = createIrrKlangDevice();
    cursong = NULL;
    qDebug() << eng->getDriverName();
}

void PlaybackMgr::changeSong(QString song)
{
    if(cursong != NULL)
    {
        cursong->drop();
        cursong = NULL;
        eng->stopAllSounds();
    }
    cursong = eng->play2D(song.toStdString().c_str(), false, false, true);
    cursong->setVolume(1);
}

void PlaybackMgr::changeState(PlaybackMgr::Playstate s)
{
}
