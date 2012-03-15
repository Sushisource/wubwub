#include "albumW.h"

//TODO: Custom resizer for album art
albumW::albumW(void)
{
}

albumW::albumW(QString artist, QString album, QString year, QList<QString> tracks, QString img = "")
{
	//TODO: Fix for too-long track/album names etc
    ui.setupUi(this);	
    ui.nameLbl->setText(artist + " - " + album + " [" + year + "]");
	int i = 1;
	QString trackz = "";
	foreach(QString t, tracks)
	{
		trackz += QString::number(i) + ". " + t + "\n";
		++i;
	}
	ui.tracklbl->setText(trackz);

	if(img != "")
	{
		cover = new QGraphicsScene;
		cover->addPixmap(QPixmap(img).scaledToWidth(ui.albumView->width(),Qt::SmoothTransformation));
		ui.albumView->setScene(cover);		
	}
}

albumW::~albumW(void)
{
}
