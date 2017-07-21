#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //setFixedSize(QSize(600, 800));
    Controller *c = new Controller(this);
    setCentralWidget(c);
    createMenu();
}

MainWindow::~MainWindow()
{
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::about()
{
    QMessageBox about;
    about.setFixedSize(QSize(300, 300));
    about.setText(tr("Zrandr"
                     "\nA simple xrandr front end based on Qt."));
    about.setInformativeText(tr("Contact me: zoronlivingston@gmail.com"));
    about.setIconPixmap(QPixmap(":/images/icon"));
    about.exec();
}

void MainWindow::createMenu()
{
    QAction *quitAct    = new QAction(tr("&Quit"), this);
    QAction *aboutQtAct = new QAction(tr("About Qt"), this);
    QAction *aboutAct   = new QAction(tr("&About"), this);
    connect(quitAct,    SIGNAL(triggered()), SLOT(close()));
    connect(aboutAct,   SIGNAL(triggered()), SLOT(about()));
    connect(aboutQtAct, SIGNAL(triggered()), SLOT(aboutQt()));

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    fileMenu->addAction(quitAct);
    helpMenu->addAction(aboutQtAct);
    helpMenu->addAction(aboutAct);
}
