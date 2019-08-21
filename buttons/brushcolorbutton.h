#ifndef BRUSHCOLORBUTTON_H
#define BRUSHCOLORBUTTON_H

#include <QToolButton>

class BrushColorButton : public QToolButton
{
    Q_OBJECT
public:
    explicit BrushColorButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void onButtonClicked();
};

#endif // BRUSHCOLORBUTTON_H
