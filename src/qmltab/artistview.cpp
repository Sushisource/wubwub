#include "artistview.h"

ArtistView::ArtistView(int arid, QWidget* parent, QString qmlfile) :
    AlbumView(qmlfile, parent)
{
    addAlbums(db->getArtistAlbums(arid));
}
