#ifndef ALBUMVIEW_H
#define ALBUMVIEW_H

#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsTextItem>
#include <qgraphicsscene.h>
#include <qlabel.h>

class AlbumView : public QGraphicsWidget
{
public:
	AlbumView(QString artist, QString album, QString year, QList<QString> tracks, QString img = "",
		QGraphicsItem *parent = 0);
	~AlbumView();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
	void resizeEvent(QGraphicsSceneResizeEvent* e);

private:	
	QGraphicsScene* scene;
	QGraphicsLinearLayout* layout;
	QGraphicsWidget* aart;
	QGraphicsPixmapItem* cover;
};

#endif // ALBUMVIEW_H
