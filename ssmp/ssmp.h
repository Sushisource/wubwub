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
#include "dbi/dbi.h"
#include "options/optionswindow.h"
#include "recentalbumsview.h"
#include "playlist.h"
#include "playback/playbackmgr.h"
#include "albumtab.h"

Q_DECLARE_METATYPE(QList<QString>)

class ssmp : public QMainWindow
{
	Q_OBJECT

public:
	ssmp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ssmp();

    QSettings* settings;
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

	inline void addPopupItem(QString name, QString type);
    void initPopup();
    void addAlbumToRecent(Alb);
	void addAlbsToRecent(QList<Alb>);
    void openSearchWindow(QString name, QMap<QString,QString> results);
    QWidget *openAlbumTab(int alid);
	
private slots:
    void autoSuggest();
    void newAlbumTab(int alid);
    bool eventFilter(QObject* object, QEvent* e);
	bool openOptions();
};

#endif // SSMP_H
