//TODO: Parameterize album art default size
#include "albumview.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsEffect>
#include <qapplication.h>
#include <qdebug.h>

AlbumView::AlbumView(QString artist, QString album, QString year, QList<QString> tracks, QString img, QGraphicsItem *parent)
	: QGraphicsWidget(parent)
{
	scene = new QGraphicsScene(this);
	layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
    aart = new QGraphicsWidget(this);
    this->setLayout(layout);

    QString yearstr = (year == "0") ? "" : " ["+year+"]";
	int i = 1;
	QString trackz = "";
	foreach(QString t, tracks)
	{
		trackz += QString::number(i) + ". " + t + "\n";
		++i;
    }
    trackz.remove(trackz.length()-1,1); //Remove final newline
    QLabel* label = new QLabel();
    label->setMargin(0);
    label->setText(album + " - " + artist + yearstr + "\n" + trackz);
    label->setStyleSheet("QLabel{background: transparent;}");
    QGraphicsProxyWidget* info = scene->addWidget(label);
    info->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Expanding);
    layout->addItem(info);
    layout->setStretchFactor(info, 1);
	
	if(img != "")			
	{
		QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(10);
		shadow->setColor(Qt::black);
		shadow->setOffset(0,0);
		cover = new QGraphicsPixmapItem(QPixmap(img).scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation)
										,aart);
		cover->setTransformationMode(Qt::SmoothTransformation);
        cover->setGraphicsEffect(shadow);
	}
	else
        cover = NULL;

    layout->addItem(aart);
    layout->setStretchFactor(aart, 0);
}

void AlbumView::resizeEvent(QGraphicsSceneResizeEvent* e)
{
	if(cover == NULL) return;
    QSize scaledSize = aart->size().toSize();
    //Only thing that matters for scale is the height and to maintain aspect ratio
    scaledSize.scale(INT_MAX, this->geometry().height(), Qt::KeepAspectRatio);
    if (scaledSize != aart->size().toSize())
    {
        //TODO: Parameterize the 200, also see initial cover scaling
        aart->setScale(scaledSize.height() / 200.0);
    }
}

void AlbumView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

void AlbumView::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{

}


AlbumView::~AlbumView()
{
//	delete info, cover, layout;
}
