#include "ssmp.h"

//Setup some constants
const QFont RecentAlbumsView::albFont = QFont("Arial", 11);

RecentAlbumsView::RecentAlbumsView(QWidget* parent, int listsize) : QGraphicsView(parent)
{
	scene = new QGraphicsScene(this);
    db = &DBI::getInstance();
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setScene(scene);
    bottomfade = new QGraphicsRectItem;
    bottomfade->setPen(Qt::NoPen);
    bottomfade->setZValue(1000);
    bottomfade->setBrush(QApplication::palette().window());
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(7);
    shadow->setColor(QApplication::palette().window().color());
    shadow->setOffset(0,-7);
    bottomfade->setGraphicsEffect(shadow);
    scene->addItem(bottomfade);
}

void RecentAlbumsView::update(int howmany)
{
    addAlbsToRecent(db->getNRecentAlbums(howmany));
    resizeEvent();
}

void RecentAlbumsView::resizeEvent(QResizeEvent* e)
{
    if(rnum < 1) //If there's nothing to resize, don't bother.
        return;
    int padding = 8;
    float siz  = (this->geometry().height() - padding*(1+rnum)) / rnum;
    int buttonsiz = plusbuttons[0]->boundingRect().width();
    float scale = siz / 200.0;
    int albumxpos = this->width()-padding-scale*200;
    for(int i = 0; i < rnum; i++)
    {
        int ypos = i*(siz + padding) + padding;
        backgrounds[i]->setRect(0,ypos,albumxpos,siz);
        descriptions[i]->setPos(0,ypos);
        plusbuttons[i]->setPos(albumxpos-(buttonsiz + 5),ypos);
        tabbuttons[i]->setPos(albumxpos-(buttonsiz + 5),ypos+35);
        //Don't try to resize covers we couldn't load
        if(covers[i] != NULL)
        {
            covers[i]->setPos(albumxpos,ypos);
            covers[i]->setScale(scale);
        }
    }
    //Update the bottom fade
    bottomfade->setRect(0,this->geometry().height(),albumxpos,10);
}

void RecentAlbumsView::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF sc = this->mapToScene(event->pos());
    QGraphicsItem* item = scene->itemAt(sc);
    if(item == NULL) return;
    if(item->type() == QGraphicsSvgItem::Type)
    {
        int alid = item->data(ALID).toInt();
        QList<int> em = QList<int>();

        switch(item->data(BTNTYPE).toInt())
        {
            case PLUS:
                em.append(alid);
                emit addAlbsToNowPlaying(em);
                break;
            case TAB:
                emit openAlbumTab(alid);
                break;
        }
    }
}

void RecentAlbumsView::wheelEvent(QWheelEvent *event)
{
    //SCREW YOU WHEEL EVENTS!!! HAHAHAHA!!!@!!111
}

void RecentAlbumsView::addAlbsToRecent(QList<Alb> albs)
{
    int znum = 0;
    //Add the albums
    foreach(Alb al, albs)
    {
        //Dont add if it's already in there
        if(recents.count() > 0 && recents.contains(al.alid))
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
        textDesc->setZValue(znum+1);
        //First add a backing rectangle
        QGraphicsRectItem* back = new QGraphicsRectItem();
        back->setPen(Qt::NoPen);
        back->setZValue(znum);
        back->setBrush(QApplication::palette().window());
        QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(7);
        shadow->setColor(QApplication::palette().window().color());
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
            cover->setZValue(znum+1);
        }
        scene->addItem(cover);
        covers.push_front(cover);

        //add buttons
        QGraphicsSvgItem* plus = new QGraphicsSvgItem(":/imgs/plus");
        QGraphicsSvgItem* tabb = new QGraphicsSvgItem(":/imgs/eye");
        plus->setZValue(znum+1);
        plus->setOpacity(0.8);
        tabb->setZValue(znum+1);
        tabb->setOpacity(0.8);
        plus->setData(ALID,al.alid);
        plus->setData(BTNTYPE,PLUS);
        tabb->setData(ALID,al.alid);
        tabb->setData(BTNTYPE,TAB);
        scene->addItem(plus);
        scene->addItem(tabb);
        plusbuttons.push_front(plus);
        tabbuttons.push_front(tabb);

        recents.push_front(al.alid);
        //Remove bottom item if more than five in view
        //TODO: Parameterize # in view
        if(recents.count() > 5)
        {
            scene->removeItem(descriptions.back());
            scene->removeItem(covers.back());
            scene->removeItem(plusbuttons.back());
            scene->removeItem(tabbuttons.back());
            scene->removeItem(backgrounds.back());
            delete descriptions.back();
            delete covers.back();
            delete plusbuttons.back();
            delete tabbuttons.back();
            delete backgrounds.back();
            descriptions.pop_back();
            covers.pop_back();
            plusbuttons.pop_back();
            tabbuttons.pop_back();
            backgrounds.pop_back();
            recents.pop_back();
        }
        znum -= 2;
    }
    rnum = recents.count();
}

RecentAlbumsView::~RecentAlbumsView()
{
}
