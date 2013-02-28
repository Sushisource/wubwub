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
#include "prettytext.h"

class AlbumTab : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AlbumTab(int alid, QWidget *parent = 0);

    void addTrack(QFont trackfont, QList<QString> tracknums, QGraphicsDropShadowEffect* shadow, int i, QPen oline, QList<QString> tracknames);
    void addTracks(int alid, DBI* db);
    ~AlbumTab();

signals:
    void clearPlaylist();
    void playSongFromAlbum(int alid, int songid);
    
public slots:

private:
    int album_id;

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
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // ALBUMTAB_H
