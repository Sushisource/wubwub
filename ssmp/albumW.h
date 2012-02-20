#pragma once
#include "e:\libs\qtvs\src\gui\kernel\qwidget.h"
#include "ui_album.h"
class albumW : public QWidget
{
	Q_OBJECT

public:
	albumW(void);
	albumW(QString artist, QString album, QString year, QList<QString> tracks, QString img);
	~albumW(void);

private:
	Ui::album ui;
	QGraphicsScene* cover;
};

