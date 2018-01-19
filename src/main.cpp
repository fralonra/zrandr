#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QTranslator translator;
    bool res = translator.load(QString("zrandr_") + QLocale::system().name(), ":/i18n");

    QApplication a(argc, argv);
    if (res)
        a.installTranslator(&translator);
    a.setStyleSheet("");

    MainWindow w;
    w.show();

    return a.exec();
}
