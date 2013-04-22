#include "qmltab.h"

QmlTab::QmlTab(QWidget *parent) :
    QWidget(parent)
{
    auto view = std::unique_ptr<QQuickView>(new QQuickView());
    auto container = std::unique_ptr<QWidget>(
                QWidget::createWindowContainer(view.release(), this));
    container->setFocusPolicy(Qt::TabFocus);

    auto layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout(this));
    this->setLayout(layout.release());
    this->layout()->addWidget(container.release());
}
