#include "dbi.h"

DBI::DBI() : QObject(NULL)
{	
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setHostName("localhost");
    db.setDatabaseName("music.db");
	//TODO: Fix possible 'missing expected db file' w/ try catch
    db.open();
    //This has to match the DB. ORMs are for bitches.
    songCols << "sid" << "name" << "tracknum" << "album" << "artist" << "length" << "path" << "numplays" << "genre" << "year";
    //Setup watcher on root dirs
    watcher = new QFileSystemWatcher();
    connect(watcher, SIGNAL(directoryChanged(QString)), SLOT(processDir(QString)));
}

QMap<QString, int> DBI::search(QString query, searchFlag s)
{
	QSqlQueryModel qm;
	query = sanitize(query);
    QString quer = "select alid as id, 'album' as tbn from album where name like '%"+query+"%'";
	quer += " UNION";
    quer += " select arid as id, 'artist' as tbn from artist where name like '%"+query+"%'";
	quer += " UNION";
    quer += " select sid as id, 'song' as tbn from song where name like '%"+query+"%'";
    quer += " ORDER BY id COLLATE NOCASE ASC";
	qm.setQuery(quer);	
    QMap<QString, int> ret;
	for(int i = 0; i < qm.rowCount(); i++)
	{
        QString type = qm.record(i).field(1).value().toString();
        ret.insertMulti(type, qm.record(i).field(0).value().toInt());
	}
    return ret;
}

QList<Alb> DBI::getNRecentAlbums(int n)
{
	QList<Alb> retme;
	QSqlQueryModel qm;
	qm.setQuery("SELECT * FROM album ORDER BY alid DESC LIMIT " + QString::number(n));
	for(int i = qm.rowCount()-1; i >= 0; i--)
	{
		Alb a;
		a.name = qm.record(i).value("name").toString();
		a.alid = qm.record(i).value("alid").toString();
		a.artist = getArtistNameFromID(qm.record(i).value("artist").toString());
        a.tracks = getTrackColFromAlbum(qm.record(i).value(0).toInt(), 1);
		a.imguri = getOrFindAlbumArt(a);
		a.year = qm.record(i).value("year").toString();
		retme.append(a);
	}
	return retme;
}

//Finds if the album has a stored imguri, finds one and sets it if not
QString DBI::getOrFindAlbumArt(Alb a)
{
	QSqlQueryModel qm;
	qm.setQuery("SELECT imguri FROM album WHERE alid=" + a.alid);
    QString curpath = qm.record(0).value(0).toString();
    if(curpath != "")
    {
        //Let's check if this is valid, and return it if it is, otherwise
        //continue updating
        if(QFile::exists(curpath))
            return qm.record(0).value(0).toString();
    }
	if(a.tracks.length() > 0) //TODO: Handle this better
	{
		qm.setQuery("SELECT path FROM song WHERE name='" + sanitize(a.tracks[0]) + "' AND album=" + a.alid);
		QString path = qm.record(0).value(0).toString();
		QDir d = QDir(path.left(path.lastIndexOf("/")));
		QStringList filters;
		filters << "*.jpg" << "*.png" << "*.jpeg" << "*.bmp";
		d.setNameFilters(filters);
		QStringList imgs = d.entryList(QDir::Hidden | QDir::Files);
		QString ret = "";
		foreach(QString s, imgs)
		{
			if(s.contains("folder",Qt::CaseInsensitive))
				ret = d.filePath(s);
			if(s.contains("cover",Qt::CaseInsensitive))
				ret = d.filePath(s);
		}
		//Add uri
		QSqlQuery qi;
		qi.prepare("UPDATE album SET imguri='"+ret+"' WHERE alid="+a.alid);
		qi.exec();
		return ret;
	}
	else
		return NULL;	
}

//TODO: This SQL should probably be in a resource file... for everything
void DBI::initDB()
{
    QSqlQuery q;
    /* Not sure this needs to be done
	q.exec("drop table artist");
	q.exec("drop table album");
	q.exec("drop table song");
    q.exec("drop table dirs");*/
    q.exec("PRAGMA foreign_keys = ON");
	q.exec("CREATE TABLE artist(arid integer primary key autoincrement, name text, "
		"UNIQUE(name) ON CONFLICT IGNORE)");
	q.exec("CREATE TABLE album(alid integer primary key autoincrement, name text, genre text, "
		"dateadded text, numsongs int, artist int, year text, imguri text, UNIQUE(artist, name) ON CONFLICT REPLACE, "
		"FOREIGN KEY(artist) REFERENCES artist(arid))");
	q.exec("CREATE TABLE song(sid integer primary key autoincrement, name text, tracknum int, "
		"album int, artist int, length int, path text, numplays int, genre text, year text, "
		"FOREIGN KEY(artist) REFERENCES artist(arid),"
		"FOREIGN KEY(album) REFERENCES album(alid), "
		"UNIQUE(name,tracknum,artist))");
    q.exec("CREATE TABLE dirs(dirid integer primary key autoincrement, path text, lastmod text)");
}

void DBI::refresh()
{
    QList<QString> dirs;
    QSqlQueryModel qm;
    qm.setQuery("SELECT path FROM dirs");
    for(int i = 0; i < qm.rowCount(); ++i)
    {
        dirs.append(qm.record(i).value(0).toString());
    }
    processDirs(dirs);
}

QList<int> DBI::getTrackIdsFromAlbum(int alid)
{
    QSqlQueryModel qm;
    QList<int> tracks;
    QString quer = "SELECT sid, name FROM song WHERE album=" + QString().setNum(alid);
    qm.setQuery(quer);
    for(int i = 0; i < qm.rowCount(); i++)
    {
        tracks.append(qm.record(i).value(0).toInt());
    }
    return tracks;
}

QList<QString> DBI::getTrackColFromAlbum(int alid, int col)
{
	QSqlQueryModel qm;
    QList<QString> tracks;
    QString quer = "SELECT "+songCols[col]+" FROM song WHERE album=" + QString().setNum(alid);
    qm.setQuery(quer);
    for(int i = 0; i < qm.rowCount(); i++)
	{
        tracks.append(qm.record(i).value(0).toString());
    }
    return tracks;
}

QList<QString> DBI::getNames(QList<int> ids, QString type)
{
    QList<QString> ret;
    if(type == "song")
    {
        foreach(int i, ids)
        {
            ret.append(getSongNameFromId(i));
        }
    }
    else if(type == "album")
    {
        foreach(int i, ids)
        {
            ret.append(getAlbumNameFromId(i));
        }
    }
    else if(type == "artist")
    {
        foreach(int i, ids)
        {
            ret.append(getArtistNameFromId(i));
        }
    }
    return ret;
}

QString DBI::getTrackColFromSong(int sid, int col)
{
    QSqlQueryModel qm;
    QString quer = "SELECT "+songCols[col]+" FROM song WHERE sid=" + QString().setNum(sid);
    qm.setQuery(quer);
    return qm.record(0).value(0).toString();
}

QString DBI::getSongNameFromId(int sid)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT name FROM song WHERE sid=" + QString::number(sid));
    return qm.record(0).value(0).toString();
}

QString DBI::getAlbumNameFromId(int alid)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT name FROM album WHERE alid=" + QString::number(alid));
    return qm.record(0).value(0).toString();
}

QString DBI::getArtistNameFromId(int arid)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT name FROM artist WHERE arid=" + QString::number(arid));
    return qm.record(0).value(0).toString();
}

QString DBI::getArtistNameFromID(QString arid)
{
	QSqlQueryModel qm;
	qm.setQuery("SELECT name FROM artist WHERE arid=" + arid);
    return qm.record(0).value(0).toString();
}

void DBI::processDir(QString dir)
{
    QList<QString> l;
    l.append(dir);
    processDirs(l);
}

void DBI::processDirs(QList<QString> dirlist)
{
    foreach(QString dirstr, dirlist)
    {
        QDateTime rootlastmod = getPathLastMod(dirstr);
        subProcess(dirstr, rootlastmod);
        updatePathLastMod(dirstr);
        watcher->addPath(dirstr);
    }
}

void DBI::subProcess(QString path, QDateTime rootlastmod)
{
    emit atDir(path);
    qDebug() << path;
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    QDirIterator di(dir);

    //TODO: Remove files from DB which no longer exist

    while(di.hasNext())
    {
        di.next();
        QString fpath = di.filePath();
        QFileInfo f = di.fileInfo();
        if(TagExtractor::isVaildAudioFile(f)) //Add or update song
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
        // Recur
        else if(f.isDir() && f.lastModified() >= rootlastmod)
        {
            //First check if we even need to look in here
            if(QFileInfo(fpath).lastModified() <= rootlastmod)
                return;
            subProcess(fpath, rootlastmod);
        }
    }
    //Update ui
    emit recentChange();
}

int DBI::addSong(DBItem sng)
{
	QString albartist, colnames = "", colbinds = "";
    int alkey = 0, arkey = 0, prevalid = -1;
    QSqlQuery q;

    //Check if we're actually going to replace a song record
    //and get what album it belonged to, at the end we check if it still has children
    QSqlQueryModel qm;
    QString qs = "SELECT album FROM song WHERE path='"+sanitize(sng.strVals["path"])+"'";
    qm.setQuery(qs);
    if(qm.rowCount() > 0)
        prevalid = qm.record(0).value(0).toInt();


	//Annoying exception
	albartist = sng.strVals.value("albumartist", "unknown");
	sng.strVals.remove("albumartist");
	
	colnames += QStringList(sng.strVals.keys()).join(", ");
	colbinds += ":" + QStringList(sng.strVals.keys()).join(", :");
	colnames += ", " + QStringList(sng.intVals.keys()).join(", ");
	colbinds += ", :" + QStringList(sng.intVals.keys()).join(", :");
    QString sql = "INSERT OR REPLACE INTO song (" + colnames + ") VALUES (" + colbinds + ")";
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
    qs = "SELECT * FROM album WHERE name like '";
	qs.append(sanitize(sng.strVals.value("album","unknown")));
	qs.append("'");
	qm.setQuery(qs);
    if(qm.rowCount() > 0)
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
	qs.append(sanitize(sng.strVals.value("artist", "unknown")));
	qs.append("'");
	qm.setQuery(qs);
	if(qm.rowCount() > 0) //If artist present, get id
		arkey = qm.record(0).value("arid").toInt();
	else //add artist
	{
		arkey = addArtist(sng.strVals.value("artist","unknown"));
	}

    //Execute
	q.bindValue(":album", alkey);
	q.bindValue(":artist", arkey);
    bool execsuccess = q.exec();

    //Now check if the old album we may have replaced should be deleted
    deleteAlbumIfEmpty(prevalid);

    return (execsuccess) ? q.lastInsertId().toInt() : -1;
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
	qs.append(sanitize(a.strVals.value("artist", "unknown")));
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

//If the given alid has no child songs remove it
void DBI::deleteAlbumIfEmpty(int alid)
{
    QSqlQuery q;
    QString qs = "SELECT sid FROM song WHERE album=:alid";
    q.prepare(qs);
    q.bindValue(":alid", alid);
    q.exec();
    if(q.next() && q.isValid())
        return;
    qDebug() << "Nosongs";
    //No songs. Delete it.
    q.prepare("DELETE FROM album WHERE alid=:alid");
    q.bindValue(":alid", alid);
    qDebug() << q.exec();
}


//TODO: This probably needs to fix more than just single quotes
QString DBI::sanitize(QString s)
{
    return s.replace("'","''");
}

QDateTime DBI::getPathLastMod(QString path)
{
    QSqlQueryModel qm;
    QDateTime lastmod;
    QString quer = "SELECT lastmod FROM dirs WHERE path='" + sanitize(path)+"'";
    qm.setQuery(quer);
    if(qm.rowCount() > 0)
    {
        QString res = qm.record(0).value(0).toString();
        lastmod = QDateTime::fromString(res, Qt::ISODate);
    }
    return lastmod;
}

void DBI::updatePathLastMod(QString path)
{
    QSqlQuery q;
    bool insert = false;
    if(getPathLastMod(path).isNull())
        insert = true;
    if(insert)
        q.prepare("INSERT INTO dirs (path, lastmod) VALUES (:path, :lm)");
    else
        q.prepare("UPDATE dirs SET lastmod=:lm WHERE path=:path");
    q.bindValue(":path", path);
    q.bindValue(":lm", QDateTime::currentDateTime());
    q.exec();
}

DBI::~DBI()
{

}
