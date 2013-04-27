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

public slots:
    void update();
    void newAlbs(QList<Alb> albs);

private:
};

#endif // RECENTALBUMSVIEW_H
