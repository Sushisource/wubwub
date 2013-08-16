#include "prettytext.h"

PrettyText::PrettyText(QGraphicsItem* parent) : QGraphicsSimpleTextItem(parent)
{
}

void PrettyText::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
    this->setBrush(QBrush(QColor::fromRgb(shade,shade,shade), Qt::SolidPattern));
}

void PrettyText::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
    this->setBrush(QBrush(QColor::fromRgb(0,0,0), Qt::SolidPattern));
}

PrettyText::~PrettyText()
{

}
