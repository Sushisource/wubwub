#ifndef SSMP_H
#define SSMP_H

#include <QtGui/QMainWindow>
#include <qsettings.h>
#include <qthreadpool.h>
#include <qtreewidget.h>
#include <QKeyEvent>
#include "ui_ssmp.h"
#include <functional>
#include <numeric>
#include "dbi.h"
#include "optionswindow.h"
#include "albumW.h"

Q_DECLARE_METATYPE(QList<QString>);

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
	QList<QString> recents;
	QTreeWidget* popup;
	QTimer* searchTimer;
	QColor disabledColor;
	QList<QString> searchtypes;
	
	inline void addPopupItem(QString name, QString type);
	void addAlbumToRecent(Alb);
	void addAlbsToRecent(QList<Alb>);
	void openSearchWindow(QString name, QMap<QString,QString> results);	
	
private slots:
	void autoSuggest();
	bool eventFilter(QObject* object, QEvent* e);
	void updateRecentView(int n = 1);
	bool openOptions();
};

#endif // SSMP_H
