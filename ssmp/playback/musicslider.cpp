#include "musicslider.h"

MusicSlider::MusicSlider(QWidget *parent) :
    QSlider(parent)
{
}

void MusicSlider::mouseReleaseEvent(QMouseEvent *ev)
{
    float max = maximum();
    if (ev->button() == Qt::LeftButton)
    {
        float ratio = (float)ev->x() / width();
        float newval = max * ratio;
        setValue(newval);
        ev->accept();
        emit seekTo(ratio);
    }
    QSlider::mousePressEvent(ev);
}
