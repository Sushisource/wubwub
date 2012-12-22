#ifndef RECENTALBUMSVIEW_H
#define RECENTALBUMSVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include "dbi/dbi.h"
#include "albumview.h"

class RecentAlbumsView : public AlbumView
{
	Q_OBJECT

public:
    RecentAlbumsView(QWidget *parent);
	~RecentAlbumsView();

public slots:
    void update();

private:
};

#endif // RECENTALBUMSVIEW_H
