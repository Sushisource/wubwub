#include "ssmp.h"

RecentAlbumsView::RecentAlbumsView(ssmp* parent, int listsize)
	: QGraphicsView(parent)
{
	scene = new QGraphicsScene(this);
	this->parent = parent;
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setScene(scene);
}

void RecentAlbumsView::update(int howmany)
{
	addAlbsToRecent(parent->dbi->getNRecentAlbums(howmany));	
}

void RecentAlbumsView::resizeEvent(QResizeEvent* e)
{
    if(recentps.count() < 1) //If there's nothing to resize, don't bother.
        return;
    int padding = 5;
    int siz  = (this->geometry().height() - padding*(1+recentps.count())) / recentps.count();
	int i = 0;
	foreach(AlbumView* al, recentps)
    {
        int ypos = i*siz;
        if(i > 0) ypos += padding;
        al->setGeometry(0, ypos, this->geometry().width(), siz);
        ++i;
	}
}

void RecentAlbumsView::addAlbsToRecent(QList<Alb> albs)
{
	//Add the albums
    foreach(Alb al, albs)
    {	
        //Dont add if it's already in there
        if(recents.count() > 0 && recents.contains(al.artist+al.name))
            continue;
        //add
        AlbumView* ap = new AlbumView(al.artist,al.name,al.year,al.tracks,al.imguri);
		ap->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Ignored);
		scene->addItem(ap);
		recentps.push_front(ap);
		recents.push_front(al.artist + al.name);
        //Remove bottom item if more than five in view
		//TODO: Parameterize # in view
        if(recents.count() > 5)
        {
			scene->removeItem(recentps.back());			
			recentps.pop_back();
            recents.pop_back();
        }
    }
}

RecentAlbumsView::~RecentAlbumsView()
{

}
