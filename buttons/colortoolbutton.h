#ifndef COLORTOOLBUTTON_H
#define COLORTOOLBUTTON_H

#include <QToolButton>

class ColorToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ColorToolButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void onButtonClicked();
};

#endif // COLORTOOLBUTTON_H
