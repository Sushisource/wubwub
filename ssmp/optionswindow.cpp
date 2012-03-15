#include "optionswindow.h"
#include "ssmp.h" //TODO: Fuck you circular dependencies

optionsWindow::optionsWindow(ssmp * parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	myparent = parent;
	ui.setupUi(this);	
	connect(ui.addFolderBtn, SIGNAL(clicked()), this, SLOT(getDir()));	
	connect(ui.saveBtn, SIGNAL(clicked()),this, SLOT(save()));
	foreach(QVariant folder, parent->settings->value("libdir").toList())
	{
		ui.libFList->addItem(folder.toString());
	}
}

void optionsWindow::getDir()
{
	QString startdir = "$HOME";
	if(ui.libFList->count() > 0)
		startdir = ui.libFList->item(0)->text();
	libdir = QFileDialog::getExistingDirectory(this, "Select Libdir",startdir,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui.libFList->addItem(libdir);
}

void optionsWindow::changeStatus(const QString &s)
{
	ui.curDirLbl->setText(s);
}

void optionsWindow::save()
{
	QList<QString> dirs;
	foreach(QListWidgetItem* i, ui.libFList->findItems("*", Qt::MatchWildcard))
	{
		QString path = i->text();		
		dirs.append(path);
	}	
	myparent->dbi->addDirs2Lib(dirs);	
	emit startSongParsing();
	QVariantList dlist;
	foreach(QString d, dirs)
		dlist << d;
	myparent->settings->setValue("libdir", dlist);	
}

optionsWindow::~optionsWindow()
{
}