#include "ssmp.h"

PlaybackMgr::PlaybackMgr(ssmp *parent) : QObject(NULL)
{
    this->parent = parent;
    //Setup Audio output
    audioout = new Phonon::AudioOutput();
    qDebug() << audioout->volume();
    audioout->setVolume(5);
    cursong = new Phonon::MediaObject();
    Phonon::createPath(cursong, audioout);
}

void PlaybackMgr::changeSong(QString song)
{
    qDebug() << song;
    cursong->setCurrentSource(song);
    cursong->play();
}

void PlaybackMgr::changeState(PlaybackMgr::Playstate s)
{
}
