#ifndef RECENTALBUMSVIEW_H
#define RECENTALBUMSVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include "dbi.h"
class ssmp; //TODO: Fucking circular depends
struct Alb;

class RecentAlbumsView : public QGraphicsView
{
	Q_OBJECT

public:
    RecentAlbumsView(QWidget *parent, int listsize = 5);
	~RecentAlbumsView();

public slots:
	void update(int howmany = 5);

signals:
    void addAlbsToNowPlaying(QList<int> alids);

private:
    DBI* db;
	QGraphicsScene* scene;	
	QList<QString> recents;
    QList<QGraphicsTextItem*> descriptions;
    QList<QGraphicsPixmapItem*> covers;
    QList<QGraphicsRectItem*> backgrounds;
    QGraphicsRectItem* bottomfade;
    QList<QGraphicsSvgItem*> plusbuttons;
    int rnum;
    static const QFont albFont;
    void addAlbsToRecent(QList<Alb> albs);
    void resizeEvent(QResizeEvent* e = NULL);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
};

#endif // RECENTALBUMSVIEW_H
