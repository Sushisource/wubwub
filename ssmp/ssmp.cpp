#include "ssmp.h"

ssmp::ssmp(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    //Register metatypes
    qRegisterMetaType<QList<QString>>("QList<QString>");
    ui.setupUi(this);
    //Update global palette access
    QApplication::setPalette(this->palette());
    //Initiate settings file
    settings = new QSettings("ssmp_config.ini",QSettings::IniFormat, this);
    //Instantiate options menu
    optWin = new optionsWindow(this);

    //Connect to database
    dbi = &DBI::getInstance();

    //If db not initialized, initialize it
    if(settings->value("dbinitted").toBool() == false)
    {
        dbi->initDB();
        settings->setValue("dbinitted",true);
    }

    //Setup recent initial view
    recentAlbs = new RecentAlbumsView(this);
    ui.recentTab->layout()->addWidget(recentAlbs);

    //Run db thread
    dbthread = new QThread(this);
    dbthread->start();
    dbi->moveToThread(dbthread);

    //Hook up search to database
    ui.search->connectToDb(dbi);

    //Link up stuff
    //Dbi updates
    connect(dbi, SIGNAL(atDir(QString)), optWin, SLOT(changeStatus(QString)));
    connect(dbi, SIGNAL(recentChange()), recentAlbs, SLOT(update()));
    //Open options windows
    connect(ui.optionsAction, SIGNAL(triggered()), SLOT(openOptions()));
    //Save button on options window
    connect(optWin, SIGNAL(startSongParsing(QList<QString>)), dbi, SLOT(processDirs(QList<QString>)));
    //Connections for the recent view
    connect(recentAlbs, SIGNAL(addAlbsToNowPlaying(QList<int>)), ui.nowplayingLst, SLOT(addAlbums(QList<int>)));
    connect(recentAlbs, SIGNAL(openAlbumTab(int)), SLOT(newAlbumTab(int)));
    //Now play list
    connect(ui.nowplayingLst, SIGNAL(songChange(int)), SLOT(changeSong(int)));
    //Playback manager
    connect(ui.playbackwidget, SIGNAL(songOver()), ui.nowplayingLst, SLOT(nextSong()));
    //Search bar
    connect(ui.search, SIGNAL(addSongToNowPlaying(int)), SLOT(addSongToNowPlaying(int)));
    connect(ui.search, SIGNAL(openAlbumTab(int)), SLOT(openAlbumTab(int)));

    //Update the recent view
    dbi->refresh();
    recentAlbs->update();
}


void ssmp::newAlbumTab(int alid)
{
    openAlbumTab(alid);
}

void ssmp::addSongToNowPlaying(int sid)
{
    ui.nowplayingLst->addSong(sid);
}

//Song message multiplexer, notifies all
//objects that need to know about song changes
void ssmp::changeSong(int songid)
{
    QString songpath = dbi->getTrackColFromSong(songid, SongCol::path);
    QString sname = dbi->getSongNameFromId(songid);
    QString arname = dbi->getArtistNameFromSongId(songid);
    ui.tabWidget->setTabText(0,arname + " - " + sname);
    ui.playbackwidget->changeSong(songpath);
    emit songChange(songid);
}

bool ssmp::eventFilter(QObject* object, QEvent* e)
{
    if (e->type() == QEvent::KeyPress)
    {
        qDebug() << "Hey!";
        int key = static_cast<QKeyEvent *>(e)->key();
        if(key == Qt::Key_MediaPlay)
            ui.playbackwidget->togglePlay();
    }
    return false;
}

void ssmp::openSearchWindow(QString name, QMap<QString,QString> results)
{
    QWidget* searchtab = new QWidget(ui.tabWidget);
    ui.tabWidget->addCTab(searchtab, name);
}

QWidget* ssmp::openAlbumTab(int alid)
{
    QWidget* container = new QWidget(ui.tabWidget);
    ui.tabWidget->addCTab(container, dbi->getAlbumNameFromId(alid));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setMargin(0);
    AlbumTab* altab = new AlbumTab(alid, container);
    connect(altab, SIGNAL(clearPlaylist()), ui.nowplayingLst, SLOT(clear()));
    connect(altab, SIGNAL(playSongFromAlbum(int, int)), ui.nowplayingLst, SLOT(playSongFromAlbum(int,int)));
    lay->addWidget(altab,1);
    ui.tabWidget->setCurrentWidget(container);
    return container;
}

bool ssmp::openOptions()
{
    optWin->show();
    return true;
}

ssmp::~ssmp()
{
    dbthread->terminate();
}
