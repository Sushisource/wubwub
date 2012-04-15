#include "albumtab.h"

AlbumTab::AlbumTab(int alid, QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizeIncrement(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setScene(scene);

    DBI* db = &DBI::getInstance();

    //Init fonts
    //TODO: Grab these from settings
    titlefont = QFont("Calibri",20, QFont::Bold);
    trackfont = QFont("Arial",15,QFont::Bold);

    //Album title
    QGraphicsSimpleTextItem* title = new QGraphicsSimpleTextItem();
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(4);
    shadow->setColor(QColor(255,255,255));
    shadow->setOffset(1.5,1.5);
    QString tstr = db->getAlbumNameFromId(alid) + " - " + db->getArtistNameFromAlbumId(alid);
    title->setText(tstr);
    title->setFont(titlefont);
    title->setPos(0,0);
    title->setZValue(1);
    //title->setGraphicsEffect(shadow);
    scene->addItem(title);

    //Individual Tracks
    addTracks(alid, db);

    //Backing rectangle
    /*
    QGraphicsRectItem* bgrect = new QGraphicsRectItem();
    bgrect->setRect(0,0,500,100);
    bgrect->setPen(Qt::NoPen);
    bgrect->setBrush(QBrush(QColor(255,255,255,160), Qt::Dense4Pattern));
    scene->addItem(bgrect);*/

    //Album art
    cover = new QGraphicsPixmapItem(QPixmap(db->getImgUriFromAlbumId(alid)));
    cover->setTransformationMode(Qt::SmoothTransformation);
    cover->setZValue(-1);
    scene->addItem(cover);
}

void AlbumTab::addTracks(int alid, DBI* db)
{
    QList<QString> tracknames = db->getTrackColFromAlbum(alid, 1);
    QList<QString> tracknums = db->getTrackColFromAlbum(alid, 2);
    QGraphicsDropShadowEffect* shad = new QGraphicsDropShadowEffect();
    for(int i = 0; i < tracknames.count(); ++i)
    {
        QGraphicsSimpleTextItem* track = new QGraphicsSimpleTextItem();
        track->setText(tracknums[i] + ".  " + tracknames[i]);
        track->setFont(trackfont);
        track->setPos(5,i*16+30);
        track->setZValue(1);
        shad->setBlurRadius(2);
        shad->setColor(QColor(255,255,255,255));
        shad->setOffset(0,0);
        track->setGraphicsEffect(shad);
        scene->addItem(track);
    }
}

void AlbumTab::resizeEvent(QResizeEvent *event)
{
    qDebug() << this->geometry();
    cover->setPos(scene->width() - cover->boundingRect().width(),scene->height() - cover->boundingRect().height());
}
