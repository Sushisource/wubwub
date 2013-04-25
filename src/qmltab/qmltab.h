#ifndef QMLTAB_H
#define QMLTAB_H

#include <QWidget>
#include <QLayout>
#include <QQuickView>
#include <QQmlEngine>
#include <memory>

class QmlTab : public QWidget
{
    Q_OBJECT
public:
    explicit QmlTab(QString qmlfile, QWidget *parent = 0);

private:
    
signals:
    
public slots:
    
};

#endif // QMLTAB_H
