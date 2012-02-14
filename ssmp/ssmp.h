#ifndef SSMP_H
#define SSMP_H

#include <QtGui/QMainWindow>
#include <qsettings.h>
#include "ui_ssmp.h"
#include <functional>
#include <numeric>
#include "dbi.h"

class ssmp : public QMainWindow
{
	Q_OBJECT

public:
	ssmp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ssmp();

	QSettings* settings;
	std::vector<std::string> supportedFileFormats;
	DBI* dbi;

private:	
	Ui::ssmpClass ui;		
	
private slots:
	bool openLibDialog();
};

#endif // SSMP_H
