#include "ssmptabwidget.h"
#include <QDebug>

ssmpTabWidget::ssmpTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
}

void ssmpTabWidget::addCloseableTab(QWidget *container, QString name, bool closeable)
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
        closeableTabs.append(ix);
    }
}

//CLoses tab of signaling button
void ssmpTabWidget::closeMyTab()
{
    QToolButton* clicked = (QToolButton*)QObject::sender();
    int index = clicked->property("tabix").toInt();
    closeTab(index);
}

bool ssmpTabWidget::event(QEvent* e)
{
    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(e);
        if(key->key() == Qt::Key_W && QApplication::keyboardModifiers() & Qt::ControlModifier)
        {
            int curtab = this->currentIndex();
            closeTab(curtab);
        }
    }
    QTabWidget::event(e);
    return false;
}

void ssmpTabWidget::closeTab(int index)
{
    if(closeableTabs.contains(index))
    {
        delete widget(index);
        removeTab(index);
    }
}
