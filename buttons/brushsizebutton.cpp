#include "brushsizebutton.h"
#include "sessionsettings.h"

#include <QPainter>
#include <QBrush>
#include <QColor>
#include <algorithm>

#include <iostream>

BrushSizeButton::BrushSizeButton(QWidget *parent) : QToolButton(parent)
{
    //connect(this, SIGNAL(released()), this, SLOT(onButtonClicked()));
    setCursor(Qt::SizeAllCursor);
}

void BrushSizeButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event); // for styling

    QPainter p(this);
    const int brushSize = settings()->brushSize();

    // draw sample brush
    float brushSizeRatio = brushSize / (float)MAX_BRUSH_SIZE;
    int halfW = width() * 0.5f - 5;
    p.drawEllipse(QPoint(width()*0.5f, height()*0.5f), halfW * brushSizeRatio, halfW * brushSizeRatio);

    // draw text
    QString sizeText = QString::number(brushSize) + "px";
    QFont textFont = p.font();
    textFont.setPixelSize(9);
    p.setFont(textFont);
    int textWidth = p.fontMetrics().width(sizeText);
    QPoint textP(width() - textWidth - 2, height() - 2);
    int textHeight = p.fontMetrics().height();
    p.fillRect(textP.x() - 2, textP.y() - 2 - textHeight, textWidth + 4, textHeight + 4, QColor(200, 200, 200, 220));
    p.setPen(Qt::black);
    p.drawText(textP, sizeText);
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
