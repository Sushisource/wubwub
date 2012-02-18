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

class DBI : public QObject
{
	Q_OBJECT

public:		
	DBI(QObject* parent = 0, QString name = "music.db");
	~DBI();

	void initDB();
	void addDirs2Lib(QList<QString> dirs);

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
	
};

#endif // DBI_H
