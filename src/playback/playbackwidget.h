#ifndef PLAYBACKMGR_H
#define PLAYBACKMGR_H

#define FFT_SIZE 4096

#include <QObject>
#include <bass.h>
#include "ui_playbackwidget.h"
#include "dbi/dbi.h"

class PlaybackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaybackWidget(QWidget *parent = 0);
    ~PlaybackWidget();
    enum Playstate {PLAYING, STOPPED, PAUSED};

    bool getFFT(void *buffer);
signals:
    void songOver();
    void stoppedPlaying();
    
public slots:
    void changeSong(QString path);
    void changeState(Playstate s);
    void togglePlay();
    void seek(float ratio);

private:
    Ui::pbwidget ui;
    DBI* db;

    int cursonglength;
    int cursongblength; //Byte length
    DWORD curchan;
    Playstate state;
    std::unique_ptr<QTimer> updateTimer;
    QPixmap play;
    QPixmap pause;
    void stopSong();
    static inline QString msToString(int ms);

private slots:
    void update();

};

#endif // PLAYBACKMGR_H
