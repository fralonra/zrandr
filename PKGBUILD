# Maintainer: Rong Zou <zoronlivingston@gmail.com> 
 
pkgname=zrandr 
pkgver=0.1 
pkgrel=1 
pkgdesc="" 
arch=('any') 
url="https://github.com/fralonra/zrandr" 
license=('GPL') 
 
depends=('python' 'python-pyqt4') 
makedepends=() 
checkdepends=() 
optdepends=() 
provides=() 
conflicts=() 
replaces=() 
backup=() 
options=() 
install= 
changelog= 
source=("git+https://github.com/fralonra/zrandr.git") 
noextract=() 
md5sums=('SKIP') 
validpgpkeys=() 
 
prepare() { 
  cd "$pkgname-$pkgver" 
  patch -p1 -i "$srcdir/$pkgname-$pkgver.patch" 
} 
 
build() { 
  cd "$pkgname-$pkgver" 
  ./configure --prefix=/usr 
  make 
} 
 
check() { 
  cd "$pkgname-$pkgver" 
  make -k check 
} 
 
package() { 
  cd "$srcdir/$pkgname-$pkgver" 
  install -Dm644 zrandr.desktop -t "$pkgdir/usr/share/applications/" 
  install -Dm644 zrandr.py "$pkgdir/usr/share/zrandr/" 
  install -Dm755 zrandr "$pkgdir/usr/bin/" 
  for icon in zrandr{,-16x16,-64x64,-128x128,-256x256}; do 
    local _resolution="${icon##*-}"; [[ "$_resolution" == "icon" ]] && _resolution="512x512" 
     install -Dm644 icons/$icon.png "$pkgdir/usr/share/icons/hicolor/$_resolution/apps/${icon%%-*}.png" 
  done 
  for prog in zrandr; do 
    install -Dm644 icons/${prog}.svg "$pkgdir/usr/share/icons/hicolor/scalable/apps/$prog.svg" 
  done 
} 
index on master: a5604b7 Initial commit
