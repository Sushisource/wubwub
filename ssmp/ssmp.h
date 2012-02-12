#ifndef SSMP_H
#define SSMP_H

#include <QtGui/QMainWindow>
#include <qsettings.h>
#include "ui_ssmp.h"
#include <QtSql>
#include <functional>
#include <numeric>

class ssmp : public QMainWindow
{
	Q_OBJECT

public:
	ssmp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ssmp();

	QSettings* settings;
	std::vector<std::string> supportedFileFormats;

private:	
	Ui::ssmpClass ui;
	QSqlDatabase db;
	void initDB();
	
private slots:
	bool openLibDialog();
};

#endif // SSMP_H
