#include "artistalbumsview.h"

//Create a new artist view with the given artist ID
ArtistAlbumsView::ArtistAlbumsView(int arid, QWidget *parent) : AlbumView(parent)
{
    maxAlbs = INT_MAX; //UNLIMITEDDDDDD
    addAlbs(db->getArtistAlbums(arid));
}
