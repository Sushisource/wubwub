#include "ssmp.h"
#include "libdialog.h"

ssmp::ssmp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	//Connnect library settings menu action to dialog
	connect(ui.libSettingsAction,SIGNAL(triggered()),this,SLOT(openLibDialog()));
	settings = new QSettings("ssmp_config.ini",QSettings::IniFormat);
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setHostName("localhost");
	db.setDatabaseName("music.db");
	db.open();

	//TODO: This shouldn't be hardcoded
	supportedFileFormats.push_back("mp3");
	supportedFileFormats.push_back("flac");

	//If db not initialized, initialize it
	if(settings->value("dbinitted") == QVariant())
		initDB();
}

void ssmp::initDB()
{
	QSqlQuery q;
	q.exec("drop table artist");
	q.exec("drop table album");
	q.exec("drop table song");
	q.exec("create table artist(id int primary key, name text)");
	q.exec("create table album(id int primary key, name text, genre text, dateadded text, numsongs int, artist int)");
	q.exec("create table song(id int primary key, name text, tracknum int, album int, artist int)");
	settings->setValue("dbinitted",true);
}

bool ssmp::openLibDialog()
{
	libraryDialog* ld = new libraryDialog(this);
	ld->show();
	return true;	
}

ssmp::~ssmp()
{

}
