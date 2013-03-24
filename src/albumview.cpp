#include "ssmp.h"

//Setup some constants
AlbumView::AlbumView(QWidget* parent, float minThumbSiz) : QGraphicsView(parent)
{
    albumcount = 0;
    znum = 0;
    minThumbSize = minThumbSiz;
    maxAlbs = 5; //Sensible default
    db = &DBI::getInstance();
    scene = new QGraphicsScene(this);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

void AlbumView::resizeEvent(QResizeEvent* e)
{
    if(albumcount < 1) //If there's nothing to resize, don't bother.
        return;
    int padding = 8;
    float siz  = (this->geometry().height() - padding*(1+albumcount))
                  / albumcount;
    if(minThumbSize >= 0)
        siz = qMax(siz, minThumbSize);
    int buttonsiz = plusbuttons[0]->boundingRect().width();
    float scale = siz / 200.0;
    int albumxpos = this->width()-padding-scale*200;
    for(int i = 0; i < albumcount; i++)
    {
        int ypos = i*(siz + padding) + padding;
        // This + 10 hides long text from an album one above showing up
        // behind the bottom fade of the album below
        backgrounds[i]->setRect(0, ypos, albumxpos, siz + 10);
        descriptions[i]->setPos(0, ypos);
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

void AlbumView::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF sc = this->mapToScene(event->pos());
    QGraphicsItem* item = scene->itemAt(sc, QTransform());
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

void AlbumView::addAlbs(QList<Alb> albs)
{
    //Add the albums
    foreach(Alb al, albs)
    {
        //Dont add if it's already in there
        if(alids.count() > 0 && alids.contains(al.alid))
            continue;
        //add description
        QGraphicsTextItem* textDesc = new QGraphicsTextItem();
        QString desc = "<b>" + al.name.toHtmlEscaped() + " - "
                       + al.artist.toHtmlEscaped();
        desc += (al.year != "0") ? " [" + al.year + "]" : "";
        desc += "</b><br/>";
        QString trackz = "";
        int i = 1;
        foreach(QString t, al.tracks)
        {
            trackz += QString::number(i) + ". " + t.toHtmlEscaped() + "<br/>";
            ++i;
        }
        trackz.remove(trackz.length()-1,1); //Remove final newline
        desc += trackz;
        textDesc->setHtml(desc);
        textDesc->setFont(QFont("Arial", 11));
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
        backgrounds.push_back(back);
        descriptions.push_back(textDesc);

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
        covers.push_back(cover);

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
        plusbuttons.push_back(plus);
        tabbuttons.push_back(tabb);

        alids.push_back(al.alid);
        //Remove bottom item if there are more albums
        //than we allow
        if(alids.count() > maxAlbs)
        {
            scene->removeItem(descriptions.front());
            scene->removeItem(covers.front());
            scene->removeItem(plusbuttons.front());
            scene->removeItem(tabbuttons.front());
            scene->removeItem(backgrounds.front());
            delete descriptions.front();
            delete covers.front();
            delete plusbuttons.front();
            delete tabbuttons.front();
            delete backgrounds.front();
            descriptions.pop_front();
            covers.pop_front();
            plusbuttons.pop_front();
            tabbuttons.pop_front();
            backgrounds.pop_front();
            alids.pop_front();
        }
        znum += 2;
    }
    albumcount = alids.count();
    resizeEvent();
}

AlbumView::~AlbumView()
{
    delete bottomfade;
}
