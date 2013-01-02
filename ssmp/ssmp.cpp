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
    connect(dbi, &DBI::atDir, optWin, &optionsWindow::changeStatus);
    connect(dbi, &DBI::recentChange, recentAlbs, &RecentAlbumsView::update);
    //Save button on options window
    connect(optWin, &optionsWindow::startSongParsing, dbi, &DBI::processDirs);
    //Connections for the recent view
    connect(recentAlbs, &RecentAlbumsView::addAlbsToNowPlaying, ui.nowplayingLst, &Playlist::addAlbums);
    connect(recentAlbs, &RecentAlbumsView::openAlbumTab, this, &ssmp::openAlbumTab);
    //Now play list
    connect(ui.nowplayingLst, &Playlist::songChange, this, &ssmp::changeSong);
    //Playback manager
    connect(ui.playbackwidget, &PlaybackWidget::songOver, ui.nowplayingLst, &Playlist::nextSong);
    //Search bar
    connect(ui.search, &SsmpSearch::addSongToNowPlaying, this, &ssmp::addSongToNowPlaying);
    connect(ui.search, &SsmpSearch::openAlbumTab, this, &ssmp::openAlbumTab);
    connect(ui.search, &SsmpSearch::openArtistTab, this, &ssmp::openArtistTab);

    //Update the recent view
    dbi->refresh();
    recentAlbs->update();
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
    ui.tabWidget->addCloseableTab(searchtab, name);
}

QWidget* ssmp::openAlbumTab(int alid)
{
    QWidget* container = new QWidget(ui.tabWidget);
    ui.tabWidget->addCloseableTab(container, dbi->getAlbumNameFromId(alid));
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

QWidget *ssmp::openArtistTab(int arid)
{
    QWidget* container = new QWidget(ui.tabWidget);
    ui.tabWidget->addCloseableTab(container, dbi->getArtistNameFromId(arid));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setMargin(0);
    ArtistAlbumsView* arview = new ArtistAlbumsView(arid, container, 100);
    connect(arview, &ArtistAlbumsView::addAlbsToNowPlaying, ui.nowplayingLst, &Playlist::addAlbums);
    connect(arview, &ArtistAlbumsView::openAlbumTab, this, &ssmp::openAlbumTab);
    lay->addWidget(arview,1);
    ui.tabWidget->setCurrentWidget(container);
    ui.tabWidget->setFocus();
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
