#include "libdialog.h"
#include <qfiledialog.h>

libraryDialog::libraryDialog(ssmp *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	myparent = parent;
	ui.setupUi(this);
	connect(ui.browseBtn,SIGNAL(clicked()),this,SLOT(getDir()));
	if(parent->settings->value("libdir") != QVariant())
		ui.libDirTxt->setText(parent->settings->value("libdir").toString());
}

void libraryDialog::getDir()
{
	libdir = QFileDialog::getExistingDirectory(this, "Select Libdir",ui.libDirTxt->text(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui.libDirTxt->setText(libdir);
}

void libraryDialog::accept()
{
	myparent->settings->setValue("libdir", ui.libDirTxt->text());	
	ui.buttonBox->setDisabled(true);
	addDir2Lib(ui.libDirTxt->text(), true);
	this->close();
}

void libraryDialog::addDir2Lib(QDir dir, bool top)
{
	ui.curDirLbl->setText(dir.path());
	dir.setFilter(QDir::Files | QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	QFileInfoList list = dir.entryInfoList();
	float siz = list.size();
	for(float i = 0; i < siz; i++)
	{
		QFileInfo f = list.at(i);
		if(f.isDir()) //If dir recurse
			addDir2Lib(QDir(f.filePath()), false);
		else if(isAudioFile(f))//Add this song to the database
		{
			TagLib::FileName fname = f.filePath().toStdString().c_str();
			QString artist, albumArtist;
			int length;
			TagLib::File* file = NULL;
			//MP3 Means we can check for additional info in ID3v2 tags
			if(f.suffix() == "mp3")
			{
				TagLib::MPEG::File* fr = new TagLib::MPEG::File(fname);				
				if(fr->ID3v2Tag())
				{
					TagLib::ID3v2::FrameList m = fr->ID3v2Tag()->frameList();						
					for_each(m.begin(),m.end(),[](TagLib::ID3v2::Frame* f)
					{ 
							qDebug() << f->frameID().data();							
							qDebug() << f->toString().toCString();							
					});
					//Somehow this means album artist / band. http://www.id3.org/id3v2.4.0-frames
					TagLib::ID3v2::FrameList l = fr->ID3v2Tag()->frameList("TPE2");
					if(!l.isEmpty())
						albumArtist = l.front()->toString().toCString();
				}
				file = fr;
			}			
			else
				file = TagLib::FileRef(fname).file();
			if(file != NULL)
			{
				TagLib::Tag* genTag = file->tag();
				artist = genTag->artist().toCString();
				length = file->audioProperties()->length();
				delete file;
			}
			//If you get here we couldn't read tags at all
		}
		if(top) //If we're the top level of recursion update prog bar
			ui.progressBar->setValue(i/siz * 100);
		qApp->processEvents();
	}
}

bool libraryDialog::isAudioFile(QFileInfo f)
{
	QString type = f.suffix();
	return std::accumulate(myparent->supportedFileFormats.begin(), myparent->supportedFileFormats.end(), false,
		[type](bool a, std::string b) {return (a || b == type.toStdString());});
}

libraryDialog::~libraryDialog()
{
	delete myparent;
}
