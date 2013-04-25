#include "qmltab.h"

QmlTab::QmlTab(QString qmlfile, QWidget *parent) :
    QWidget(parent)
{
    auto view = std::unique_ptr<QQuickView>(new QQuickView());
    view->engine()->addImportPath("qrc:/qml/");
    view->setSource(QUrl(qmlfile));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    auto container = std::unique_ptr<QWidget>(
                QWidget::createWindowContainer(view.release(), this));
    container->setFocusPolicy(Qt::TabFocus);

    auto layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout(this));
    this->setLayout(layout.release());
    this->layout()->addWidget(container.release());
}
