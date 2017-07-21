#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <QtCore>

class Resolution
{
public:
    Resolution();
    Resolution(const Resolution &other);
    Resolution(const QString &resolution);
    Resolution(const QString &width, const QString &height);
    Resolution operator =(const Resolution &other);
    bool operator ==(const Resolution &other) const;

    const bool isCurrent() const {return _current;}
    const QString width() const {return _width;}
    const QString height() const {return _height;}
    const QString resolution() const {return _width + " x " + _height;}
    const QString hz() const {return _hz;}
    const QString text() const {return _text;}
    const QString textFull() const {return text_full;}

    void init(const QString &width, const QString &height);
    void current(const bool &arg);
    void setHz(const QString &arg);

private:
    bool _current = false;
    QString _width;
    QString _height;
    QString _hz;
    QString _text;
    QString text_full;

    void setText(const QString &arg);
    void setTextFull(const QString &arg);
};

#endif // RESOLUTION_H
