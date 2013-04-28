#include "wubwub.h"

wubwub::wubwub(QWidget *parent, Qt::WindowFlags flags) :
    QMainWindow(parent, flags)
{
    //Register metatypes
    qRegisterMetaType<QList<QString>>("QList<QString>");
    qRegisterMetaType<QList<QString>>("QList<Alb>");
    ui.setupUi(this);
    //wubwub is the global event filter
    QApplication::instance()->installEventFilter(this);
    //Update global palette access
    QApplication::setPalette(this->palette());
    //Initiate settings file
    settings = std::unique_ptr<QSettings>(
            new QSettings("wubwub_config.ini",QSettings::IniFormat, this));
    //Instantiate options menu
    optWin = std::unique_ptr<optionsWindow>(new optionsWindow(this));

    //Connect to database
    dbi = &DBI::getInstance();
    //Run db thread
    dbthread = std::unique_ptr<QThread>(new QThread(this));
    dbthread->start();
    dbi->moveToThread(dbthread.get());
    dbi->initDB();

    //Setup recent initial view
    recentAlbs = std::unique_ptr<RecentAlbumsTab>(new RecentAlbumsTab(this));

    //Hook up search to database
    ui.search->connectToDb(dbi);

    //Link up stuff
    //Dbi updates
    connect(dbi, &DBI::atDir,
            optWin.get(), &optionsWindow::changeStatus);
    connect(dbi, &DBI::recentChange, recentAlbs.get(),
            &RecentAlbumsTab::update);
    //Save button on options window
    connect(optWin.get(),
            &optionsWindow::startSongParsing, dbi, &DBI::processDirs);
    //Connections for the recent view
    //connect(recentAlbs.get(), &RecentAlbumsView::addAlbsToNowPlaying,
    //        ui.nowplayingLst, &Playlist::addAlbums);
    //connect(recentAlbs.get(), &RecentAlbumsView::openAlbumTab, this,
    //        &wubwub::openAlbumTab);
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
    //TODO: Figure out of this is still necessary in shiny new QML world
    //QTimer::singleShot(1100, recentAlbs.get(), SLOT(update()));

    //Finish UI
    //Setup viz
    ui.viz->setPlayBackPointer(ui.playbackwidget);
    ui.recentTab->layout()->addWidget(recentAlbs.release());
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

bool wubwub::eventFilter(QObject*, QEvent* e)
{
    if (e->type() == QEvent::KeyPress)
    {
        QKeyEvent* ke = static_cast<QKeyEvent *>(e);
        Qt::KeyboardModifiers mods = ke->modifiers();
        int key = ke->key();
        if(key == Qt::Key_MediaPlay || key == Qt::Key_Space) //Pause/play
        {
            ui.playbackwidget->togglePlay();
            return true;
        }
        //Next song
        else if (key == Qt::Key_Right && mods == Qt::ControlModifier)
        {
            ui.nowplayingLst->nextSong();
            return true;
        }
        //prev song
        else if (key == Qt::Key_Left && mods == Qt::ControlModifier)
        {
            ui.nowplayingLst->prevSong();
            return true;
        }
    }
    return false;
}

void wubwub::openSearchWindow(QString name, QMap<QString,QString> results)
{
    auto searchtab = std::unique_ptr<QWidget>(new QWidget(ui.tabWidget));
    ui.tabWidget->addCloseableTab(searchtab.release(), name);
}

void wubwub::openAlbumTab(int alid)
{
    auto container = std::unique_ptr<QWidget>(new QWidget(ui.tabWidget));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto lay = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(container.get()));
    lay->setMargin(0);
    auto altab = std::unique_ptr<AlbumTab>(new AlbumTab(alid, container.get()));
    connect(altab.get(), &AlbumTab::clearPlaylist,
            ui.nowplayingLst, &Playlist::clear);
    connect(altab.get(), &AlbumTab::playSongFromAlbum,
            ui.nowplayingLst, &Playlist::playSongFromAlbum);
    lay->addWidget(altab.release(),1);
    container->setLayout(lay.release());
    ui.tabWidget->addCloseableTab(container.release(),
                                  dbi->getAlbumNameFromId(alid));
}

void wubwub::openArtistTab(int arid)
{
    auto container = std::unique_ptr<QWidget>(new QWidget(ui.tabWidget));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto lay = std::unique_ptr<QVBoxLayout>(new QVBoxLayout(container.get()));
    lay->setMargin(0);
    auto arview = std::unique_ptr<ArtistAlbumsView>(
                new ArtistAlbumsView(arid, container.get(), 100));
    connect(arview.get(), &ArtistAlbumsView::addAlbsToNowPlaying,
            ui.nowplayingLst, &Playlist::addAlbums);
    connect(arview.get(), &ArtistAlbumsView::openAlbumTab,
            this, &wubwub::openAlbumTab);
    lay->addWidget(arview.release(),1);
    container->setLayout(lay.release());
    ui.tabWidget->addCloseableTab(container.release(),
                                  dbi->getArtistNameFromId(arid));
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
