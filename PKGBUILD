pkgname=dstatus
pkgver=20130328
pkgrel=1
pkgdesc='Statusinfo for DWM written in C'
arch=(any)
groups=(custom)
url='https://github.com/gk7/dstatus'
license=(custom)
depends=(libx11 alsa-lib)
optdepends=('libmpdclient: to have mpd current song in the statusbar')
makedepends=(git)

_gitroot="https://github.com/gk7/dstatus"
_gitname="dstatus"

build() {
	cd "$srcdir"/
	msg "Connecting to GIT server...."

	if [ -d $_gitname ] ; then
		cd $_gitname && git pull origin
		msg "The local files are updated."
	else
		git clone --depth=1 $_gitroot $_gitname
		cd $_gitname
	fi
	msg "GIT checkout done or server timeout"

	cd ${srcdir}/${_gitname}

	make
}

package() {
	cd ${srcdir}/${_gitname}
	make DESTDIR="$pkgdir" install
}
