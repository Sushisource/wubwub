#ifndef WW_H
#define WW_H

#include <functional>
#include <numeric>
#include <memory>

#include <Qt>
#include <QMainWindow>
#include <QSettings>
#include <QKeyEvent>
#include <QQuickView>

#include "ui_wubwub.h"
#include "artistalbumsview.h"
#include "playlist.h"
#include "dbi/dbi.h"
#include "options/optionswindow.h"
#include "playback/playbackwidget.h"
#include "albumtab/albumtab.h"
#include "qmltab/recentalbumstab.h"
#include "qmltab/artistview.h"

Q_DECLARE_METATYPE(QList<QString>)

class wubwub : public QMainWindow
{
	Q_OBJECT

public:
    wubwub(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~wubwub();

    std::unique_ptr<QSettings> settings;
    DBI* dbi;

private:
    Ui::WWClass ui;
    std::unique_ptr<optionsWindow> optWin;
    std::unique_ptr<QThread> dbthread;	
    std::unique_ptr<RecentAlbumsTab> recentAlbs;

	inline void addPopupItem(QString name, QString type);
    void initPopup();
    void addAlbumToRecent(Alb);
	void addAlbsToRecent(QList<Alb>);
    void openSearchWindow(QString name, QMap<QString,QString> results);
	
private slots:
    void changeSong(int songid);
    bool eventFilter(QObject* object, QEvent* e);
    bool openOptions();
    void addSongToNowPlaying(int sid);
    void openAlbumTab(int alid);
    void openArtistTab(int arid);

signals:
    void songChange(int songid);
};

#endif // WW_H
