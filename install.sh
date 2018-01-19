#! /bin/bash

lrelease -verbose zrandr.pro
qmake zrandr.pro
make

install -Dm644 build/zrandr.desktop -t "/usr/share/applications/"
install -Dm755 zrandr -t "/usr/bin/"

for _i in 16 64 128 256; do
  install -Dm644 build/icons/zrandr-${_i}x${_i}.png "/usr/share/icons/hicolor/${_i}x${_i}/apps/zrandr.png"
done

for prog in zrandr; do
  install -Dm644 build/icons/${prog}.svg "/usr/share/icons/hicolor/scalable/apps/$prog.svg"
done
