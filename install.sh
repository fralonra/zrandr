#! /bin/bash

install -Dm644 zrandr.desktop -t "/usr/share/applications/"
install -Dm644 zrandr.py -t "/usr/share/zrandr/"
install -Dm755 zrandr -t "/usr/bin/"

for _i in 16 64 128 256; do
  install -Dm644 icons/zrandr-${_i}x${_i}.png "/usr/share/icons/hicolor/${_i}x${_i}/apps/zrandr.png"
done

for prog in zrandr; do
  install -Dm644 icons/${prog}.svg "/usr/share/icons/hicolor/scalable/apps/$prog.svg"
done
