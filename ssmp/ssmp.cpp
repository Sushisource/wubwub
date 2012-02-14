#include "ssmp.h"
#include "libdialog.h"

ssmp::ssmp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//Connnect library settings menu action to dialog
	connect(ui.libSettingsAction,SIGNAL(triggered()),this,SLOT(openLibDialog()));
	settings = new QSettings("ssmp_config.ini",QSettings::IniFormat);	

	//TODO: This shouldn't be hardcoded
	supportedFileFormats.push_back("mp3");
	supportedFileFormats.push_back("flac");
	supportedFileFormats.push_back("m4a");
	supportedFileFormats.push_back("ogg");	

	//Connect to database
	dbi = new DBI(this, "music.db");
	//If db not initialized, initialize it
	if(settings->value("dbinitted").toBool() == false)
	{
		dbi->initDB();
		settings->setValue("dbinitted",true);
	}
}

bool ssmp::openLibDialog()
{
	libraryDialog* ld = new libraryDialog(this, Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	ld->show();
	return true;	
}

ssmp::~ssmp()
{
	delete dbi;	
}
