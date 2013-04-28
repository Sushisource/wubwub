#ifndef QMLALBUMTAB_H
#define QMLALBUMTAB_H
#include "qmltab.h"

class QmlAlbumTab : public QmlTab
{
    Q_OBJECT
public:
    explicit QmlAlbumTab(QString qmlfile, QWidget *parent = 0);
    
protected:
    void addAlbum(Alb album);
    void addAlbums(QList<Alb> album);
    DBI* db;

signals:
    
public slots:
    
};

#endif // QMLALBUMTAB_H
