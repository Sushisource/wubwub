#include "ssmp.h"

//Bass error messages
void Error(const char *text)
{
    printf("Error(%d): %s\n",BASS_ErrorGetCode(),text);
    BASS_Free();
    exit(0);
}

PlaybackWidget::PlaybackWidget(QWidget *parent) : QWidget(parent)
{
    if (HIWORD(BASS_GetVersion())!=BASSVERSION) {
        qDebug() << "Wrong bass version";
    }
    db = &DBI::getInstance();
    //Setup ui
    ui.setupUi(this);
    play = QPixmap(":/imgs/play");
    pause = QPixmap(":/imgs/pause");
    ui.prevBtn->setIcon(QIcon(QPixmap(":/imgs/prev")));
    ui.nextBtn->setIcon(QIcon(QPixmap(":/imgs/next")));
    ui.playBtn->setIcon(QIcon(play));
    connect(ui.playBtn, SIGNAL(released()), SLOT(togglePlay()));
    //Setup Audio output
    if (!BASS_Init(-1,44100,0,0,NULL))
        Error("Can't initialize device");
    curchan = NULL;
    state = STOPPED;
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), SLOT(update()));
    connect(ui.playSlider, SIGNAL(seekTo(float)), SLOT(seek(float)));
    updateTimer->setInterval(95);
}

PlaybackWidget::~PlaybackWidget()
{
    stopSong();
}

void PlaybackWidget::changeSong(QString songPath)
{
    stopSong();
    curchan = BASS_StreamCreateFile(false,songPath.toAscii(),0,0,
                                    BASS_SAMPLE_FLOAT | BASS_STREAM_AUTOFREE);
    BASS_ChannelPlay(curchan,false);
    cursonglength = BASS_ChannelBytes2Seconds(curchan,0);
    changeState(PLAYING);
}

void PlaybackWidget::changeState(PlaybackWidget::Playstate s)
{
    //No need to do anything if there isn't a song playing.
    if(curchan == NULL)
        return;
    if(s == PLAYING)
    {
        ui.playBtn->setIcon(pause);
        BASS_ChannelPlay(curchan,false);
        updateTimer->start();
    }
    else if(s == PAUSED)
    {
        ui.playBtn->setIcon(play);
        BASS_ChannelPause(curchan);
    }
    else if(s == STOPPED)
        stopSong();
    state = s;
}

void PlaybackWidget::seek(float ratio)
{
    BASS_ChannelSetPosition(curchan,ratio*cursonglength,BASS_FILEPOS_BUFFER);
}

void PlaybackWidget::stopSong()
{
    if(curchan == NULL)
        return;
    BASS_ChannelStop(curchan);
    curchan = NULL;
    updateTimer->stop();
}

QString PlaybackWidget::msToString(int ms)
{
    float secs = ms/1000.0;
    int mins = secs / 60;
    int remainder = secs - mins*60;
    QString m =  QString("%1").arg(mins, 2, 10, QChar('0'));
    QString s =  QString("%1").arg(remainder, 2, 10, QChar('0'));
    return m + ":" + s;
}


void PlaybackWidget::togglePlay()
{
    if(state == PAUSED)
        changeState(PLAYING);
    else if(state == PLAYING)
        changeState(PAUSED);
}

void PlaybackWidget::update()
{
    //Check if the song is finished, and request the next one from playlist
    if(state == PLAYING && !BASS_ChannelIsActive(curchan))
    {
        state = STOPPED;
        emit songOver();
    }
    //Now update the gui elements
    int ms = BASS_ChannelGetPosition(curchan, BASS_POS_MUSIC_ORDER);
    float ratio = (float)ms/cursonglength;
    ui.playedLbl->setText(msToString(ms));
    ui.totalLbl->setText("-" + msToString(cursonglength - ms));
    int val = ratio*ui.playSlider->maximum();
    ui.playSlider->setValue(val);
}
