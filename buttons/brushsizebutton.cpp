#include "brushsizebutton.h"
#include "sessionsettings.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <algorithm>

#include <iostream>

BrushSizeButton::BrushSizeButton(QWidget *parent) : QToolButton(parent)
{
    connect(this, SIGNAL(released()), this, SLOT(onButtonClicked()));
}

void BrushSizeButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event); // for styling

    QPainter p(this);
    /*
    QBrush b(settings()->brushColor());
    p.setBrush(b);
    QRect colorRect(rect());
    int reduced = width() / 4;
    QMargins m(reduced, 2, reduced, 2) ;
    colorRect = colorRect.marginsRemoved(m);
    p.fillRect( colorRect, b );
    */

    p.drawText(10, 10, QString::number(settings()->brushSize()));
}

void BrushSizeButton::mousePressEvent(QMouseEvent *event)
{
    QToolButton::mousePressEvent(event); // for styling

    if (event->button() & Qt::LeftButton) {
        _dragStart = event->pos();
        _startBrushSize = settings()->brushSize();
    }
}

void BrushSizeButton::mouseReleaseEvent(QMouseEvent *event)
{
    QToolButton::mouseReleaseEvent(event); // for styling

    if (event->button() & Qt::LeftButton) {
        _dragStart = QPoint();
    }
}

void BrushSizeButton::mouseMoveEvent(QMouseEvent *event)
{
    if (!_dragStart.isNull()) {
        QPoint delta = event->pos() - _dragStart;

        int totalDelta = delta.x() + delta.y();
        int newBrushSize = std::max(1, _startBrushSize + totalDelta);
        settings()->setBrushSize(newBrushSize);

        update();
    }
}
