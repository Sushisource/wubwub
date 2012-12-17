#ifndef SSMPSEARCH_H
#define SSMPSEARCH_H

#include <QLineEdit>
#include <QTreeWidget>
#include <QHeaderView>
#include <QKeyEvent>
#include "dbi/dbi.h"

class SsmpSearch : public QLineEdit
{
    Q_OBJECT
public:
    explicit SsmpSearch(QWidget *parent = 0);
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

public slots:

private slots:
    bool eventFilter(QObject* object, QEvent* e);
    void autoSuggest();

};

#endif // SSMPSEARCH_H
