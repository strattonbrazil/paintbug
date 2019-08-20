#ifndef BRUSHSIZEBUTTON_H
#define BRUSHSIZEBUTTON_H

#include <QToolButton>
#include <QMouseEvent>

class BrushSizeButton : public QToolButton
{
    Q_OBJECT
public:
    explicit BrushSizeButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
signals:

public slots:
private:
    // used for drag events
    QPoint _dragStart;
    int _startBrushSize;
};

#endif // BRUSHSIZEBUTTON_H
