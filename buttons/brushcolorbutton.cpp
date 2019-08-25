#include "brushcolorbutton.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QColorDialog>

#include "sessionsettings.h"

BrushColorButton::BrushColorButton(QWidget *parent) : LabeledToolButton(parent)
{
    connect(this, SIGNAL(released()), this, SLOT(onButtonClicked()));
}


void BrushColorButton::drawBackground()
{
    QPainter p(this);
    QBrush b(settings()->brushColor());
    p.setBrush(b);
    QRect colorRect(rect());
    int reduced = 4;
    QMargins m(reduced, reduced, reduced, reduced) ;
    colorRect = colorRect.marginsRemoved(m);
    p.fillRect(colorRect, b);
}

QString BrushColorButton::toolName()
{
    return "Brush Color";
}

QString BrushColorButton::toolValue()
{
    return "";
}

void BrushColorButton::onButtonClicked()
{
    QColor newColor = QColorDialog::getColor(settings()->brushColor(), this, "Brush Color");
    if (newColor.isValid()) {
        settings()->setBrushColor(newColor);
        update();
    }
}
