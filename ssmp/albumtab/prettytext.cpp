#include "prettytext.h"

PrettyText::PrettyText(QGraphicsItem* parent) : QGraphicsSimpleTextItem(parent)
{
    //tl = new QTimeLine(500);
    //tl->setFrameRange(0,55);
}

void PrettyText::hoverEnterEvent(QGraphicsSceneHoverEvent* e)
{
    this->setBrush(QBrush(QColor::fromRgb(shade,shade,shade), Qt::SolidPattern));
}

void PrettyText::hoverLeaveEvent(QGraphicsSceneHoverEvent* e)
{
    this->setBrush(QBrush(QColor::fromRgb(0,0,0), Qt::SolidPattern));
    //tl->start();
}

PrettyText::~PrettyText()
{

}

/*void PrettyText::colorTick(int i)
{
    this->setBrush(QBrush(QColor::fromRgb(shade+i,shade+i,shade+i), Qt::SolidPattern));
}*/
