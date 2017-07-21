#ifndef MONITOR_H
#define MONITOR_H

#include <QVector>

#include "resolution.h"

class Monitor : public QObject
{
    Q_OBJECT
public:
    Monitor(const QString &name, QObject *parent = nullptr);

    const double brightness() const {return _brightness;}
    const int indexOf(Resolution *resolution) const
    {
        return _resolutions.indexOf(resolution);
    }
    const int currentIndex() const
    {
        return _resolutions.indexOf(current_resolution);
    }
    const QString name() const {return _name;}
    const bool connected() const {return _connected;}
    const bool containsResolution(const Resolution *resolution) const;
    const QVector<Resolution*>* resolutions() const {return &_resolutions;}
    const QStringList resolutionsList();
    Resolution* currentResolution() const {return current_resolution;}
    Resolution* oldResolution() const {return old_resolution;}

    void connect(const bool &arg) {_connected = arg;}
    void appendResolution(Resolution *resolution);
    void setBrightness(const double brightness) {_brightness = brightness;}
    void setCurrentResolution(const int index);
    void setCurrentResolution(Resolution *resolution);

signals:
    void resolutionChanged(int);

public slots:

private:
    double _brightness = 1.0;
    const QString _name;
    bool _connected = false;
    QVector<Resolution*> _resolutions;
    Resolution *current_resolution = nullptr;
    Resolution *old_resolution = nullptr;
};

#endif // MONITOR_H
