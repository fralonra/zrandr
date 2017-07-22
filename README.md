# Zrandr
A simple Qt5 front end for xrandr.

## Features:  
* Setting screen resolution
* Adding unrecognized resolutions
* Rotating screen
* Changing screen brightness

## Dependencies
* qt5
* xrandr

## Installation
### Archlinux
```
$ yaourt -S zrandr
```
### Debian-based i386 Distributions
* Install qt5-base and x11-xserver-utils packages.
* Download the latest release .deb file and install.
### Others
* First make sure you have installed all dependencies.
* For most deb-based distros (Debian, Ubuntu, Linux Mint), they are **qt5-base** and **x11-xserver-utils**.
* And for rpm-baesed, they are **qt5-qtbase** and **xrandr** (or **xorg-x11-server-utils** in Fedora) or something else.
* After all dependencies are ready, run the following commands.
```
$ qmake zrandr.pro
$ make
# install.sh
```

