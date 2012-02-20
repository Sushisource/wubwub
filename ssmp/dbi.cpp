#include "dbi.h"

DBI::DBI(QObject* parent, QString name) : QObject(parent)
{	
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setHostName("localhost");
	db.setDatabaseName(name);
	//TODO: Fix possible 'missing expected db file' w/ try catch
	db.open();
}

QList<Alb> DBI::getNRecentAlbums(int n)
{
	QList<Alb> retme;
	QSqlQueryModel qm;
	qm.setQuery("SELECT * FROM album ORDER BY dateadded DESC LIMIT " + QString::number(n));
	for(int i = qm.rowCount()-1; i >= 0; i--)
	{
		Alb a;
		a.name = qm.record(i).value("name").toString();
		a.artist = getArtistNameFromID(qm.record(i).value("artist").toString());
		a.imguri = "";
		a.year = qm.record(i).value("year").toString();
		a.tracks = getTracksFromAlbum(qm.record(i).value(0).toString());
		retme.append(a);
	}
	return retme;
}

//TODO: This SQL should probably be in a resource file... for everything
void DBI::initDB()
{
	QSqlQuery q;
	q.exec("drop table artist");
	q.exec("drop table album");
	q.exec("drop table song");
	q.exec("PRAGMA foreign_keys = ON");
	q.exec("CREATE TABLE artist(arid integer primary key autoincrement, name text, "
		"UNIQUE(name) ON CONFLICT IGNORE)");
	q.exec("CREATE TABLE album(alid integer primary key autoincrement, name text, genre text, "
		"dateadded text, numsongs int, artist int, year text, UNIQUE(artist, name) ON CONFLICT REPLACE, "
		"FOREIGN KEY(artist) REFERENCES artist(arid))");
	q.exec("CREATE TABLE song(sid integer primary key autoincrement, name text, tracknum int, "
		"album int, artist int, length int, path text, numplays int, genre text, year text, "
		"FOREIGN KEY(artist) REFERENCES artist(arid),"
		"FOREIGN KEY(album) REFERENCES album(alid), "
		"UNIQUE(name,tracknum,artist))");	
}

void DBI::processDirs()
{
	foreach(QString dirstr, dirlist)
	{
		QDir dir(dirstr);
		dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
		QDirIterator di(dir, QDirIterator::Subdirectories);	

		while(di.hasNext())
		{
			di.next();		
			QString fpath = di.filePath();	
			QFileInfo f = di.fileInfo();
			if(TagExtractor::isAudioFile(f))//Add this song to the database
			{
				//We'll store tag information in these:
				QMap<QString, QString> stmap;
				QMap<QString, int> itmap;
				//Extract tags
				TagExtractor::extractTag(fpath, &stmap, &itmap);
				//Add collected info to db
				DBItem s;
				s.strVals = stmap;
				s.intVals = itmap;
				addSong(s);
			}
			// Update ui
			else if(f.isDir())
				emit atDir(fpath);				
		}
	}
	dirlist.empty();
}

QList<QString> DBI::getTracksFromAlbum(QString alid)
{
	QSqlQueryModel qm;
	QList<QString> tracks;
	qm.setQuery("SELECT name, length FROM song WHERE album=" + alid);
	for(int i = 0; i < qm.rowCount(); i++)
	{
		tracks.append(qm.record(i).value(0).toString() + "\t" + qm.record(i).value(1).toString());
	}
	return tracks;
}

QString DBI::getArtistNameFromID(QString arid)
{
	QSqlQueryModel qm;
	qm.setQuery("SELECT name FROM artist WHERE arid=" + arid);
	return qm.record(0).value("name").toString();
}

//Sets up which dirs the run method will be adding
void DBI::addDirs2Lib(QList<QString> dirs)
{	
	dirlist.append(dirs);	
}

int DBI::addSong(DBItem sng)
{
	QString albartist, colnames = "", colbinds = "";
	int alkey = 0, arkey = 0;
	QSqlQuery q;

	//Annoying exception
	albartist = sng.strVals.value("albumartist", "unknown");
	sng.strVals.remove("albumartist");
	
	colnames += QStringList(sng.strVals.keys()).join(", ");
	colbinds += ":" + QStringList(sng.strVals.keys()).join(", :");
	colnames += ", " + QStringList(sng.intVals.keys()).join(", ");
	colbinds += ", :" + QStringList(sng.intVals.keys()).join(", :");
	QString sql = "INSERT INTO song (" + colnames + ") VALUES (" + colbinds + ")";
	q.prepare(sql);

	//Extract string values
	QMapIterator<QString, QString> sm(sng.strVals);
	while (sm.hasNext()) {
		sm.next();
		q.bindValue(":"+sm.key(), sm.value()); 
	}
	//Extract int values
	QMapIterator<QString, int> i(sng.intVals);
	while (i.hasNext()) {
		i.next();
		q.bindValue(":"+i.key(), i.value());
	}

	//Now figure out album's ID
	QSqlQueryModel qm;
	QString qs = "SELECT * FROM album WHERE name like '";
	qs.append(sng.strVals.value("album","unknown"));
	qs.append("'");
	qm.setQuery(qs);
	if(qm.rowCount() > 0) //If album present, get id
		alkey = qm.record(0).value("alid").toInt();
	else //add album
	{
		DBItem a;
		a.strVals["name"] = sng.strVals.value("album", "unknown");
		a.strVals["artist"] = albartist;
		a.intVals["year"] = sng.intVals.value("year", QDateTime::currentDateTime().date().year());
		a.strVals["dateadded"] = QDateTime::currentDateTime().toString();
		a.strVals["genre"] = sng.strVals.value("genre", "unknown");
		alkey = addAlbum(a);
	}

	//Now figure out artist's ID
	qs = "SELECT * FROM artist WHERE name like '";
	qs.append(sng.strVals.value("artist", "unknown"));
	qs.append("'");
	qm.setQuery(qs);
	if(qm.rowCount() > 0) //If artist present, get id
		arkey = qm.record(0).value("arid").toInt();
	else //add artist
	{
		arkey = addArtist(sng.strVals.value("artist","unknown"));
	}

	q.bindValue(":album", alkey);
	q.bindValue(":artist", arkey);
	return (q.exec()) ? q.lastInsertId().toInt() : -1;
}

int DBI::addAlbum(DBItem a)
{
	int arkey = 0;
	QSqlQuery q;
	q.prepare("INSERT INTO album (name, genre, dateadded, numsongs, artist, year)"
		"VALUES (:name, :genre, :dateadded, :numsongs, :artist, :year)");

	//Extract string values
	QMapIterator<QString, QString> sm(a.strVals);
	while (sm.hasNext()) {
		sm.next();
		q.bindValue(":"+sm.key(), sm.value());		 
	}
	//Extract int values
	QMapIterator<QString, int> i(a.intVals);
	while (i.hasNext()) {
		i.next();
		q.bindValue(":"+i.key(), i.value());		 
	}

	//Now figure out artist's ID
	QSqlQueryModel qm;
	QString qs = "SELECT * FROM artist WHERE name like '";
	qs.append(a.strVals.value("artist", "unknown"));
	qs.append("'");
	qm.setQuery(qs);
	if(qm.rowCount() > 0) //If artist present, get id
		arkey = qm.record(0).value("arid").toInt();
	else //add artist
	{
		arkey = addArtist(a.strVals.value("artist","unknown"));
	}	
	q.bindValue(":artist",arkey);
	return (q.exec()) ? q.lastInsertId().toInt() : -1;
}

int DBI::addArtist(QString a)
{
	QSqlQuery q;
	q.prepare("INSERT INTO artist (name) VALUES (:name)");
	q.bindValue(":name", a);
	return (q.exec()) ? q.lastInsertId().toInt() : -1;
}

DBI::~DBI()
{

}
