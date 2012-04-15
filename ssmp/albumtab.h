#ifndef ALBUMTAB_H
#define ALBUMTAB_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include <QVBoxLayout>
#include <QApplication>
#include "dbi/dbi.h"

class AlbumTab : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AlbumTab(int alid, QWidget *parent = 0);

    void addTrack(QFont trackfont, QList<QString> tracknums, QGraphicsDropShadowEffect* shadow, int i, QPen oline, QList<QString> tracknames);
    void addTracks(int alid, DBI* db);
signals:
    
public slots:

private:
    QGraphicsScene* scene;
    QGraphicsSimpleTextItem* title;
    //Fonts
    QFont titlefont;
    QFont trackfont;
    QGraphicsPixmapItem* cover;
    QGraphicsRectItem* bgrect;
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);

};

#endif // ALBUMTAB_H
