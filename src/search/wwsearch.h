#ifndef WWSEARCH_H
#define WWSEARCH_H

#include <QLineEdit>
#include <QTreeWidget>
#include <QHeaderView>
#include <QKeyEvent>
#include "dbi/dbi.h"

class WWSearch : public QLineEdit
{
    Q_OBJECT
public:
    explicit WWSearch(QWidget *parent = 0);
    void connectToDb(DBI* dbi);

private:
    void initPopup();

    QTreeWidget* popup;
    QTimer* searchTimer;
    QList<QString> searchtypes;
    QColor disabledColor;
    DBI* db;

signals:
    void addSongToNowPlaying(int sid);
    void openAlbumTab(int alid);
    void openArtistTab(int alid);

public slots:

private slots:
    bool eventFilter(QObject* object, QEvent* e);
    void autoSuggest();

};

#endif // WWSEARCH_H
