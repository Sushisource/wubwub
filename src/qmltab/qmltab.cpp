#include "qmltab.h"

QmlTab::QmlTab(QString qmlfile, QWidget *parent) :
    QWidget(parent)
{
    auto view = std::unique_ptr<QQuickView>(new QQuickView());
    view->setColor(QApplication::palette().window().color());
    view->setClearBeforeRendering(true);
    engine = std::unique_ptr<QQmlEngine>(view->engine());
    engine->addImportPath("qrc:/qml/");
    view->setSource(QUrl(qmlfile));
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    root = std::unique_ptr<QQuickItem>(view->rootObject());


    auto container = std::unique_ptr<QWidget>(
                QWidget::createWindowContainer(view.release(), this));
    container->setFocusPolicy(Qt::TabFocus);

    auto layout = std::unique_ptr<QHBoxLayout>(new QHBoxLayout(this));
    this->setLayout(layout.release());
    this->layout()->addWidget(container.release());
}

void QmlTab::addComponent()
{
    QVariantMap alrecord;
    alrecord["alname"] = "Alname";
    QVariantList trax;
    QVariantMap track1;
    track1["track"] = "1";
    track1["song"] = "lololollolo";
    trax.append(track1);
    alrecord["alcover"] = "file:../res/album_example.jpg";
    alrecord["tracks"] = trax;
    QMetaObject::invokeMethod(root.get(), "addAlbum",
                              Q_ARG(QVariant,
                                    QVariant::fromValue(alrecord)));
}
