#include "albumtab.h"

AlbumTab::AlbumTab(int alid, QWidget *parent) : QGraphicsView(parent)
{
    album_id = alid;
    scene = std::unique_ptr<QGraphicsScene>(new QGraphicsScene(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setScene(scene.get());
    this->setMouseTracking(true);

    DBI* db = &DBI::getInstance();

    //Init fonts
    //TODO: Grab these from settings
    titlefont = QFont("Calibri",20, QFont::Bold);
    trackfont = QFont("Arial",15,QFont::Bold);

    //Album title
    title = std::unique_ptr<QGraphicsSimpleTextItem>(
                new QGraphicsSimpleTextItem());
    QString tstr = db->getAlbumNameFromId(alid) + " - "
                 + db->getArtistNameFromAlbumId(alid);
    title->setText(tstr);
    title->setFont(titlefont);
    title->setPos(0,0);
    title->setZValue(1);
    scene->addItem(title.get());

    //Individual Tracks
    addTracks(alid, db);

    //Album art
    QPixmap aart = QPixmap(db->getImgUriFromAlbumId(alid));
    cover = std::unique_ptr<QGraphicsPixmapItem>(
                new QGraphicsPixmapItem(
                    aart.scaledToWidth(300, Qt::SmoothTransformation)));
    auto shad = std::unique_ptr<QGraphicsDropShadowEffect>(
                new QGraphicsDropShadowEffect(this));
    shad->setBlurRadius(6);
    shad->setColor(Qt::black);
    shad->setOffset(0,0);
    cover->setTransformationMode(Qt::SmoothTransformation);
    cover->setZValue(-1);
    cover->setGraphicsEffect(shad.release());
    scene->addItem(cover.get());
}

void AlbumTab::addTracks(int alid, DBI* db)
{
    QList<QString> tracknames = db->getTrackColFromAlbum(alid, 1);
    QList<QString> tracknums = db->getTrackColFromAlbum(alid, 2);
    QList<int> trackids = db->getTrackIdsFromAlbum(alid);
    int lasty = 0;
    int widest = 0;
    for(int i = 0; i < tracknames.count(); ++i)
    {
        auto track = new PrettyText(title.get());
        track->setAcceptHoverEvents(true);
        track->setText(tracknums[i] + ".  " + tracknames[i]);
        track->setFont(trackfont);
        lasty = i*17+30;
        track->setPos(5,lasty);
        track->setZValue(1);
        track->setData(TRACKID,trackids[i]);
        widest = std::max(widest, (int)track->boundingRect().width() + 5);
    }
    widest = std::max(widest, (int)title->boundingRect().width());
}

void AlbumTab::resizeEvent(QResizeEvent *event)
{
    int pad = 5;
    int ww = this->geometry().width();
    int wh = this->geometry().height();
    cover->setPos(ww-300-pad,wh-300-pad);
    scene->setSceneRect(0,0,ww,wh);
}

void AlbumTab::wheelEvent(QWheelEvent *event)
{
    //Get owned
}

void AlbumTab::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF sc = this->mapToScene(event->pos());
    QGraphicsItem* item = scene->itemAt(sc, QTransform());
    if(item == NULL) return;
    if(item->type() == PrettyText::Type)
    {
        int songid = item->data(TRACKID).toInt();
        emit playSongFromAlbum(album_id, songid);
    }
}

AlbumTab::~AlbumTab()
{
}
