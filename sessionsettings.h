#ifndef SESSIONSETTINGS_H
#define SESSIONSETTINGS_H

#include <QObject>
#include <QColor>
#include <algorithm>

#define MAX_BRUSH_SIZE 256

class SessionSettings : public QObject
{
    Q_OBJECT
public:
    explicit SessionSettings(QObject *parent = nullptr);
    Q_PROPERTY(QColor brushColor MEMBER _brushColor READ brushColor WRITE setBrushColor)
    Q_PROPERTY(int brushSize MEMBER _brushSize READ brushSize WRITE setBrushSize)

    QColor brushColor();
    void setBrushColor(QColor c);

    int brushSize();
    void setBrushSize(int s);
signals:
    void brushColorChanged(QColor oldColor, QColor newColor);
    void brushSizeChanged();
public slots:
private:
    QColor _brushColor;
    int _brushSize;
};

// returns a shared instance of the settings object
SessionSettings* settings();

#endif // SESSIONSETTINGS_H
