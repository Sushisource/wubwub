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
	//Dbi updates
	connect(dbi, SIGNAL(atDir(QString)), optWin, SLOT(changeStatus(QString)));
	connect(dbi, SIGNAL(atDir(QString)), this, SLOT(updateRecentView()));
	//Open options windows
	connect(ui.optionsAction, SIGNAL(triggered()), this, SLOT(openOptions()));	
	//Save button on options window
	connect(optWin, SIGNAL(startSongParsing()), dbi, SLOT(processDirs()));

	//Run db thread
	dbthread = new QThread;
	dbthread->start();
	dbi->moveToThread(dbthread);	

	//Setup recent initial view	
	updateRecentView(5);
}

void ssmp::updateRecentView(int num)
{
	addAlbsToRecent(dbi->getNRecentAlbums(num));
}

void ssmp::addAlbsToRecent(QList<Alb> albs)
{	
	//Add the albums
	foreach(Alb al, albs)
	{	
		//Dont add if it's already in there
		if(recents.count() > 0 && al.artist+al.name == recents.front())
			continue;
		//add
		addAlbumToRecent(al);		
		//Remove bottom item if more than five in view
		if(ui.recentLayout->count() > 4)
		{
			QLayoutItem* child = ui.recentLayout->takeAt(4);		
			delete child->widget();
			delete child;		
			recents.pop_back();
		}
	}
}

void ssmp::addAlbumToRecent(Alb a)
{
	albumW* ap = new albumW(a.artist,a.name,a.year,a.tracks,a.imguri);
	ui.recentLayout->insertWidget(0, ap);
	ui.recentLayout->setAlignment(Qt::AlignTop);
	recents.push_front(a.artist + a.name);
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
