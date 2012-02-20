#ifndef DBI_H
#define DBI_H

#include <QObject>
#include <QtSql>
#include <qrunnable.h>
#include <qthread.h>
#include "TagExtractor.h"

struct DBItem 
{
	QMap<QString,int> intVals;
	QMap<QString,QString> strVals;	
};

struct Alb
{
	QString name, artist, imguri, year;
	QList<QString> tracks;
};

class DBI : public QObject
{
	Q_OBJECT

public:		
	DBI(QObject* parent = 0, QString name = "music.db");
	~DBI();

	void initDB();
	void addDirs2Lib(QList<QString> dirs);
	QList<Alb> getNRecentAlbums(int n);

public slots:
	int addSong(DBItem song);
	int addAlbum(DBItem album);
	int addArtist(QString artist);	
	void processDirs();

signals:
	void atDir(const QString &s);

private:
	QSqlDatabase db;
	QThread* thread;
	QList<QString> dirlist;

	QString getArtistNameFromID(QString arid);	
	QList<QString> getTracksFromAlbum(QString alid);
};

#endif // DBI_H
