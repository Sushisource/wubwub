#include "ssmptabwidget.h"

ssmpTabWidget::ssmpTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
}

void ssmpTabWidget::addCTab(QWidget *container, QString name, bool closeable)
{
    int ix = this->addTab(container, name);
    if(closeable)
    {
        QToolButton* close = new QToolButton(container);
        close->setStyleSheet("background-color: rgba( 255, 255, 255, 0% );");
        close->setGeometry(QRect(0,0,btnw,btnw));
        close->setProperty("tabix", QVariant(ix));
        connect(close, SIGNAL(clicked()), SLOT(closeMyTab()));
        QIcon closeicon(QPixmap(":/imgs/x").scaledToWidth(btnw,Qt::SmoothTransformation));
        close->setIcon(closeicon);
        tabBar()->setTabButton(ix, QTabBar::RightSide, close);
    }
}

//CLoses tab of signaling button
void ssmpTabWidget::closeMyTab()
{
    QToolButton* clicked = (QToolButton*)QObject::sender();
    int index = clicked->property("tabix").toInt();
    delete widget(index);
    removeTab(index);
}
