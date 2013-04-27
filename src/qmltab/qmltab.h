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
#include "models/roleitemmodel.h"

class QmlTab : public QWidget
{
    Q_OBJECT
public:
    explicit QmlTab(QString qmlfile, QWidget *parent = 0);

protected:
    std::unique_ptr<QQuickItem> root;
    std::unique_ptr<QQmlEngine> engine;

signals:
    
public slots:
    
};

struct AlbumItem {
    enum AlbumRoles {
        NameRole = Qt::UserRole +1,
        CoverRole,
        TracksRole
    };
};

#endif // QMLTAB_H
