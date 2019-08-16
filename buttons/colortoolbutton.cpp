#include "colortoolbutton.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QColorDialog>

#include "sessionsettings.h"

#include <QDebug>

ColorToolButton::ColorToolButton(QWidget *parent) : QToolButton(parent)
{
    connect(this, SIGNAL(released()), this, SLOT(onButtonClicked()));
}


void ColorToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter p(this);
    QBrush b(settings()->brushColor());
    p.setBrush(b);
    QRect colorRect(rect());
    int reduced = width() / 4;
    QMargins m(reduced, 2, reduced, 2) ;
    colorRect = colorRect.marginsRemoved(m);
    p.fillRect( colorRect, b );
}

void ColorToolButton::onButtonClicked()
{
    QColor newColor = QColorDialog::getColor(settings()->brushColor(), this, "Brush Color");
    if (newColor.isValid()) {
        settings()->setBrushColor(newColor);
    }
}
