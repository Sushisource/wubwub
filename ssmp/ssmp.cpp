#include "ssmp.h"

ssmp::ssmp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	settings = new QSettings("ssmp_config.ini",QSettings::IniFormat);
	//Instantiate options menu
	optWin = new optionsWindow(this);		

	//Connect to database	
	dbi = new DBI(0, "music.db");	
	
	//If db not initialized, initialize it
	if(settings->value("dbinitted").toBool() == false)
	{
		dbi->initDB();
		settings->setValue("dbinitted",true);
	}

	//Link up stuff
	connect(optWin,SIGNAL(startSongParsing()),dbi,SLOT(processDirs()));
	connect(dbi, SIGNAL(atDir(QString)), optWin, SLOT(changeStatus(QString)), Qt::QueuedConnection);
	connect(ui.optionsAction,SIGNAL(triggered()),this,SLOT(openOptions()));

	//Run db thread
	dbthread = new QThread;
	dbthread->start();
	dbi->moveToThread(dbthread);
}

bool ssmp::openOptions()
{		
	optWin->show();
	return true;	
}

void ssmp::alerter()
{
	qDebug() << "DICKS!!!";
}

ssmp::~ssmp()
{
	delete dbi;	
	delete dbthread;
}
