#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenuBar>
#include "controller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void aboutQt();
    void about();

private:
    void createMenu();
};

#endif // MAINWINDOW_H
