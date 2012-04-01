#ifndef RECENTALBUMSVIEW_H
#define RECENTALBUMSVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
class ssmp; //Fucking circular depends

class RecentAlbumsView : public QGraphicsView
{
public:
	RecentAlbumsView(ssmp* parent, int listsize = 5);
	~RecentAlbumsView();

public slots:
	void update(int howmany = 5);

private:
	ssmp* parent;
	QGraphicsScene* scene;	
	QList<QString> recents;
    QList<QGraphicsTextItem*> descriptions;
    QList<QGraphicsPixmapItem*> covers;
    QList<QGraphicsRectItem*> backgrounds;
    static const QFont albFont;
    void addAlbsToRecent(QList<Alb> albs);
    void resizeEvent(QResizeEvent * e);
	
};

#endif // RECENTALBUMSVIEW_H
