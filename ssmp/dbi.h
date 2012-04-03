#ifndef DBI_H
#define DBI_H

#include <QObject>
#include <QtSql>
#include <QPair>
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
	QString name, artist, imguri, year, alid;
	QList<QString> tracks;
};

//Database interface. Is a singleton which runs on it's own thread.
class DBI : public QObject
{
	Q_OBJECT

public:		
    ~DBI();
    enum searchFlag {All, ArtOnly, AlbOnly, SonOnly};

    void initDB();
    void addDirs2Lib(QList<QString> dirs);
	QMap<QString, QString> search(QString query, searchFlag s = DBI::All);
    QList<QPair<int, QString>> getTracksFromAlbum(int alid);
    QString getSongNameFromId(int sid);
    QList<Alb> getNRecentAlbums(int n);

    static DBI& getInstance()
    {
        static DBI i;
        return i;
    }
    static QList<QString> extractTracks(QList<QPair<int, QString>> ql);
    static QList<int> extractIds(QList<QPair<int, QString>> ql);

public slots:
	int addSong(DBItem song);
	int addAlbum(DBItem album);
	int addArtist(QString artist);	
	void processDirs();

signals:
	void atDir(const QString &s);
	void recentChange();

private:
	QSqlDatabase db;
	QThread* thread;
	QList<QString> dirlist;	

	QString getArtistNameFromID(QString arid);	
	QString getOrFindAlbumArt(Alb a);
    QString sanitize(QString);

    DBI();
    DBI(DBI const&); //Don't impl
    void operator=(DBI const&); //Don't impl
};

#endif // DBI_H
