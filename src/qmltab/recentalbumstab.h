#ifndef RECENTALBUMSVIEW_H
#define RECENTALBUMSVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include "dbi/dbi.h"
#include "qmltab/albumview.h"
#include "playlist.h"

class RecentAlbumsTab : public AlbumView
{
	Q_OBJECT

public:
    RecentAlbumsTab(QWidget *parent,
                    QString qmlfile = "qrc:/qml/qml/albumview.qml");
    ~RecentAlbumsTab();
    void addAlbum(Alb album);

public slots:
    void update();

private:
    int mostrecentAlb;
};

#endif // RECENTALBUMSVIEW_H
