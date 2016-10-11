import sys
import os
import re
from PyQt4.QtGui import *
from PyQt4.QtCore import *

class Zrandr(QWidget):

    def __init__(self):
        super(Zrandr, self).__init__()
        self.setWindowTitle('Zrandr')

        self.monitor = self.getMonitor()

        layout = QVBoxLayout(self)
        resolutions, current = self.getResolution()
        w = str(current.get('width'))
        h = str(current.get('height'))
        self.current = w + 'x' + h
        self.w_old, self.h_old = float(w), float(h)
        
        # Available Layouts
        arLayout = QHBoxLayout()
        self.box = QComboBox()
        box = self.box
        for i in resolutions:
            box.addItem(i['width'] + 'x' + i['height'])
        arLayout.addWidget(QLabel('Available Resolutions : '))
        arLayout.addWidget(box)

        # Buttons
        btnLayout = QHBoxLayout()
        aplBtn = QPushButton('Apply')
        rmBtn = QPushButton('Remove')
        btnLayout.addWidget(aplBtn)
        btnLayout.addWidget(rmBtn)

        # Current Layout
        self.currentL = QLabel()
        self.showCurrent()

        # New Layout
        nrLayout = QHBoxLayout()
        newL = QLabel('New resolution : ')
        fix = QCheckBox('Fixed Scale')
        self.width = QLineEdit(w)
        self.height = QLineEdit(h)
        addBtn = QPushButton('Add')
        nrLayout.addWidget(newL)
        nrLayout.addWidget(fix)
        nrLayout.addWidget(self.width)
        nrLayout.addWidget(QLabel('x'))
        nrLayout.addWidget(self.height)
        nrLayout.addWidget(addBtn)

        # Rotations
        rtLayout = QHBoxLayout()
        rtL = QLabel('Rotations :')
        rt = QComboBox()
        rotations = ['Normal', 'Left', 'Right', 'Inverted']
        rt.addItems(rotations)
        rtBtn = QPushButton('Rotate')
        rtLayout.addWidget(rtL)
        rtLayout.addWidget(rt)
        rtLayout.addWidget(rtBtn)

        # Brightness
        brtLayout = QHBoxLayout()
        brtL = QLabel('Brightness :')
        brt = QSlider(Qt.Horizontal)
        brt.setMaximum(100)
        brt.setMinimum(0)
        brt.setValue(50)
        brt.setTickPosition(QSlider.TicksBelow)
        brt.setTickInterval(5)
        brtLayout.addWidget(brtL)
        brtLayout.addWidget(brt)

        qLayout = QHBoxLayout()
        qBtn = QPushButton('Quit')
        qLayout.addStretch()
        qLayout.addWidget(qBtn)

        # Connect Slot
        box.currentIndexChanged.connect(self.selectResolution)
        fix.stateChanged.connect(lambda: self.fixScale(fix))
        addBtn.clicked.connect(self.addResolution)
        rtBtn.clicked.connect(lambda: self.rtResolution(rt.currentText()))
        brt.valueChanged.connect(lambda: self.bright(brt.value()))
        aplBtn.clicked.connect(self.setResolution)
        rmBtn.clicked.connect(self.rmResolution)
        qBtn.clicked.connect(self.cancel)

        layout.addWidget(self.currentL)
        layout.addLayout(arLayout)
        layout.addLayout(btnLayout)
        layout.addLayout(nrLayout)
        layout.addLayout(rtLayout)
        layout.addLayout(brtLayout)
        layout.addLayout(qLayout)

        center(self)

    def showCurrent(self):
        self.currentL.setText('Current resolution : ' + self.current)

    def getResolution(self):
        a = os.popen('xrandr').read()
        _list = a.splitlines()
        resolutions = []
        currentResolution = {}
        for i in _list:
            resolution = i.split()
            if 'x' in resolution[0]:
                width, height = resolution[0].split('x')
                fps = re.sub("[^0-9.]", "", resolution[1])
                resolutions.append({'width': width, 'height': height, 'fps': fps})
                if '*' in resolution[1]:
                    currentResolution = {'width': width, 'height': height, 'fps': fps}
        return resolutions, currentResolution

    def getMonitor(self):
        a = os.popen('xrandr').read()
        _list = a.splitlines()
        for i in _list:
            line = _list.index(i)
            if line == 1:
                monitor = i.split()
                monitor = monitor[0]
        return monitor

    def selectResolution(self):
        resolution = self.box.currentText()
        return resolution

    def fixScale(self, fix):
        if fix.isChecked() is True:
            self.width.editingFinished.connect(lambda: self.custom('w'))
            self.height.editingFinished.connect(lambda: self.custom('h'))

    def custom(self, arg):
        if arg == 'w':
            w = float(self.width.text())
            scale = w / self.w_old
            h = round(self.h_old * scale)
            self.height.setText(str(h))
        else:
            h = float(self.height.text())
            scale = h / self.h_old
            w = round(self.w_old * scale)
            self.width.setText(str(w))
        self.w_old, self.h_old = w, h

    def addResolution(self):
        modeline = ''
        w, h = self.width.text(), self.height.text()
        add = w + 'x' + h

        # init modeline
        a = os.popen('cvt ' + w + ' ' + h).read()
        _list = a.splitlines()
        for i in _list:
            line = _list.index(i)
            if line == 1:
                modeline = i.split(' ', 2)[2]

        alls = [self.box.itemText(i) for i in range(self.box.count())]
        if add in alls:
            QMessageBox.warning(self, "Warning", "Already have this resolution !", QMessageBox.Cancel)
        else:
            os.system('xrandr --newmode "' + add + '" ' + modeline)
            self.box.addItem(add)

    def rtResolution(self, rt):
        os.system('xrandr --output ' + self.monitor + ' --rotate ' + rt)

    def bright(self, brt):
        brt = str(brt / 100)
        os.system('xrandr --output ' + self.monitor + ' --brightness ' + brt)

    def setResolution(self):
        self.old = self.current
        self.current = self.selectResolution()
        os.system('xrandr --addmode ' + self.monitor + ' ' + self.current)
        os.system('xrandr --output ' + self.monitor + ' --mode ' + self.current)
        self.showCurrent()
        self.keepResolution()

    def keepResolution(self):
        self.dialog = QDialog()
        l = QLabel('Keep this resolution?')
        keep = QPushButton('Yes')
        resume = QPushButton('No')

        keep.clicked.connect(lambda: self.btnClicked('y'))
        resume.clicked.connect(lambda: self.btnClicked('n'))

        hlayout = QHBoxLayout()
        hlayout.addWidget(keep)
        hlayout.addWidget(resume)
        layout = QVBoxLayout()
        layout.addWidget(l)
        layout.addLayout(hlayout)

        self.dialog.setLayout(layout)

        self.dialog.exec_()

    def rmResolution(self):
        msg = QMessageBox()
        msg.setWindowTitle('Warning')
        msg.setIcon(2)
        msg.setInformativeText('Are you sure you want to remove mode ' + self.selectResolution())
        msg.setStandardButtons(QMessageBox.Yes | QMessageBox.No)
        msg.buttonClicked.connect(lambda: self.btnClicked('rm'))

        msg.exec_()

    def btnClicked(self, arg):
        if arg == 'y':
                self.dialog.close()
        elif arg == 'n':
            os.system('xrandr --output ' + self.monitor + ' --mode ' + self.old)
            self.current = self.old
            self.showCurrent()
            self.dialog.close()
        elif arg == 'rm':
            rm = self.selectResolution()
            os.system('xrandr --delmode ' + self.monitor + ' ' + rm)
            os.system('xrandr --rmmode ' + rm)
            self.box.removeItem(self.box.currentIndex())

    def cancel(self):
        sys.exit()

def center(self):
    screen = QDesktopWidget().screenGeometry()
    size = self.geometry()
    self.move((screen.width() - size.width()) / 2, (screen.height() - size.height()) / 2)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    zrandr = Zrandr()
    zrandr.show()
    sys.exit(app.exec_())
