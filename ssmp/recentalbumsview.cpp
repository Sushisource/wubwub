#include "ssmp.h"

//Setup some constants
const QFont RecentAlbumsView::albFont = QFont("Arial", 11);

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
    if(recents.count() < 1) //If there's nothing to resize, don't bother.
        return;
    int padding = 5;
    float siz  = (this->geometry().height() - padding*(1+recents.count())) / recents.count();
    int buttonsiz = plusbuttons[0]->boundingRect().width();
    float scale = siz / 200.0;
    for(int i = 0; i < recents.count(); i++)
    {
        int ypos = i*(siz + padding) + padding;
        int albumxpos = this->width()-scale*200;
        backgrounds[i]->setRect(0,ypos,albumxpos,siz);
        descriptions[i]->setPos(0,ypos);
        plusbuttons[i]->setPos(albumxpos-(buttonsiz + 5),ypos);
        covers[i]->setPos(albumxpos,ypos);
        covers[i]->setScale(scale);
    }
}

void RecentAlbumsView::addAlbsToRecent(QList<Alb> albs)
{
    int alnum = 500;
    //Add the albums
    foreach(Alb al, albs)
    {	
        //Dont add if it's already in there
        if(recents.count() > 0 && recents.contains(al.artist+al.name))
            continue;
        //add description
        QGraphicsTextItem* textDesc = new QGraphicsTextItem();
        QString desc = "<b>" + al.name + " - " + al.artist;
        desc += (al.year != "0") ? " [" + al.year + "]" : "";
        desc += "</b><br/>";
        QString trackz = "";
        int i = 1;
        foreach(QString t, al.tracks)
        {
            trackz += QString::number(i) + ". " + t + "<br/>";
            ++i;
        }
        trackz.remove(trackz.length()-1,1); //Remove final newline
        desc += trackz;
        textDesc->setHtml(desc);
        textDesc->setFont(albFont);
        textDesc->setZValue(alnum+1);
        //First add a backing rectangle
        QGraphicsRectItem* back = new QGraphicsRectItem();
        back->setPen(Qt::NoPen);
        back->setZValue(alnum);
        back->setBrush(parent->palette().window());
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(7);
        shadow->setColor(parent->palette().window().color());
        shadow->setOffset(0,-7);
        back->setGraphicsEffect(shadow);
        scene->addItem(back);
        scene->addItem(textDesc);
        backgrounds.push_front(back);
        descriptions.push_front(textDesc);

        //add cover
        QGraphicsPixmapItem* cover = NULL;
        if(al.imguri != "")
        {
            shadow = new QGraphicsDropShadowEffect();
            shadow->setBlurRadius(7);
            shadow->setColor(Qt::black);
            shadow->setOffset(0,0);
            cover = new QGraphicsPixmapItem(QPixmap(al.imguri).scaled(200,200,
                                                                      Qt::KeepAspectRatio,
                                                                      Qt::SmoothTransformation));

            cover->setTransformationMode(Qt::SmoothTransformation);
            cover->setGraphicsEffect(shadow);
            cover->setZValue(alnum+1);
        }
        scene->addItem(cover);
        covers.push_front(cover);

        //add buttons
        QGraphicsSvgItem* plus = new QGraphicsSvgItem("../ssmp/Resources/plus_alt.svg");
        plus->setZValue(alnum+1);
        plus->setOpacity(0.8);
        scene->addItem(plus);
        plusbuttons.push_front(plus);

        recents.push_front(al.artist + al.name);
        //Remove bottom item if more than five in view
		//TODO: Parameterize # in view
        if(recents.count() > 5)
        {
            scene->removeItem(descriptions.back());
            scene->removeItem(covers.back());
            scene->removeItem(plusbuttons.back());
            scene->removeItem(backgrounds.back());
            recents.pop_back();
            descriptions.pop_back();
            covers.pop_back();
            plusbuttons.pop_back();
            backgrounds.pop_back();
        }
        alnum -= 2;
    }
}

RecentAlbumsView::~RecentAlbumsView()
{

}
