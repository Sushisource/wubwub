#include "wwsearch.h"

WWSearch::WWSearch(QWidget *parent) : QLineEdit(parent)
{
    //Setup suggest popup
    initPopup();
    searchtypes = QList<QString>() << "artist" << "album" << "song";
    disabledColor = this->palette().color(QPalette::Disabled, QPalette::WindowText);
    //Connections
    this->installEventFilter(this);
    searchTimer = new QTimer(this);
    searchTimer->setSingleShot(true);
    searchTimer->setInterval(300);
    connect(this, SIGNAL(textEdited(QString)), searchTimer, SLOT(start()));
    connect(searchTimer, SIGNAL(timeout()), SLOT(autoSuggest()));
}

void WWSearch::connectToDb(DBI *dbi)
{
    db = dbi;
}

void WWSearch::initPopup()
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

void WWSearch::autoSuggest()
{
    if(db == NULL)
        return;
    QString q = this->text();
    if(q.length() < 3) return;
    QMap<QString, int> res = db->search(q);
    if(res.count() < 1) return;

    popup->setUpdatesEnabled(false);
    popup->clear();

    foreach(QString t, searchtypes)
    {
        QList<int> resp = res.values(t);
        QList<QString> resp_str = db->getNames(resp, t);
        QList<QPair<QString, int> > pairs;
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
    popup->resize(this->width(), h);
    popup->move(this->mapToGlobal(QPoint(0, this->height())));
    popup->setFocus();
    popup->show();
}

bool WWSearch::eventFilter(QObject* object, QEvent* e)
{
    if (object == this)
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
            this->setFocus();
            return true;
        }
        if (e->type() == QEvent::KeyPress)
        {
            bool consumed = false;
            int key = static_cast<QKeyEvent *>(e)->key();
            QString type = popup->selectedItems()[0]->text(1);
            int id = popup->selectedItems()[0]->data(0, Qt::WhatsThisRole).toInt();
            switch (key)
            {
                case Qt::Key_Enter:
                case Qt::Key_Return:
                    consumed = true;
                    this->clearFocus();
                    popup->hide();
                    break;

                case Qt::Key_Control:
                    if(type == "song")
                        emit addSongToNowPlaying(id);
                    else if(type == "album")
                        emit openAlbumTab(id);
                    else if(type == "artist")
                        emit openArtistTab(id);
                    consumed = true;
                    this->clearFocus();
                    popup->clearFocus();
                    popup->hide();
                    break;

                case Qt::Key_Escape:
                    this->clearFocus();
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
                    this->setFocus();
                    this->event(e);
                    popup->hide();
                    break;
            }
            return consumed;
        }
    }
    return false;
}
