#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QWidget>
#include <QProcess>
#include <QPushButton>

#include "monitor.h"

class Controller : public QWidget
{
    Q_OBJECT
public:
    explicit Controller(QWidget *parent = nullptr);

signals:

public slots:
    void setMonitor(int index);
    void setResolution();
    void widthEdited();
    void heightEdited();
    void addResolution();
    void removeResolution();
    void rotate();
    void changeBrightness(int value);

private:
    QLabel *currentL = nullptr;
    QComboBox *avaCb = nullptr;
    QCheckBox *fixed = nullptr;
    QLineEdit *width = nullptr;
    QLineEdit *height= nullptr;
    QComboBox *rotCb = nullptr;

    Monitor *current_monitor = nullptr;
    Resolution *minimum_resolution = nullptr;
    Resolution *maximum_resolution = nullptr;
    QVector<Monitor*> monitors;
    const QStringList rot_list = {tr("Normal"), tr("Left"), tr("Right"), tr("Inverted")};

private:
    void initInfo();
    void initView();

    const QString accessShell(const QString &command);
    const QString accessShell(const QStringList &arg, const QString &program = "xrandr");
    const QStringList getMonitorAllNames();
    const QStringList getMonitorConnectedNames();

    const QString getScaled(const float in, const float ratio);
};

#endif // CONTROLLER_H
