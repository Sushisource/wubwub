#include "ssmptabwidget.h"
#include <QDebug>

Q_DECLARE_METATYPE(QWidget *)

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
        qDebug() << container;
        close->setStyleSheet("background-color: rgba( 255, 255, 255, 0% );");
        close->setGeometry(QRect(0,0,btnw,btnw));
        close->setProperty("tabpointer", qVariantFromValue(container));
        connect(close, SIGNAL(clicked()), SLOT(closeMyTab()));
        QIcon closeicon(QPixmap(":/imgs/x").scaledToWidth(btnw,Qt::SmoothTransformation));
        close->setIcon(closeicon);
        tabBar()->setTabButton(ix, QTabBar::RightSide, close);
    }
}

//Closes tab of signaling button
void ssmpTabWidget::closeMyTab()
{
    QToolButton* clicked = qobject_cast<QToolButton*>(QObject::sender());
    delete clicked->property("tabpointer").value<QWidget*>();
}

bool ssmpTabWidget::event(QEvent* e)
{
    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent* key = static_cast<QKeyEvent*>(e);
        if(key->key() == Qt::Key_W && QApplication::keyboardModifiers() & Qt::ControlModifier)
        {
            closeCurTab();
        }
    }
    QTabWidget::event(e);
    return false;
}

void ssmpTabWidget::closeCurTab()
{
    int curix = this->currentIndex();
    delete widget(curix);
}
