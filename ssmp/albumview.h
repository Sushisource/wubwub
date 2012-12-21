#ifndef ALBUMVIEW_H
#define ALBUMVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include "dbi/dbi.h"
class ssmp; //TODO: Fucking circular depends
struct Alb;

class AlbumView : public QGraphicsView
{
	Q_OBJECT

public:
    AlbumView(QWidget *parent);
    ~AlbumView();

signals:
    void addAlbsToNowPlaying(QList<int> alids);
    void openAlbumTab(int alid);

protected:
    DBI* db;
    void addAlbs(QList<Alb> albs);
    void resizeEvent(QResizeEvent* e = NULL);
    int maxAlbs;

private:
	QGraphicsScene* scene;	
    QList<QString> alids;
    QList<QGraphicsTextItem*> descriptions;
    QList<QGraphicsPixmapItem*> covers;
    QList<QGraphicsRectItem*> backgrounds;
    QGraphicsRectItem* bottomfade;
    QList<QGraphicsSvgItem*> plusbuttons;
    QList<QGraphicsSvgItem*> tabbuttons;
    int rnum;
    int znum; // Most recent Z depth of albums being displayed
    enum DataKeys {ALID, BTNTYPE};
    enum BtnTypes {PLUS, TAB};
    static const QFont albFont;
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
};

#endif // ALBUMVIEW_H
