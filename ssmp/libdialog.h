#ifndef libdialog_H
#define libdialog_H

#include <QtGui/QMainWindow>
#include "ui_libdialog.h"
#include "ssmp.h"
#include <QDir>
#include <QDirIterator>
#include <taglib\tag.h>
#include <taglib\fileref.h>
#include <id3v2tag.h>
#include <taglib\mpeg\mpegfile.h>

class libraryDialog : public QDialog
{
	Q_OBJECT

public:
	libraryDialog(ssmp *parent = 0, Qt::WFlags flags = 0);
	~libraryDialog();

private:
	Ui::libraryDialog ui;
	QString libdir;
	ssmp * myparent;

	bool isAudioFile(QFileInfo f);
	void addDir2Lib(QDir dir);
	

private slots:
	void getDir();
	void accept();
};

#endif // libdialog_H
