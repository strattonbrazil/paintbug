#ifndef LABELEDTOOLBUTTON_H
#define LABELEDTOOLBUTTON_H

#include <QToolButton>

class LabeledToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit LabeledToolButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

    virtual QString toolName() = 0;
    virtual QString toolValue() = 0;
    virtual void drawBackground() = 0;
signals:

public slots:
};

#endif // LABELEDTOOLBUTTON_H
