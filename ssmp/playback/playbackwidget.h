#ifndef PLAYBACKMGR_H
#define PLAYBACKMGR_H

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

signals:
    void songOver();
    
public slots:
    void changeSong(QString path);
    void changeState(Playstate s);
    void seek(float ratio);

private:
    Ui::pbwidget ui;
    DBI* db;

    int cursonglength;
    DWORD curchan;
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
