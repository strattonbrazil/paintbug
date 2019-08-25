#ifndef BRUSHSIZEBUTTON_H
#define BRUSHSIZEBUTTON_H

#include <QMouseEvent>

#include "labeledtoolbutton.h"

class BrushSizeButton : public LabeledToolButton
{
    Q_OBJECT
public:
    explicit BrushSizeButton(QWidget *parent = nullptr);

    void drawBackground();
    QString toolName();
    QString toolValue();

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:

public slots:
    void onBrushSizeChanged();
private:
    // used for drag events
    QPoint _dragStart;
    int _startBrushSize;
};

#endif // BRUSHSIZEBUTTON_H
