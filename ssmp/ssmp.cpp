#include "ssmp.h"

ssmp::ssmp(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags)
{
    //Register metatypes
    qRegisterMetaType<QList<QString>>("QList<QString>");
    ui.setupUi(this);
    ui.menuOptions->setWindowOpacity(.5);
    disabledColor = this->palette().color(QPalette::Disabled, QPalette::WindowText);
    searchtypes = QList<QString>() << "artist" << "album" << "song";
    //Update global palette access
    QApplication::setPalette(this->palette());

    settings = new QSettings("ssmp_config.ini",QSettings::IniFormat, this);
    //Instantiate options menu
    optWin = new optionsWindow(this);
    //Setup suggest popup
    initPopup();

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

    //Link up stuff
    //Dbi updates
    connect(dbi, SIGNAL(atDir(QString)), optWin, SLOT(changeStatus(QString)));
    connect(dbi, SIGNAL(recentChange()), recentAlbs, SLOT(update()));
    //Open options windows
    connect(ui.optionsAction, SIGNAL(triggered()), SLOT(openOptions()));
    //Save button on options window
    connect(optWin, SIGNAL(startSongParsing(QList<QString>)), dbi, SLOT(processDirs(QList<QString>)));
    //Search bar. Auto suggest after 500ms of stopped typing
    ui.search->installEventFilter(this);
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    searchTimer->setInterval(300);
    connect(searchTimer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(ui.search, SIGNAL(textEdited(QString)), searchTimer, SLOT(start()));
    //Connections for the recent view
    connect(recentAlbs, SIGNAL(addAlbsToNowPlaying(QList<int>)), ui.nowplayingLst, SLOT(addAlbums(QList<int>)));
    connect(recentAlbs, SIGNAL(openAlbumTab(int)), SLOT(newAlbumTab(int)));
    //Now play list
    connect(ui.nowplayingLst, SIGNAL(songChange(QString)), ui.playbackwidget, SLOT(changeSong(QString)));
    //Playback manager
    connect(ui.playbackwidget, SIGNAL(songOver()), ui.nowplayingLst, SLOT(nextSong()));

    //Update the recent view
    dbi->refresh();
    recentAlbs->update();
}

void ssmp::initPopup()
{
    popup = new QTreeWidget(this);
    popup->setWindowFlags(Qt::Popup);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->setMouseTracking(true);
    popup->setColumnCount(2);
    popup->setUniformRowHeights(true);
    popup->setRootIsDecorated(false);
    popup->setEditTriggers(QTreeWidget::NoEditTriggers);
    popup->setSelectionBehavior(QTreeWidget::SelectRows);
    popup->setFrameStyle(QFrame::Box | QFrame::Plain);
    popup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    popup->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    popup->header()->hide();
    popup->installEventFilter(this);
}

void ssmp::autoSuggest()
{
    QString q = ui.search->text();
    if(q.length() < 3) return;
    QMap<QString, int> res = dbi->search(q);
    if(res.count() < 1) return;

    popup->setUpdatesEnabled(false);
    popup->clear();

    foreach(QString t, searchtypes)
    {
        QList<int> resp = res.values(t);
        QList<QString> resp_str = dbi->getNames(resp, t);
        QList<QPair<QString, int>> pairs;
        for(int i = 0; i < resp.count(); ++i)
        {
            pairs.append(qMakePair(resp_str[i],resp[i]));
        }
        //Sort by string length and startsWith, shortest first
        qSort(pairs.begin(),pairs.end(), [&q](const QPair<QString, int> &s1, const QPair<QString, int> &s2)->bool
        {
            if(!s1.first.toLower().startsWith(q.toLower()))
                return false;
            return s1.first.length() < s2.first.length();
        });
        for(int i = 0; i < pairs.count(); ++i)
        {
            QTreeWidgetItem * item;
            item = new QTreeWidgetItem(popup);
            item->setText(0, pairs[i].first);
            item->setText(1, t);
            item->setData(0,Qt::WhatsThisRole, pairs[i].second);
            item->setTextAlignment(1, Qt::AlignRight);
            item->setTextColor(1, disabledColor);
        }
    }

    popup->setCurrentItem(popup->topLevelItem(0));
    popup->resizeColumnToContents(0);
    popup->resizeColumnToContents(1);
    popup->adjustSize();
    popup->setUpdatesEnabled(true);
    int h = popup->sizeHintForRow(0) * qMin(7, res.count()) + 3;
    popup->resize(ui.search->width(), h);
    popup->move(ui.search->mapToGlobal(QPoint(0, ui.search->height())));
    popup->setFocus();
    popup->show();
}

void ssmp::newAlbumTab(int alid)
{
    ui.tabWidget->setCurrentWidget(openAlbumTab(alid));
}

bool ssmp::eventFilter(QObject* object, QEvent* e)
{
    if (object == ui.search)
    {
        if(e->type() == QEvent::MouseButtonPress) //Select all of search when clicked
            QTimer::singleShot(0, object, SLOT(selectAll()));
        return false;
    }
    else if(object == popup)
    {
        if (e->type() == QEvent::MouseButtonPress)
        {
            popup->hide();
            ui.search->setFocus();
            return true;
        }
        if (e->type() == QEvent::KeyPress)
        {
            bool consumed = false;
            int key = static_cast<QKeyEvent *>(e)->key();
            QList<int> l;
            switch (key)
            {
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    consumed = true;
                    ui.search->clearFocus();
                    popup->hide();
                    break;

                case Qt::Key_Control:
                    l.append(popup->selectedItems()[0]->data(0, Qt::WhatsThisRole).toInt());
                    if(popup->selectedItems()[0]->text(1) == "song")
                        ui.nowplayingLst->addSongs(l);
                    consumed = true;
                    ui.search->clearFocus();
                    popup->hide();
                    break;

                case Qt::Key_Escape:
                    ui.search->clearFocus();
                    popup->hide();
                    consumed = true;

                case Qt::Key_Up:
                case Qt::Key_Down:
                case Qt::Key_Tab:
                case Qt::Key_Home:
                case Qt::Key_End:
                case Qt::Key_PageUp:
                case Qt::Key_PageDown:
                    break;

                default:
                    ui.search->setFocus();
                    ui.search->event(e);
                    popup->hide();
                    break;
            }
            return consumed;
        }
    }
    return false;
}

void ssmp::openSearchWindow(QString name, QMap<QString,QString> results)
{
    QWidget* searchtab = new QWidget(ui.tabWidget);
    ui.tabWidget->addTab(searchtab, name);
}

QWidget* ssmp::openAlbumTab(int alid)
{
    QWidget* container = new QWidget(ui.tabWidget);
    ui.tabWidget->addTab(container, dbi->getAlbumNameFromId(alid));
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* lay = new QVBoxLayout(container);
    lay->setMargin(0);
    AlbumTab* altab = new AlbumTab(alid, container);
    connect(altab, SIGNAL(clearPlaylist()), ui.nowplayingLst, SLOT(clear()));
    connect(altab, SIGNAL(playSong(int)), ui.playbackwidget, SLOT(changeSong(int)));
    lay->addWidget(altab,1);
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
