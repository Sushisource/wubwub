#pragma once
#include "e:\libs\qtvs\src\gui\kernel\qwidget.h"
#include "ui_album.h"
#include <qdebug.h>
class albumW : public QWidget
{
	Q_OBJECT

public:
	albumW(void);
	albumW(QString artist, QString album, QString year, QList<QString> tracks, QString img);
	~albumW(void);

protected:
     void resizeEvent(QResizeEvent *event);

private:
	Ui::album ui;
	QPixmap cover;	
};

