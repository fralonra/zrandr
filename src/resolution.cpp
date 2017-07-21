#include "resolution.h"

Resolution::Resolution()
{
}

Resolution::Resolution(const Resolution &other)
{
    init(other._width, other._height);
}

Resolution::Resolution(const QString &resolution)
{
    QString data(resolution);
    QString width  = data.split(" x ")[0];
    QString height = data.split(" x ")[1];
    init(width, height);
}

Resolution::Resolution(const QString &width, const QString &height)
{
    init(width, height);
}

Resolution Resolution::operator =(const Resolution &other)
{
    if (this != &other)
        init(other._width, other._height);
    return *this;
}

bool Resolution::operator ==(const Resolution &other) const
{
    if (_current == other._current && _width == other._width && _height == other._height)
        return true;
    return false;
}

void Resolution::init(const QString &width, const QString &height)
{
    _width = width;
    _height = height;
    setText(resolution());
}

void Resolution::current(const bool &arg)
{
     _current = arg;
     if (_current)
         text_full += "  *";
     else
         text_full.remove("  *");
}

void Resolution::setHz(const QString &arg)
{
    _hz = arg;
    if (!_hz.contains("Hz"))
        _hz += "Hz";
    int length = _text.length();
    QString text = _text;
    for (int i = 0; i < 17 - length; ++i)
        text += "  ";
    setText(text + _hz);
}

void Resolution::setText(const QString &arg)
{
    _text = arg;
    setTextFull(_text);
}

void Resolution::setTextFull(const QString &arg)
{
    text_full = arg;
    if (_current)
        text_full += "  *";
}
