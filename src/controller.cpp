#include "controller.h"

Controller::Controller(QWidget *parent) : QWidget(parent)
{
    initInfo();
    initView();
}

void Controller::setMonitor(int index)
{
    current_monitor = monitors.at(index);
}

void Controller::setResolution()
{
    QString monitor(current_monitor->name());
    QString resolution(current_monitor->resolutions()->at(avaCb->currentIndex())->resolution());
    if (resolution != current_monitor->currentResolution()->resolution()) {
        QStringList arg;
        arg << "--output" << monitor << "--mode" << resolution.remove(" ");
        accessShell(arg);

        current_monitor->setCurrentResolution(avaCb->currentIndex());
        currentL->setText(current_monitor->currentResolution()->resolution());
        avaCb->setItemText(avaCb->findText(current_monitor->oldResolution()->text(),
                                           Qt::MatchContains),
                           current_monitor->oldResolution()->textFull());
        avaCb->setItemText(avaCb->findText(current_monitor->currentResolution()->text(),
                                           Qt::MatchContains),
                           current_monitor->currentResolution()->textFull());
    }
}

void Controller::widthEdited()
{
    if (fixed->isChecked()) {
        double ratio = current_monitor->currentResolution()->width().toFloat()
                / current_monitor->currentResolution()->height().toFloat();
        height->setText(getScaled(width->text().toFloat(), ratio));
    }
}

void Controller::heightEdited()
{
    if (fixed->isChecked()) {
        double ratio = current_monitor->currentResolution()->height().toFloat()
                / current_monitor->currentResolution()->width().toFloat();
        width->setText(getScaled(height->text().toFloat(), ratio));
    }
}

void Controller::addResolution()
{
    if (width->hasAcceptableInput() && height->hasAcceptableInput()) {
        QString resolution = width ->text() + " x " + height->text();
        if (avaCb->findText(resolution) == -1) {
            QStringList arg1;
            arg1 << width->text() << height->text();
            QString output = accessShell(arg1, "cvt").split("\n")[1];
            QString hz = output.split(QRegularExpression("\\d{1,5}x\\d{1,5}"))[1]
                    .split("\"")[0].remove("_");
            QString mode = output.split("Modeline ")[1].remove("_" + hz).remove("\"");
            QStringList arg2;
            arg2 << "--newmode";
            arg2 = arg2 + mode.split(" ");
            arg2.removeAll("");
            accessShell(arg2);

            Resolution *r = new Resolution(resolution);
            r->setHz(hz);
            current_monitor->appendResolution(r);
            avaCb->addItem(r->textFull());
            QStringList arg3;
            arg3 << "--addmode" << current_monitor->name() << resolution.remove(" ");
            accessShell(arg3);
        }
    }
}

void Controller::removeResolution()
{
    if (avaCb->currentIndex() == current_monitor->currentIndex()) {
        QMessageBox::warning(this, tr("Warning"),
                                       tr("Can't delete resolution which is being used."),
                                       QMessageBox::Cancel);
    } else {
        QString monitor(current_monitor->name());
        QString resolution(current_monitor->resolutions()->at(avaCb->currentIndex())->resolution());
        QStringList arg1;
        arg1 << "--delmode" << monitor << resolution.remove(" ");
        accessShell(arg1);

        QStringList arg2;
        arg2 << "--rmmode" << resolution.remove(" ");

        avaCb->removeItem(avaCb->currentIndex());
    }
}

void Controller::rotate()
{
    QString monitor = current_monitor->name();
    QString rt = rotCb->currentText().toLower();
    QStringList arg;
    arg << "--output" << monitor << "--rotate" << rt;
    accessShell(arg);
}

void Controller::changeBrightness(int value)
{
    QString monitor = current_monitor->name();
    QString brt = QString::number(double(value) / 100);
    QStringList arg;
    arg << "--output" << monitor << "--brightness" << brt;
    accessShell(arg);
}

void Controller::initInfo()
{
    QStringList arg;
    arg << "--current" << "--verbose";
    QStringList output = accessShell(arg).split("\n", QString::SkipEmptyParts);
    minimum_resolution = new Resolution(output[0].split("minimum ")[1].split(",")[0]);
    maximum_resolution = new Resolution(output[0].split("maximum ")[1].split("\n")[0]);
    output.removeFirst();
    for (QString line : output) {
        if (!line.startsWith(" ") && !line.startsWith("\t")) {
            Monitor *monitor = new Monitor(line.split(" ")[0]);
            monitors.append(monitor);
            if (line.contains(" connected ")) {
                monitor->connect(true);
                if (current_monitor == nullptr)
                    current_monitor = monitor;
            }
        } else {
            if (!line.contains(QRegularExpression("\\s\\s\\d{1,5}x\\d{1,5}\\s"))) {
                if (line.contains("Brightness: ")) {
                    double brt = line.split("Brightness: ")[1].toDouble();
                    monitors.last()->setBrightness(brt);
                } else if (line.contains(QRegularExpression("\\d{2}\\.\\d{2}Hz$"))) {
                    QString hz = line.split(" ").last();
                    monitors.last()->resolutions()->last()->setHz(hz);
                }
            } else {
                QString r = line.split(" ", QString::SkipEmptyParts)[0];
                QString width  = r.split("x")[0];
                QString height = r.split("x")[1];
                Resolution *resolution = new Resolution(width, height);
                monitors.last()->appendResolution(resolution);
                if (line.contains("*current"))
                    monitors.last()->setCurrentResolution(resolution);
            }
        }
    }
}

void Controller::initView()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QFormLayout *infoLayout = new QFormLayout(this);
                 currentL   = new QLabel((current_monitor->currentResolution()->resolution()), this);
    infoLayout->setSpacing(10);
    infoLayout->addRow(tr("Current resolution: "), currentL);
    infoLayout->addRow(tr("Minimum resolution: "), new QLabel(minimum_resolution->resolution()));
    infoLayout->addRow(tr("Maximum resolution: "), new QLabel(maximum_resolution->resolution()));
    infoLayout->addRow(tr("All monitors: "), new QLabel(getMonitorAllNames().join("  ")));

    QGroupBox   *avaG      = new QGroupBox(tr("Available resolutions"), this);
    QGridLayout *avaLayout = new QGridLayout(this);
    QComboBox   *mntCb     = new QComboBox(this);
                 avaCb     = new QComboBox(this);
    QPushButton *set       = new QPushButton(tr("Set"), this);
    QPushButton *rmv       = new QPushButton(tr("Remove"), this);
    mntCb->addItems(getMonitorConnectedNames());
    mntCb->setCurrentIndex(monitors.indexOf(current_monitor));
    avaCb->addItems(current_monitor->resolutionsList());
    avaCb->setCurrentIndex(current_monitor->currentIndex());
    set->setFixedWidth(100);
    rmv->setFixedWidth(100);
    avaLayout->addWidget(mntCb, 0, 0);
    avaLayout->addWidget(avaCb, 0, 1);
    avaLayout->addWidget(set,   0, 2);
    avaLayout->addWidget(rmv,   1, 2);
    avaG->setLayout(avaLayout);

    QGroupBox   *newG      = new QGroupBox(tr("New resolution"), this);
    QHBoxLayout *newLayout = new QHBoxLayout(this);
                 fixed     = new QCheckBox(tr("Fixed Scale"), this);
                 width     = new QLineEdit(this);
                 height    = new QLineEdit(this);
    QPushButton *add       = new QPushButton(tr("Add"), this);
    add->setFixedWidth(100);
    width ->setMaxLength(5);
    height->setMaxLength(5);
    QValidator *wv = new QIntValidator(minimum_resolution->width().toInt(),
                                       maximum_resolution->width().toInt(), this);
    QValidator *hv = new QIntValidator(minimum_resolution->height().toInt(),
                                       maximum_resolution->height().toInt(), this);
    width ->setValidator(wv);
    height->setValidator(hv);
    width ->setText(current_monitor->currentResolution()->width());
    height->setText(current_monitor->currentResolution()->height());
    newLayout->addWidget(fixed);
    newLayout->addWidget(width);
    newLayout->addWidget(new QLabel("x", this));
    newLayout->addWidget(height);
    newLayout->addWidget(add);
    newG->setLayout(newLayout);

    QGroupBox   *rotG      = new QGroupBox(tr("Rotation"), this);
    QGridLayout *rotLayout = new QGridLayout(this);
                 rotCb     = new QComboBox(this);
    QPushButton *rot       = new QPushButton(tr("Rotate"), this);
    rotCb->addItems(rot_list);
    rot->setFixedWidth(100);
    rotLayout->addWidget(rotCb, 0, 0);
    rotLayout->addWidget(rot, 0, 2);
    rotG->setLayout(rotLayout);

    QGroupBox   *brtG      = new QGroupBox(tr("Brightness"), this);
    QHBoxLayout *brtLayout = new QHBoxLayout(this);
    QSlider     *brtSd     = new QSlider(Qt::Horizontal, this);
    brtSd->setValue(current_monitor->brightness() * 100);
    brtSd->setTickInterval(5);
    brtLayout->addWidget(new QLabel("0   ", this));
    brtLayout->addWidget(brtSd);
    brtLayout->addWidget(new QLabel(" 100", this));
    brtG->setLayout(brtLayout);

    mainLayout->setSpacing(20);
    mainLayout->setMargin(20);
    mainLayout->addLayout(infoLayout);
    mainLayout->addWidget(avaG);
    mainLayout->addWidget(newG);
    mainLayout->addWidget(rotG);
    mainLayout->addWidget(brtG);
    setLayout(mainLayout);

    connect(mntCb,  SIGNAL(currentIndexChanged(int)), SLOT(setMonitor(int)));
    connect(set,    SIGNAL(clicked(bool)),            SLOT(setResolution()));
    connect(rmv,    SIGNAL(clicked(bool)),            SLOT(removeResolution()));
    connect(width,  SIGNAL(editingFinished()),        SLOT(widthEdited()));
    connect(height, SIGNAL(editingFinished()),        SLOT(heightEdited()));
    connect(add,    SIGNAL(clicked(bool)),            SLOT(addResolution()));
    connect(rot,    SIGNAL(clicked(bool)),            SLOT(rotate()));
    connect(brtSd,  SIGNAL(valueChanged(int)),        SLOT(changeBrightness(int)));
}

const QString Controller::accessShell(const QString &command)
{
    QProcess *pro = new QProcess(this);
    pro->start(command);
    pro->waitForFinished();
    QString output(pro->readAllStandardOutput());
    return output;
}

const QString Controller::accessShell(const QStringList &arg, const QString &program)
{
    QProcess *pro = new QProcess(this);
    pro->start(program, arg);
    pro->waitForFinished();
    QString output(pro->readAllStandardOutput());
    return output;
}

const QStringList Controller::getMonitorAllNames()
{
    QStringList list;
    for (Monitor *monitor : monitors) {
        list.append(monitor->name());
    }
    return list;
}

const QStringList Controller::getMonitorConnectedNames()
{
    QStringList list;
    for (Monitor *monitor : monitors) {
        if (monitor->connected())
            list.append(monitor->name());
    }
    return list;
}

const QString Controller::getScaled(const float in, const float ratio)
{
    return QString::number(int(in / ratio));
}
