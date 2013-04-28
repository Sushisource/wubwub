#ifndef optionswindow_H
#define optionswindow_H

#include "ui_options.h"
#include <QDir>
#include <QDirIterator>
#include <qfiledialog.h>
class wubwub; //TODO: Fucking circular depends

class optionsWindow : public QMainWindow
{
	Q_OBJECT

public:
    optionsWindow(wubwub * parent = 0, Qt::WindowFlags flags = 0);
	~optionsWindow();

public slots:
	void changeStatus(const QString &s);

signals:
    void startSongParsing(QList<QString>, bool);

private:
	Ui::optionsWindow ui;
	QString libdir;
	wubwub * myparent;

	bool isAudioFile(QFileInfo f);
	void addDir2Lib(QDir dir);
	
private slots:
	void getDir();
	void save();
};

#endif // optionswindow_H
