#ifndef SSMPTABWIDGET_H
#define SSMPTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include <QVariant>
#include <QKeyEvent>
#include <QApplication>

class ssmpTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ssmpTabWidget(QWidget *parent = 0);
    void addCloseableTab(QWidget* container, QString name, bool closeable = true);
    
signals:
    
public slots:
    void closeMyTab();

protected:
    bool event(QEvent *e);

private:
    const static short btnw = 12;
    QList<int> closeableTabs;

    void closeTab(int index);
};

#endif // SSMPTABWIDGET_H