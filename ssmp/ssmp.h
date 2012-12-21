#ifndef SSMP_H
#define SSMP_H

#include <functional>
#include <numeric>

#include <QMainWindow>
#include <QSettings>
#include <QThreadPool>
#include <QKeyEvent>

#include "ui_ssmp.h"
#include "recentalbumsview.h"
#include "playlist.h"
#include "dbi/dbi.h"
#include "options/optionswindow.h"
#include "playback/playbackwidget.h"
#include "albumtab/albumtab.h"

Q_DECLARE_METATYPE(QList<QString>)

class ssmp : public QMainWindow
{
	Q_OBJECT

public:
    ssmp(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ssmp();

    QSettings* settings;
    DBI* dbi;

private:
	Ui::ssmpClass ui;		
	optionsWindow* optWin;
	QThread* dbthread;	
    RecentAlbumsView* recentAlbs;

	inline void addPopupItem(QString name, QString type);
    void initPopup();
    void addAlbumToRecent(Alb);
	void addAlbsToRecent(QList<Alb>);
    void openSearchWindow(QString name, QMap<QString,QString> results);
	
private slots:
    void newAlbumTab(int alid);
    void changeSong(int songid);
    bool eventFilter(QObject* object, QEvent* e);
    bool openOptions();
    void addSongToNowPlaying(int sid);
    QWidget *openAlbumTab(int alid);

signals:
    void songChange(int songid);
};

#endif // SSMP_H
