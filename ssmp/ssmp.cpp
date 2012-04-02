#include "ssmp.h"

ssmp::ssmp(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags)
{
    ui.setupUi(this);
    disabledColor = this->palette().color(QPalette::Disabled, QPalette::WindowText);
    searchtypes = QList<QString>() << "artist" << "album" << "song";
    //Update global palette access
    QApplication::setPalette(this->palette());

    settings = new QSettings("ssmp_config.ini",QSettings::IniFormat);
    //Instantiate options menu
    optWin = new optionsWindow(this);
    //Setup suggest popup
    popup = new QTreeWidget;
    popup->setWindowFlags(Qt::Popup);
    popup->setFocusPolicy(Qt::NoFocus);
    popup->setFocusProxy(parent);
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

    //Link up stuff	
    //Dbi updates
    connect(dbi, SIGNAL(atDir(QString)), optWin, SLOT(changeStatus(QString)));
    connect(dbi, SIGNAL(recentChange()), recentAlbs, SLOT(update()));
    //Open options windows
    connect(ui.optionsAction, SIGNAL(triggered()), SLOT(openOptions()));	
    //Save button on options window
    connect(optWin, SIGNAL(startSongParsing()), dbi, SLOT(processDirs()));
    //Search bar. Auto suggest after 500ms of stopped typing
    ui.search->installEventFilter(this);
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    searchTimer->setInterval(300);
    connect(searchTimer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(ui.search, SIGNAL(textEdited(QString)), searchTimer, SLOT(start()));
    //Connections for the recent view
    connect(recentAlbs, SIGNAL(addAlbsToNowPlaying(QList<int>)), ui.nowplayingLst, SLOT(addAlbums(QList<int>)));

    //Run db thread
    dbthread = new QThread;
    dbthread->start();
    dbi->moveToThread(dbthread);    

	//Update the recent view
    recentAlbs->update();
}

void ssmp::autoSuggest()
{
    QString q = ui.search->text();
    if(q.length() < 3) return;
    QMap<QString, QString> res = dbi->search(q);
    if(res.count() < 1) return;

    popup->setUpdatesEnabled(false);
    popup->clear();

    foreach(QString t, searchtypes)
    {    
        QList<QString> resp = res.values(t);
        //Sort by string length and startsWith, shortest first
        qSort(resp.begin(),resp.end(), [&q](const QString &s1, const QString &s2)->bool
		{
			if(!s1.toLower().startsWith(q.toLower()))
                return false;	
			return s1.length() < s2.length();
		});
        foreach(QString n, resp)
        {
            QTreeWidgetItem * item;
            item = new QTreeWidgetItem(popup);
            item->setText(0, n);
            item->setText(1, t);
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
            switch (key) 
            {
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    //doneCompletion();
                    consumed = true;

                case Qt::Key_Escape:
                    ui.search->setFocus();
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
    QWidget* searchtab = new QWidget();

    //Add to results pointer list and tabwiget
    tabs[ui.tabWidget->addTab(searchtab, name)] = searchtab;	
}

bool ssmp::openOptions()
{		
    optWin->show();
    return true;	
}

ssmp::~ssmp()
{	
    delete dbi;
    dbthread->terminate();
    delete dbthread;
    delete popup;
}
