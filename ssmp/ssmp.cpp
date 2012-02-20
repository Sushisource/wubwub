#include "ssmp.h"

ssmp::ssmp(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags)
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
	connect(dbi, SIGNAL(atDir(QString)), optWin, SLOT(changeStatus(QString)));
	connect(ui.optionsAction,SIGNAL(triggered()),this,SLOT(openOptions()));

	//Run db thread
	dbthread = new QThread;
	dbthread->start();
	dbi->moveToThread(dbthread);

	//Setup recent view	
	addAlbsToRecent(dbi->getNRecentAlbums(5));
}

void ssmp::addAlbsToRecent(QList<Alb> albs)
{
	foreach(Alb al, albs)
	{	
		addAlbumToRecent(al);
	}
}

void ssmp::addAlbumToRecent(Alb a)
{
	albumW* ap = new albumW(a.artist,a.name,a.year,a.tracks,a.imguri);
	ui.recentLayout->insertWidget(0, ap);
	ui.recentLayout->setAlignment(Qt::AlignTop);
}

bool ssmp::openOptions()
{		
	optWin->show();
	return true;	
}

ssmp::~ssmp()
{
	delete dbi;	
	delete dbthread;
}
