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

enum SongCol {
sid,name,tracknum,album,artist,length,path,numplays,genre,year
};

//Database interface. Is a singleton which runs on it's own thread.
class DBI : public QObject
{
	Q_OBJECT

public:		
    ~DBI();
    enum searchFlag {All, ArtOnly, AlbOnly, SonOnly};
    //Song table's columns. ORMs are for bitches
    //*** KEEP IN SYNC WITH ENUM ABOVE***//
    QList<QString> songCols;

    void initDB();
    void refresh();
    static inline QString formatSeconds(int secs);
    QMap<QString, int> search(QString query, searchFlag s = DBI::All);
    QList<int> getTrackIdsFromAlbum(int alid);
    QList<QString> getTrackLengthsFromAlbum(int alid);
    QList<QString> getTrackColFromAlbum(int alid, int col);
    QList<QString> getNames(QList<int> ids, QString type);
    QString getTrackColFromSong(int sid, SongCol col);
    QString getSongNameFromId(int sid);
    QString getAlbumNameFromId(int alid);
    QString getArtistNameFromId(int arid);
    QString getArtistNameFromAlbumId(int alid);
    QString getArtistNameFromSongId(int sid);
    QString getImgUriFromAlbumId(int alid);
    QList<Alb> getNRecentAlbums(int n);

    static DBI& getInstance()
    {
        static DBI i;
        return i;
    }

public slots:
	int addSong(DBItem song);
	int addAlbum(DBItem album);
	int addArtist(QString artist);	
    void processDirs(QList<QString> dirlist);
    void processDir(QString dir);

signals:
	void atDir(const QString &s);
	void recentChange();

private:
    QSqlDatabase db;
    QThread* thread;
    QFileSystemWatcher* watcher;

	QString getOrFindAlbumArt(Alb a);
    QString sanitize(QString);
    QDateTime getPathLastMod(QString path);
    void updatePathLastMod(QString path);
    void subProcess(QString path, QDateTime rootlastmod);
    void deleteAlbumIfEmpty(int alid);

    DBI();
    DBI(DBI const&); //Don't impl
    void operator=(DBI const&); //Don't impl
};

#endif // DBI_H
