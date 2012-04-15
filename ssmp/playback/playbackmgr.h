#ifndef PLAYBACKMGR_H
#define PLAYBACKMGR_H

#include <QObject>
#include <irrKlang.h>
#include "ui_playbackwidget.h"

class PlaybackMgr : public QWidget
{
    Q_OBJECT
public:
    explicit PlaybackMgr(QWidget *parent = 0);
    ~PlaybackMgr();
    enum Playstate {PLAYING, STOPPED, PAUSED};

signals:
    void songOver();
    
public slots:
    void changeSong(QString path);
    void changeState(Playstate s);
    void seek(float ratio);

private:
    Ui::pbwidget ui;

    irrklang::ISoundEngine* eng;
    irrklang::ISound* cursong;
    int cursonglength;
    Playstate state;
    QTimer* updateTimer;
    QPixmap play;
    QPixmap pause;
    void stopSong();
    static inline QString msToString(int ms);

private slots:
    void togglePlay();
    void update();

};

#endif // PLAYBACKMGR_H
