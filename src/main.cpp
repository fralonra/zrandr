#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QTranslator translator;
    translator.load(QString("zrandr_") + QLocale::system().name());

    QApplication a(argc, argv);
    a.installTranslator(&translator);
    a.setStyleSheet("");

    MainWindow w;
    w.show();

    return a.exec();
}
