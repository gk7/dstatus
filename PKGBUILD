pkgname=dstatus
_gitname=dstatus
pkgver=2013.04.26
pkgrel=1
pkgdesc='Statusbar for DWM written in C'
arch=('any')
url='https://github.com/gk7/dstatus'
license=('GPL2')
groups=('custom')
depends=('libx11' 'alsa-lib')
optdepends=('libmpdclient: For current MPD status')
makedepends=(git)
source=('git://github.com/gk7/dstatus.git')
md5sum=('SKIP')

pkgver() {
  cd $_gitname
  git log -1 --format="%cd" --date=short | sed 's|-|.|g'
}

build() {
  cd $_gitname
  make
}

package() {
  cd $_gitname
  make DESTDIR="$pkgdir" install
}
