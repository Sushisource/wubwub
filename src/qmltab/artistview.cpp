#include "artistview.h"

ArtistView::ArtistView(int arid, QWidget* parent, QString qmlfile) :
    QmlAlbumTab(qmlfile, parent)
{
    addAlbums(db->getArtistAlbums(arid));
}
