#include "albumW.h"
//TODO: Custom resizer for album art
albumW::albumW(QString artist, QString album, QString year, QList<QString> tracks, QString img = "")
{
	//TODO: Fix for too-long track/album names etc
    ui.setupUi(this);
	ui.albumView->setBackgroundRole(QPalette::Base);
	
	QString yearstr = (year == "0") ? "" : " ["+year+"]";
    ui.nameLbl->setText(album + " - " + artist + yearstr);
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
		cover = img;
		ui.albumView->setPixmap(img);	
	}
	else
		cover = NULL;
}

void albumW::resizeEvent(QResizeEvent* e)
{
	if(cover.isNull()) return;
	QSize scaledSize = cover.size();
	scaledSize.scale(ui.albumView->size(), Qt::KeepAspectRatio);
    if (!ui.albumView->pixmap() || scaledSize != ui.albumView->pixmap()->size())
	{
		ui.albumView->setPixmap(cover.scaled(ui.albumView->size(),
											Qt::KeepAspectRatio,
											Qt::SmoothTransformation));
	}
}

albumW::~albumW(void)
{
}
