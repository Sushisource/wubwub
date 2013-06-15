#ifndef QMLTAB_H
#define QMLTAB_H

#include <QWidget>
#include <QLayout>
#include <QQuickView>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlComponent>
#include <QVariantList>
#include <QApplication>
#include <memory>
#include "dbi/dbi.h"

class QmlTab : public QWidget
{
    Q_OBJECT
public:
    explicit QmlTab(QString qmlfile, QWidget *parent = 0);
    ~QmlTab();
    std::unique_ptr<QQuickItem> root;
    std::unique_ptr<QQmlEngine> engine;

};

struct AlbumItem {
    enum AlbumRoles {
        NameRole = Qt::UserRole +1,
        CoverRole,
        TracksRole
    };
};

#endif // QMLTAB_H
