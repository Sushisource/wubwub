#ifndef SSMP_H
#define SSMP_H

#include <QtGui/QMainWindow>
#include <qsettings.h>
#include <qthreadpool.h>
#include <qtreewidget.h>
#include <QKeyEvent>
#include <functional>
#include <numeric>
#include "ui_ssmp.h"
#include "dbi.h"
#include "optionswindow.h"
#include "recentalbumsview.h"
#include "playlist.h"
#include "playbackmgr.h"

Q_DECLARE_METATYPE(QList<QString>)

class ssmp : public QMainWindow
{
	Q_OBJECT

public:
	ssmp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ssmp();

	QSettings* settings;
	QMap<int, QWidget*> tabs;
    DBI* dbi;

private:
	Ui::ssmpClass ui;		
	optionsWindow* optWin;
	QThread* dbthread;	
	QTreeWidget* popup;
	QTimer* searchTimer;
	QColor disabledColor;
	QList<QString> searchtypes;
    RecentAlbumsView* recentAlbs;
    PlaybackMgr* playback;

	inline void addPopupItem(QString name, QString type);
    void initPopup();
    void addAlbumToRecent(Alb);
	void addAlbsToRecent(QList<Alb>);
	void openSearchWindow(QString name, QMap<QString,QString> results);	
	
private slots:
    void autoSuggest();
    bool eventFilter(QObject* object, QEvent* e);
	bool openOptions();
};

#endif // SSMP_H
