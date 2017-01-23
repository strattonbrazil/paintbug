#include "paintutils.h"

void drawOutlinedText(QPainter* painter, int x, int y, QString text, QColor bgColor, QColor fgColor)
{
    painter->setPen(bgColor);
    painter->drawText(x, y, text);
    painter->setPen(fgColor);
    painter->drawText(x-2, y-2, text);
}
