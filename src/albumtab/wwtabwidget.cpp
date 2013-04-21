#include "wwtabwidget.h"
#include <QDebug>

Q_DECLARE_METATYPE(QWidget *)

WWTabWidget::WWTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    connect(this, &WWTabWidget::currentChanged,
            this, &WWTabWidget::focusSetter);
}

void WWTabWidget::addCloseableTab(QWidget *container, QString name, bool closeable)
{
    int ix = this->addTab(container, name);
    if(closeable)
    {
        auto close = std::unique_ptr<QToolButton>(new QToolButton(container));
        close->setStyleSheet("background-color: rgba( 255, 255, 255, 0% );");
        close->setGeometry(QRect(0,0,btnw,btnw));
        close->setProperty("tabpointer", qVariantFromValue(container));
        connect(close.get(), SIGNAL(clicked()), SLOT(closeMyTab()));
        QIcon closeicon(QPixmap(":/imgs/x").scaledToWidth(btnw,Qt::SmoothTransformation));
        close->setIcon(closeicon);
        tabBar()->setTabButton(ix, QTabBar::RightSide, close.release());
    }
    this->setCurrentWidget(container);
}

void WWTabWidget::setSongTabText(QString songname, QString arname)
{
    this->setTabText(0, songname);
    this->setTabToolTip(0, arname + " - " + songname);
}

//Closes tab of signaling button
void WWTabWidget::closeMyTab()
{
    QToolButton* clicked = qobject_cast<QToolButton*>(QObject::sender());
    delete clicked->property("tabpointer").value<QWidget*>();
}

void WWTabWidget::focusSetter()
{
    this->setFocus();
}

bool WWTabWidget::event(QEvent* e)
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

void WWTabWidget::closeCurTab()
{
    int curix = this->currentIndex();
    delete widget(curix);
}
