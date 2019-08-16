#ifndef SESSIONSETTINGS_H
#define SESSIONSETTINGS_H

#include <QObject>
#include <QColor>

class SessionSettings : public QObject
{
    Q_OBJECT
public:
    explicit SessionSettings(QObject *parent = nullptr);
    Q_PROPERTY(QColor brushColor MEMBER _brushColor READ brushColor WRITE setBrushColor)

    QColor brushColor() { return _brushColor; }
    void setBrushColor(QColor c) { _brushColor = c; }
signals:

public slots:
private:
    QColor _brushColor;
};

// returns a shared instance of the settings object
SessionSettings* settings();

#endif // SESSIONSETTINGS_H
