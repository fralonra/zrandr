#include "monitor.h"

Monitor::Monitor(const QString &name, QObject *parent) : QObject(parent), _name(name)
{
}

const bool Monitor::containsResolution(const Resolution *resolution) const
{
    for (Resolution *r : _resolutions) {
        if (r->resolution() == resolution->resolution())
            return true;
    }
    return false;
}

const QStringList Monitor::resolutionsList()
{
    QStringList list;
    for (const Resolution *resolution : _resolutions)
        list.append(resolution->textFull());
    return list;
}

void Monitor::appendResolution(Resolution *resolution)
{
    if (!_resolutions.contains(resolution))
        _resolutions.append(resolution);
    if (current_resolution == nullptr) {
        if (resolution->isCurrent())
            setCurrentResolution(current_resolution);
    }
}

void Monitor::setCurrentResolution(const int index)
{
    setCurrentResolution(_resolutions.at(index));
}

void Monitor::setCurrentResolution(Resolution *resolution)
{
    if (current_resolution != nullptr) {
        old_resolution = current_resolution;
        old_resolution->current(false);
    }
    resolution->current(true);
    current_resolution = resolution;
}
