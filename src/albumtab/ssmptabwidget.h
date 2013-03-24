#ifndef SSMPTABWIDGET_H
#define SSMPTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include <QVariant>
#include <QKeyEvent>
#include <QApplication>
#include <QUuid>

class ssmpTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ssmpTabWidget(QWidget *parent = 0);
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

#endif // SSMPTABWIDGET_H
