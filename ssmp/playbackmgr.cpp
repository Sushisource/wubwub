#include "ssmp.h"

PlaybackMgr::PlaybackMgr(ssmp *parent) : QObject(NULL)
{
    this->parent = parent;
    //Setup Audio output
    audioout = new Phonon::AudioOutput(Phonon::MusicCategory);
    audioout->setVolumeDecibel(100);
    cursong = new Phonon::MediaObject();
    Phonon::createPath(cursong, audioout);
}

void PlaybackMgr::changeSong(QString song)
{
    cursong->setCurrentSource(song);
    cursong->play();
}

void PlaybackMgr::changeState(PlaybackMgr::Playstate s)
{
}
