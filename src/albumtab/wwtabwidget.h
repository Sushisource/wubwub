#ifndef WWTABWIDGET_H
#define WWTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include <QVariant>
#include <QKeyEvent>
#include <QApplication>
#include <QUuid>

class WWTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit WWTabWidget(QWidget *parent = 0);
    void addCloseableTab(QWidget* container, QString name, bool closeable = true);
    
signals:
    
public slots:
    void closeMyTab();
    void focusSetter(int index);

protected:
    bool event(QEvent *e);

private:
    const static short btnw = 12;

    void closeTab(QWidget* tab);
    void closeCurTab();
};

#endif // WWTABWIDGET_H
