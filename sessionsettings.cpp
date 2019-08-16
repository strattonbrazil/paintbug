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

}
