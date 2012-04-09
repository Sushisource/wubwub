#include "ssmp.h"

PlaybackMgr::PlaybackMgr(QWidget *parent) : QWidget(parent)
{
    //Setup ui
    ui.setupUi(this);
    connect(ui.playBtn, SIGNAL(released()), SLOT(togglePlay()));
    //Setup Audio output
    eng = irrklang::createIrrKlangDevice();
    cursong = NULL;
    state = STOPPED;
    seekerUpdater = new QTimer(this);
    connect(seekerUpdater, SIGNAL(timeout()), SLOT(updateSeeker()));
    connect(ui.playSlider, SIGNAL(seekTo(float)), SLOT(seek(float)));
    seekerUpdater->setInterval(500);
}

PlaybackMgr::~PlaybackMgr()
{
    stopSong();
    eng->drop();
}

void PlaybackMgr::changeSong(QString song)
{
    stopSong();
    cursong = eng->play2D(song.toStdString().c_str(), false, false, true);
    cursong->setVolume(1);
    cursonglength = cursong->getPlayLength();
    changeState(PLAYING);
}

void PlaybackMgr::changeState(PlaybackMgr::Playstate s)
{
    //No need to do anything if there isn't a song playing.
    if(cursong == NULL)
        return;
    if(s == PLAYING)
    {
        ui.playBtn->setText("pause");
        cursong->setIsPaused(false);
        seekerUpdater->start();
    }
    else if(s == PAUSED)
    {
        ui.playBtn->setText("play");
        cursong->setIsPaused(true);
    }
    else if(s == STOPPED)
        stopSong();
    state = s;
}

void PlaybackMgr::seek(float ratio)
{
    cursong->setPlayPosition(ratio*cursonglength);
}

void PlaybackMgr::stopSong()
{
    if(cursong == NULL)
        return;
    cursong->drop();
    cursong = NULL;
    eng->stopAllSounds();
    seekerUpdater->stop();
}

QString PlaybackMgr::msToString(int ms)
{
    float secs = ms/1000;
    int mins = secs / 60;
    int remainder = secs - mins*60;
    QString m =  QString("%1").arg(mins, 2, 10, QChar('0'));
    QString s =  QString("%1").arg(remainder, 2, 10, QChar('0'));
    return m + ":" + s;
}


void PlaybackMgr::togglePlay()
{
    if(state == PAUSED)
        changeState(PLAYING);
    else if(state == PLAYING)
        changeState(PAUSED);
    qDebug() << state;
}

void PlaybackMgr::updateSeeker()
{
    int ms = cursong->getPlayPosition();
    float ratio = (float)ms/cursonglength;
    ui.playedLbl->setText(msToString(ms));
    ui.totalLbl->setText("-" + msToString(cursonglength - ms));
    int val = ratio*ui.playSlider->maximum();
    ui.playSlider->setValue(val);
}
