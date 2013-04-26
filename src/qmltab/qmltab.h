#ifndef QMLTAB_H
#define QMLTAB_H

#include <QWidget>
#include <QLayout>
#include <QQuickView>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlComponent>
#include <memory>

class QmlTab : public QWidget
{
    Q_OBJECT
public:
    explicit QmlTab(QString qmlfile, QWidget *parent = 0);
    void addComponent();

private:
    std::unique_ptr<QQuickItem> root;
    std::unique_ptr<QQmlEngine> engine;

signals:
    
public slots:
    
};

#endif // QMLTAB_H
