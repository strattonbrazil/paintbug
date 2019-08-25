#ifndef BRUSHCOLORBUTTON_H
#define BRUSHCOLORBUTTON_H

#include "labeledtoolbutton.h"

class BrushColorButton : public LabeledToolButton
{
    Q_OBJECT
public:
    explicit BrushColorButton(QWidget *parent = nullptr);
    void drawBackground();
    QString toolName();
    QString toolValue();
signals:

public slots:
    void onButtonClicked();
};

#endif // BRUSHCOLORBUTTON_H
