#include "wubwub.h"

wubwub::wubwub(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags)
{
    //Register metatypes
    qRegisterMetaType<QList<QString>>("QList<QString>");
    qRegisterMetaType<QList<QString>>("QList<Alb>");
    ui.setupUi(this);
    //Update global palette access
    QApplication::setPalette(this->palette());
    //Initiate settings file
    settings = new QSettings("wubwub_config.ini",QSettings::IniFormat, this);
    //Instantiate options menu
    optWin = new optionsWindow(this);

    //Connect to database
    dbi = &DBI::getInstance();
    //Run db thread
    dbthread = new QThread(this);
    dbthread->start();
    dbi->moveToThread(dbthread);

    //If db not initialized, initialize it
    if(settings->value("dbinitted").toBool() == false)
    {
        dbi->initDB();
        settings->setValue("dbinitted",true);
    }

    //Setup recent initial view
    recentAlbs = new RecentAlbumsView(this);
    ui.recentTab->layout()->addWidget(recentAlbs);

    //Hook up search to database
    ui.search->connectToDb(dbi);

    //Link up stuff
    //Dbi updates
    connect(dbi, &DBI::atDir, optWin, &optionsWindow::changeStatus);
    connect(dbi, &DBI::recentChange, recentAlbs, &RecentAlbumsView::newAlbs);
    //Save button on options window
    connect(optWin, &optionsWindow::startSongParsing, dbi, &DBI::processDirs);
    //Connections for the recent view
    connect(recentAlbs, &RecentAlbumsView::addAlbsToNowPlaying,
            ui.nowplayingLst, &Playlist::addAlbums);
    connect(recentAlbs, &RecentAlbumsView::openAlbumTab, this,
            &wubwub::openAlbumTab);
    //Now play list
    connect(ui.nowplayingLst, &Playlist::songChange, this, &wubwub::changeSong);
    //Playback manager
    connect(ui.playbackwidget, &PlaybackWidget::songOver, ui.nowplayingLst,
            &Playlist::nextSong);
    //Search bar
    connect(ui.search, &WWSearch::addSongToNowPlaying, this,
            &wubwub::addSongToNowPlaying);
    connect(ui.search, &WWSearch::openAlbumTab, this, &wubwub::openAlbumTab);
    connect(ui.search, &WWSearch::openArtistTab, this, &wubwub::openArtistTab);

    //Update the recent view but use a timer so the window pops up first
    QTimer::singleShot(1000, dbi, SLOT(refresh()));
    //We have to immediately update the view itself otherwise it's blank
    recentAlbs->update();
    //But then update it again after the dbi has refreshed.
    QTimer::singleShot(1100, recentAlbs, SLOT(update()));

    //Setup viz
    ui.viz->setPlayBackPointer(ui.playbackwidget);
}

void wubwub::addSongToNowPlaying(int sid)
{
    ui.nowplayingLst->addSong(sid);
}

//Song message multiplexer, notifies all
//objects that need to know about song changes
void wubwub::changeSong(int songid)
{
    QString songpath = dbi->getTrackColFromSong(songid, SongCol::path);
    QString sname = dbi->getSongNameFromId(songid);
    QString arname = dbi->getArtistNameFromSongId(songid);
    ui.tabWidget->setSongTabText(sname, arname);
    ui.playbackwidget->changeSong(songpath);
    this->setWindowTitle(arname
                         + QString::fromUtf8(" \xf0\x9f\x8e\xb5 ") //Music note
                         + sname);
    emit songChange(songid);
}

bool wubwub::eventFilter(QObject* object, QEvent* e)
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

void wubwub::openSearchWindow(QString name, QMap<QString,QString> results)
{
    QWidget* searchtab = new QWidget(ui.tabWidget);
    ui.tabWidget->addCloseableTab(searchtab, name);
}

QWidget* wubwub::openAlbumTab(int alid)
{
    QWidget* container = new QWidget(ui.tabWidget);
    ui.tabWidget->addCloseableTab(container, dbi->getAlbumNameFromId(alid));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setMargin(0);
    AlbumTab* altab = new AlbumTab(alid, container);
    connect(altab, SIGNAL(clearPlaylist()), ui.nowplayingLst, SLOT(clear()));
    connect(altab, SIGNAL(playSongFromAlbum(int, int)),
            ui.nowplayingLst, SLOT(playSongFromAlbum(int,int)));
    lay->addWidget(altab,1);
    ui.tabWidget->setCurrentWidget(container);
    return container;
}

QWidget *wubwub::openArtistTab(int arid)
{
    QWidget* container = new QWidget(ui.tabWidget);
    ui.tabWidget->addCloseableTab(container, dbi->getArtistNameFromId(arid));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setMargin(0);
    ArtistAlbumsView* arview = new ArtistAlbumsView(arid, container, 100);
    connect(arview, &ArtistAlbumsView::addAlbsToNowPlaying,
            ui.nowplayingLst, &Playlist::addAlbums);
    connect(arview, &ArtistAlbumsView::openAlbumTab, this, &wubwub::openAlbumTab);
    lay->addWidget(arview,1);
    ui.tabWidget->setCurrentWidget(container);
    ui.tabWidget->setFocus();
    return container;
}

bool wubwub::openOptions()
{
    optWin->show();
    return true;
}

wubwub::~wubwub()
{
    ui.viz->stopRenderThread();
    dbthread->terminate();
}
