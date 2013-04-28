#ifndef ARTISTVIEW_H
#define ARTISTVIEW_H
#include "qmlalbumtab.h"

class ArtistView : public QmlAlbumTab
{
    Q_OBJECT
public:
    explicit ArtistView(int arid, QWidget* parent,
                        QString qmlfile = "qrc:/qml/qml/artistview.qml");
    
signals:
    
public slots:
    
};

#endif // ARTISTVIEW_H
