#ifndef PRETTYTEXT_H
#define PRETTYTEXT_H

#include <QGraphicsSimpleTextItem>
#include <QTimeLine>
#include <QBrush>
#include <QObject>
#include <QGraphicsObject>

class PrettyText : public QGraphicsSimpleTextItem
{
private:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    static const short shade = 90;

public:
//    QTimeLine* tl;
    explicit PrettyText(QGraphicsItem *parent = 0);
    
};

#endif // PRETTYTEXT_H
