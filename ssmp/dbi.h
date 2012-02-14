#ifndef DBI_H
#define DBI_H

#include <QObject>
#include <QtSql>
#include <qsqlrelationaltablemodel.h>

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

	int addSong(DBItem song);
	int addAlbum(DBItem album);
	int addArtist(QString artist);
	void initDB();

private:
	QSqlDatabase db;
	
};

#endif // DBI_H
