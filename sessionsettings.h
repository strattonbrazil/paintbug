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

    QColor brushColor() { return _brushColor; }
    void setBrushColor(QColor c) { _brushColor = c; }

    int brushSize() { return _brushSize; }
    void setBrushSize(int s) { _brushSize = std::max(1, std::min(s, MAX_BRUSH_SIZE)); }
signals:

public slots:
private:
    QColor _brushColor;
    int _brushSize;
};

// returns a shared instance of the settings object
SessionSettings* settings();

#endif // SESSIONSETTINGS_H
