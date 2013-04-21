#ifndef ALBUMVIEW_H
#define ALBUMVIEW_H

#include <QGraphicsView>
#include <QGraphicsEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSvgItem>
#include "dbi/dbi.h"
class wubwub; //TODO: Fucking circular depends
struct Alb;

class AlbumView : public QGraphicsView
{
	Q_OBJECT

public:
    AlbumView(QWidget *parent, float minThumbSiz=-1);
    ~AlbumView();

signals:
    void addAlbsToNowPlaying(QList<int> alids);
    void openAlbumTab(int alid);

protected:
    DBI* db;
    void addAlbs(QList<Alb> albs);
    virtual void resizeEvent(QResizeEvent* e = NULL);
    int maxAlbs;

private:
    std::unique_ptr<QGraphicsScene> scene;
    std::unique_ptr<QGraphicsRectItem> bottomfade;
    QList<QString> alids;
    QList<QGraphicsTextItem*> descriptions;
    QList<QGraphicsPixmapItem*> covers;
    QList<QGraphicsRectItem*> backgrounds;
    QList<QGraphicsSvgItem*> plusbuttons;
    QList<QGraphicsSvgItem*> tabbuttons;
    int albumcount;
    int znum; // Most recent Z depth of albums being displayed
    float minThumbSize; // Minimum size of album art thumbnails
    enum DataKeys {ALID, BTNTYPE};
    enum BtnTypes {PLUS, TAB};
    static const QFont albFont;
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // ALBUMVIEW_H
