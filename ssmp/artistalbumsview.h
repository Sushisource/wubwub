#ifndef ARTISTALBUMSVIEW_H
#define ARTISTALBUMSVIEW_H

#include "albumview.h"

class ArtistAlbumsView : public AlbumView
{
    Q_OBJECT

public:
    explicit ArtistAlbumsView(int arid, QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // ARTISTALBUMSVIEW_H
