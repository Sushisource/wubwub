#include "albumtab.h"

AlbumTab::AlbumTab(int alid, QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setScene(scene);

    DBI* db = &DBI::getInstance();

    //Init fonts
    //TODO: Grab these from settings
    titlefont = QFont("Calibri",20, QFont::Bold);
    trackfont = QFont("Arial",15,QFont::Bold);

    //Album title
    title = new QGraphicsSimpleTextItem();
    QString tstr = db->getAlbumNameFromId(alid) + " - " + db->getArtistNameFromAlbumId(alid);
    title->setText(tstr);
    title->setFont(titlefont);
    title->setPos(0,0);
    title->setZValue(1);
    scene->addItem(title);

    //Backing rectangle
    bgrect = new QGraphicsRectItem();
    QColor c = QApplication::palette().window().color();
    bgrect->setPen(QPen(c,1));
    c.setAlpha(180);
    bgrect->setBrush(QBrush(c, Qt::Dense4Pattern));
    scene->addItem(bgrect);

    //Individual Tracks
    addTracks(alid, db);

    //Album art
    cover = new QGraphicsPixmapItem(QPixmap(db->getImgUriFromAlbumId(alid)));
    QGraphicsDropShadowEffect* shad = new QGraphicsDropShadowEffect();
    shad->setBlurRadius(6);
    shad->setColor(Qt::black);
    shad->setOffset(0,0);
    cover->setTransformationMode(Qt::SmoothTransformation);
    cover->setZValue(-1);
    cover->setGraphicsEffect(shad);
    scene->addItem(cover);
}

void AlbumTab::addTracks(int alid, DBI* db)
{
    QList<QString> tracknames = db->getTrackColFromAlbum(alid, 1);
    QList<QString> tracknums = db->getTrackColFromAlbum(alid, 2);
    int lasty = 0;
    int widest = 0;
    for(int i = 0; i < tracknames.count(); ++i)
    {
        QGraphicsSimpleTextItem* track = new QGraphicsSimpleTextItem();
        track->setText(tracknums[i] + ".  " + tracknames[i]);
        track->setFont(trackfont);
        lasty = i*17+30;
        track->setPos(5,lasty);
        track->setZValue(1);
        widest = max(widest, (int)track->boundingRect().width() + 5);
        scene->addItem(track);
    }
    widest = max(widest, (int)title->boundingRect().width());
    bgrect->setRect(-1,-1,widest + 3,lasty + 23);
}

void AlbumTab::resizeEvent(QResizeEvent *event)
{
    int pad = 5;
    int ww = this->geometry().width();
    int wh = this->geometry().height();
    scene->setSceneRect(0,0,ww,wh);
    title->setPos(0,0);
    cover->setPos(ww - pad - cover->boundingRect().width(),wh - pad -cover->boundingRect().height());
}

void AlbumTab::wheelEvent(QWheelEvent *event)
{
    //Get owned
}
