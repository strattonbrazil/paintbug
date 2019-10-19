#include "brushsizebutton.h"
#include "sessionsettings.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <algorithm>

#include <iostream>

BrushSizeButton::BrushSizeButton(QWidget *parent) : LabeledToolButton(parent)
{
    //connect(this, SIGNAL(released()), this, SLOT(onButtonClicked()));
    setCursor(Qt::SizeAllCursor);

    connect(settings(), SIGNAL(brushSizeChanged()), this, SLOT(onBrushSizeChanged()));
}

void BrushSizeButton::drawBackground()
{
    QPainter p(this);
    const int brushSize = settings()->brushSize();

    // draw sample brush
    float brushSizeRatio = brushSize / (float)MAX_BRUSH_SIZE;
    int halfW = width() * 0.5f - 5;
    p.drawEllipse(QPointF(width()*0.5f, height()*0.5f), halfW * brushSizeRatio, halfW * brushSizeRatio);
}

QString BrushSizeButton::toolName()
{
    return "Brush Size";
}

QString BrushSizeButton::toolValue()
{
    const int brushSize = settings()->brushSize();
    return QString::number(brushSize) + "px";
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

void BrushSizeButton::onBrushSizeChanged()
{
    update(); // redraw with new brush size
}
