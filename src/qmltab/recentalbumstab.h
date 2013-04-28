#ifndef RECENTALBUMSVIEW_H
#define RECENTALBUMSVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include "dbi/dbi.h"
#include "albumview.h"
#include "qmltab/qmltab.h"

class RecentAlbumsTab : public QmlTab
{
	Q_OBJECT

public:
    RecentAlbumsTab(QWidget *parent,
                    QString qmlfile = "qrc:/qml/qml/albumview.qml");
    ~RecentAlbumsTab();
    void addAlbum(Alb album);
    void addAlbums(QList<Alb> albums);

public slots:
    void update();

private:
    DBI* db;
    int mostrecentAlb;
};

#endif // RECENTALBUMSVIEW_H
