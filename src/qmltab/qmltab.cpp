#include "qmltab.h"

QmlTab::QmlTab(QString qmlfile, QWidget *parent) :
    QWidget(parent)
{
    tabparent = std::unique_ptr<QWidget>(parent);
    auto view = std::unique_ptr<QQuickView>(new QQuickView());
    view->setColor(QApplication::palette().window().color());
    view->setClearBeforeRendering(true);
    engine = std::unique_ptr<QQmlEngine>(view->engine());
    engine->addImportPath("qrc:/qml/");
    view->setSource(QUrl(qmlfile));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->installEventFilter(parent);

    root = std::unique_ptr<QQuickItem>(view->rootObject());

    auto container = std::unique_ptr<QWidget>(
                QWidget::createWindowContainer(view.release(), this));

    auto layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout(this));
    this->setLayout(layout.release());
    this->layout()->addWidget(container.release());
    this->installEventFilter(parent);
}

QmlTab::~QmlTab()
{
    tabparent.release();
}
