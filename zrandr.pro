#-------------------------------------------------
#
# Project created by QtCreator 2017-07-20T21:27:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = zrandr
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/controller.cpp \
    src/monitor.cpp \
    src/resolution.cpp

HEADERS += src/controller.h \
    src/mainwindow.h \
    src/monitor.h \
    src/resolution.h

RESOURCES += \
    resources.qrc

TRANSLATIONS += i18n/zrandr_zh_CN.ts

CONFIG += C++11
