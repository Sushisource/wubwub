#ifndef RECENTALBUMSVIEW_H
#define RECENTALBUMSVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include "dbi/dbi.h"
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
    void openAlbumTab(int alid);

private:
    DBI* db;
	QGraphicsScene* scene;	
	QList<QString> recents;
    QList<QGraphicsTextItem*> descriptions;
    QList<QGraphicsPixmapItem*> covers;
    QList<QGraphicsRectItem*> backgrounds;
    QGraphicsRectItem* bottomfade;
    QList<QGraphicsSvgItem*> plusbuttons;
    QList<QGraphicsSvgItem*> tabbuttons;
    int rnum;
    enum DataKeys {ALID, BTNTYPE};
    enum BtnTypes {PLUS, TAB};
    static const QFont albFont;
    void addAlbsToRecent(QList<Alb> albs);
    void resizeEvent(QResizeEvent* e = NULL);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent* event);
};

#endif // RECENTALBUMSVIEW_H
