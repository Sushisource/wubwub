#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QSlider>
#include <QMouseEvent>

class MusicSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MusicSlider(QWidget *parent = 0);

protected:
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void seekTo(float ratio);
    
public slots:
    
};

#endif // MUSICSLIDER_H
