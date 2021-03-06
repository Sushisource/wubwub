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
    watcher = std::unique_ptr<QFileSystemWatcher>(new QFileSystemWatcher(this));
    connect(watcher.get(),
            SIGNAL(directoryChanged(QString)), SLOT(processDir(QString)));
}

QMap<QString, QPair<int, QString>> DBI::search(QString query, searchFlag)
{
	QSqlQueryModel qm;
	query = sanitize(query);
    QString quer = "select alid as id, 'album' as tbn, name from album where name like '%"+query+"%'";
	quer += " UNION";
    quer += " select arid as id, 'artist' as tbn, name from artist WHERE arid IN ("
        "SELECT arid "
          "FROM artist "
               "LEFT JOIN album ON artist.arid == album.artist "
         "WHERE album.alid IS NOT NULL "
    ") AND name LIKE '%"+query+"%'";
	quer += " UNION";
    quer += " select sid as id, 'song' as tbn, name from song where name like '%"+query+"%'";
    quer += " ORDER BY id COLLATE NOCASE ASC";
	qm.setQuery(quer);	
    QMap<QString, QPair<int, QString>> ret;
	for(int i = 0; i < qm.rowCount(); i++)
	{
        QString type = qm.record(i).field(1).value().toString();
        QPair<int, QString> id_name = QPair<int, QString>(
                    qm.record(i).field(0).value().toInt(),
                    qm.record(i).field(2).value().toString());
        ret.insertMulti(type, id_name);
	}
    return ret;
}

QList<Alb> DBI::extractAlbums(QSqlQueryModel* qm)
{
    QList<Alb> retme;
    for(int i = qm->rowCount()-1; i >= 0; i--)
    {
        Alb a;
        QSqlRecord qr = qm->record(i);
        a.name = qr.value("name").toString();
        if(a.name == "")  // For blank album fields.
            a.name = "Unknown Album";
        a.alid = qr.value("alid").toString();
        a.artist = getArtistNameFromId(qr.value("artist").toInt());
        a.tracks = getTrackColFromAlbum(qr.value(0).toInt(), 1);
        a.imguri = getOrFindAlbumArt(a);
        a.year = qr.value("year").toString();
        retme.append(a);
    }
    return retme;
}

QList<Alb> DBI::getNRecentAlbums(int n)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT * FROM album ORDER BY alid DESC LIMIT " +
                QString::number(n));
    return extractAlbums(&qm);
}

QList<Alb> DBI::getNewAlbumsSince(int alid)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT * FROM album WHERE alid > " +
                QString::number(alid) + " ORDER BY alid DESC LIMIT 5");
    return extractAlbums(&qm);
}

QList<Alb> DBI::getArtistAlbums(int arid)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT * FROM album WHERE artist=" +
                QString().setNum(arid) + " ORDER BY year DESC, alid DESC");
    return extractAlbums(&qm);
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
        filters << "*.jpg" << "*.png" << "*.jpeg" << "*.bmp" << "*.gif";
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

QString DBI::formatSeconds(int secs)
{
    int min = secs/60;
    QString m =  QString::number(min);
    QString s =  QString("%1").arg(secs - min*60, 2, 10, QChar('0'));
    return m+":"+s;
}

QList<int> DBI::getTrackIdsFromAlbum(int alid)
{
    QSqlQueryModel qm;
    QList<int> tracks;
    QString quer = "SELECT sid FROM song WHERE album=" + QString().setNum(alid);
    qm.setQuery(quer);
    for(int i = 0; i < qm.rowCount(); i++)
    {
        tracks.append(qm.record(i).value(0).toInt());
    }
    return tracks;
}

QList<QString> DBI::getTrackLengthsFromAlbum(int alid)
{
    QSqlQueryModel qm;
    QList<QString> tracks;
    QString quer = "SELECT length FROM song WHERE album=" + QString().setNum(alid);
    qm.setQuery(quer);
    for(int i = 0; i < qm.rowCount(); i++)
    {
        int t = qm.record(i).value(0).toInt();
        tracks.append(formatSeconds(t));
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

QString DBI::getTrackColFromSong(int sid, SongCol col)
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

QString DBI::getArtistNameFromAlbumId(int alid)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT artist FROM album WHERE alid=" + QString::number(alid));
    return getArtistNameFromId(qm.record(0).value(0).toInt());
}

QString DBI::getArtistNameFromSongId(int sid)
{
    int arid = this->getTrackColFromSong(sid, SongCol::artist).toInt();
    return getArtistNameFromId(arid);
}

QString DBI::getImgUriFromAlbumId(int alid)
{
    QSqlQueryModel qm;
    qm.setQuery("SELECT imguri FROM album WHERE alid=" + QString::number(alid));
    return qm.record(0).value(0).toString();
}

void DBI::processDir(QString dir)
{
    QList<QString> l;
    l.append(dir);
    processDirs(l);
}

void DBI::processDirs(QList<QString> dirlist, bool fromOptions)
{
    foreach(QString dirstr, dirlist)
    {
        subProcess(dirstr, fromOptions);
        watcher->addPath(dirstr);
    }
}

void DBI::subProcess(QString path, bool toplevel = false)
{
    emit atDir(path);
    QDateTime lastseen = getPathLastSeen(path);
    //First check if we even need to look in here
    if(!lastseen.isNull()) {
        if(QFileInfo(path).lastModified() < lastseen && !toplevel)
            return;
    }
    qDebug() << path;
    updatePathLastMod(path);

    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Dirs |
                  QDir::NoSymLinks | QDir::NoDotAndDotDot);
    QDirIterator di(dir);

    //TODO: Remove files from DB which no longer exist best way to do this is
    //maybe any time a song is accesed, delete it if it no longer exists.
    //Unfortunately I need a song object...

    album_added_during_proccess = false;
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
        else if(f.isDir())
        {
            subProcess(fpath);
        }
    }
    if(album_added_during_proccess) {
        emit recentChange();
    }
}

int DBI::addSong(DBItem sng)
{
    QString albartist, colnames = "", colbinds = "";
    int alkey = 0, arkey = 0, prevalid = -1, prevsid = -1;
    QSqlQuery q;

    //Check if we're actually going to replace a song record
    //and get what album it belonged to, at the end we check if it still has children
    QSqlQueryModel qm;
    QString qs = "SELECT sid, album FROM song WHERE path='"+sanitize(sng.strVals["path"])+"'";
    qm.setQuery(qs);
    if(qm.rowCount() > 0) {
        prevsid = qm.record(0).value(0).toInt();
        prevalid = qm.record(0).value(1).toInt();
    }


	//Annoying exception
	albartist = sng.strVals.value("albumartist", "unknown");
	sng.strVals.remove("albumartist");
	
    if(prevsid >= 0)
        sng.intVals.insert("sid", prevsid);
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

    //First try to determine if an aritst in the database matches the albumartist
    qs = "SELECT arid FROM artist WHERE name LIKE '";
    qs.append(sanitize(albartist) + "'");
    qm.setQuery(qs);
    if(qm.rowCount() > 0)
        arkey = qm.record(0).value("arid").toInt();
    else //add artist
    {
        arkey = addArtist(albartist);
    }

	//Now figure out album's ID
    qs = "SELECT * FROM album WHERE name like '";
    qs.append(sanitize(sng.strVals.value("album","unknown")) + "'");
    qs.append(" AND artist = " + QString::number(arkey));
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
    int lastinsert = q.lastInsertId().toInt();

    //Now check if the old album we may have replaced should be deleted
    if(prevalid > -1)
        deleteAlbumIfEmpty(prevalid);
    //It's also possible to add an album with no songs in it if the
    //album has songs by different arists but hasn't specified an albumartist
    //TODO: Fix this by grouping songs which have the same album name and were
    //loaded at the same time?
    deleteAlbumIfEmpty(alkey);

    return (execsuccess) ? lastinsert : -1;
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
    if(q.exec()) {
        album_added_during_proccess = true;
        return q.lastInsertId().toInt();
    }
    return -1;
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
    QSqlQueryModel qm;
    QString qs = "SELECT sid FROM song WHERE album=" + QString::number(alid);
    qm.setQuery(qs);
    if(qm.rowCount() > 0)
        return;
    qDebug() << alid << " deleted - nosongs";
    //No songs. Delete it.
    QSqlQuery q;
    q.prepare("DELETE FROM album WHERE alid=:alid");
    q.bindValue(":alid", alid);
    q.exec();
}

//TODO: This probably needs to fix more than just single quotes
QString DBI::sanitize(QString s)
{
    return s.replace("'","''");
}

QDateTime DBI::getPathLastSeen(QString path)
{
    QSqlQueryModel qm;
    QString quer = "SELECT lastmod FROM dirs WHERE path='" + sanitize(path)+"'";
    qm.setQuery(quer);
    if(qm.rowCount() > 0)
    {
        QString res = qm.record(0).value(0).toString();
        return QDateTime::fromString(res, Qt::ISODate);
    }
    return QDateTime();
}

void DBI::updatePathLastMod(QString path)
{
    QSqlQuery q;
    bool insert = false;
    if(getPathLastSeen(path).isNull())
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
    db.close();
}
