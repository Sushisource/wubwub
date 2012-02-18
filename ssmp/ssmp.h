#ifndef SSMP_H
#define SSMP_H

#include <QtGui/QMainWindow>
#include <qsettings.h>
#include <qthreadpool.h>
#include "ui_ssmp.h"
#include <functional>
#include <numeric>
#include "dbi.h"
#include "optionswindow.h"

Q_DECLARE_METATYPE(QList<QString>);

class ssmp : public QMainWindow
{
	Q_OBJECT

public:
	ssmp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ssmp();

	QSettings* settings;	
	DBI* dbi;

public slots:
	void alerter();

private:	
	Ui::ssmpClass ui;		
	optionsWindow* optWin;
	QThread* dbthread;
	
private slots:
	bool openOptions();
};

#endif // SSMP_H
