#ifndef RECENTALBUMSVIEW_H
#define RECENTALBUMSVIEW_H

#include <QGraphicsView>
#include "albumview.h"
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
	QList<AlbumView*> recentps;
	void addAlbsToRecent(QList<Alb> albs);
	void resizeEvent(QResizeEvent * e);
	
};

#endif // RECENTALBUMSVIEW_H
