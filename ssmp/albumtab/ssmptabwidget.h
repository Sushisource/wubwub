#ifndef SSMPTABWIDGET_H
#define SSMPTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QToolButton>
#include <QVariant>

class ssmpTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ssmpTabWidget(QWidget *parent = 0);
    void addCTab(QWidget* container, QString name, bool closeable = true);
    
signals:
    
public slots:
    void closeMyTab();

private:
    const static short btnw = 12;
};

#endif // SSMPTABWIDGET_H
