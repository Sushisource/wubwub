#ifndef WWTABWIDGET_H
#define WWTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include <QVariant>
#include <QKeyEvent>
#include <QApplication>
#include <QUuid>
#include <memory>
#include <QSet>

class WWTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit WWTabWidget(QWidget *parent = 0);
    void addCloseableTab(QWidget* container, QString name, bool closeable = true);
    void setSongTabText(QString songname, QString arname);
    
signals:
    
public slots:
    void closeMyTab();
    void focusSetter();

protected:
    bool event(QEvent *e);

private:
    const static short btnw = 12;
    QSet<QWidget*> closeableTabs;

    void closeTab(QWidget* tab);
    void closeCurTab();
};

#endif // WWTABWIDGET_H
