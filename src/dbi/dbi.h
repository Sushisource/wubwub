#ifndef DBI_H
#define DBI_H

#include <QObject>
#include <QtSql>
#include <QPair>
#include <memory>
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
    //Song table's columns. ORMs are for bitches
    //*** KEEP IN SYNC WITH ENUM ABOVE***//
    QList<QString> songCols;

    static inline QString formatSeconds(int secs);
    enum searchFlag {All, ArtOnly, AlbOnly, SonOnly};
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
    QList<Alb> getNewAlbumsSince(int alid);
    QList<Alb> getArtistAlbums(int arid);

    static DBI& getInstance()
    {
        static DBI i;
        return i;
    }

public slots:
    void refresh();
    void initDB();
    int addSong(DBItem song);
	int addAlbum(DBItem album);
	int addArtist(QString artist);	
    void processDir(QString dir);
    void processDirs(QList<QString> dirlist, bool fromOptions=false);

signals:
	void atDir(const QString &s);
    void recentChange();

private:
    QSqlDatabase db;
    std::unique_ptr<QFileSystemWatcher> watcher;
	QString getOrFindAlbumArt(Alb a);
    QString sanitize(QString);
    QDateTime getPathLastSeen(QString path);
    QList<Alb> extractAlbums(QSqlQueryModel *qm);
    //FIXME: Kinda sucks but I don't know how else to do this as easily
    bool album_added_during_proccess;

    void updatePathLastMod(QString path);
    void subProcess(QString path, bool);
    void deleteAlbumIfEmpty(int alid);

    DBI();
    DBI(DBI const&); //Don't impl
    void operator=(DBI const&); //Don't impl
};

#endif // DBI_H
