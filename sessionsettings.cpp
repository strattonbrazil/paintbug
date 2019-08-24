#include "sessionsettings.h"

SessionSettings *_settings = 0;
SessionSettings* settings()
{
    if (!_settings) {
        _settings = new SessionSettings();
    }
    return _settings;
}

SessionSettings::SessionSettings(QObject *parent) : QObject(parent)
{
    _brushColor = QColor(0,0,0);
    _brushSize = 64;
}

QColor SessionSettings::brushColor()
{
    return _brushColor;
}

void SessionSettings::setBrushColor(QColor c)
{
    emit brushColorChanged(_brushColor, c);
    _brushColor = c;
}

int SessionSettings::brushSize()
{
    return _brushSize;
}

void SessionSettings::setBrushSize(int s)
{
    _brushSize = std::max(1, std::min(s, MAX_BRUSH_SIZE));
    emit brushSizeChanged();
}
