#ifndef ALBUMTAB_H
#define ALBUMTAB_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include <QVBoxLayout>
#include <QApplication>
#include <QMouseEvent>
#include "dbi/dbi.h"

class AlbumTab : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AlbumTab(int alid, QWidget *parent = 0);

    void addTrack(QFont trackfont, QList<QString> tracknums, QGraphicsDropShadowEffect* shadow, int i, QPen oline, QList<QString> tracknames);
    void addTracks(int alid, DBI* db);

signals:
    void clearPlaylist();
    
public slots:

private:
    QGraphicsScene* scene;
    QGraphicsSimpleTextItem* title;
    //Fonts
    QFont titlefont;
    QFont trackfont;
    QGraphicsPixmapItem* cover;
    QGraphicsRectItem* bgrect;
    enum DataKeys {TRACKID};
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // ALBUMTAB_H
