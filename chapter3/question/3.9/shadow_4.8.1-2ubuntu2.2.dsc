-----BEGIN PGP SIGNED MESSAGE-----
Hash: SHA512

Format: 3.0 (quilt)
Source: shadow
Binary: passwd, login, uidmap
Architecture: any
Version: 1:4.8.1-2ubuntu2.2
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Uploaders: Balint Reczey <balint@balintreczey.hu>, Serge Hallyn <serge@hallyn.com>
Homepage: https://github.com/shadow-maint/shadow
Standards-Version: 4.5.1
Vcs-Browser: https://salsa.debian.org/debian/shadow
Vcs-Git: https://salsa.debian.org/debian/shadow -b master
Testsuite: autopkgtest
Build-Depends: debhelper-compat (= 13), gettext, libpam0g-dev, quilt, xsltproc, docbook-xsl, docbook-xml, libxml2-utils, libselinux1-dev [linux-any], libsemanage-dev [linux-any], itstool, bison, libaudit-dev [linux-any]
Package-List:
 login deb admin required arch=any essential=yes
 passwd deb admin required arch=any
 uidmap deb admin optional arch=any
Checksums-Sha1:
 63457a0ba58dc4e81b2663b839dc6c89d3343f12 1611196 shadow_4.8.1.orig.tar.xz
 bfbbb3c3ef5b972d3802a7d17a9e2cbc78330101 98488 shadow_4.8.1-2ubuntu2.2.debian.tar.xz
Checksums-Sha256:
 a3ad4630bdc41372f02a647278a8c3514844295d36eefe68ece6c3a641c1ae62 1611196 shadow_4.8.1.orig.tar.xz
 7ea89214714f06a15925f6fb89de4d96a61ad5fc18bbd2ed210e7c4568f63391 98488 shadow_4.8.1-2ubuntu2.2.debian.tar.xz
Files:
 4b05eff8a427cf50e615bda324b5bc45 1611196 shadow_4.8.1.orig.tar.xz
 8a62e60a6959a467305b21a319a85ac5 98488 shadow_4.8.1-2ubuntu2.2.debian.tar.xz
Original-Maintainer: Shadow package maintainers <pkg-shadow-devel@lists.alioth.debian.org>

-----BEGIN PGP SIGNATURE-----

iQFYBAEBCgBCFiEEGq96SdAIJY1vInRLbzAtCH6LqTYFAmXCUD4kHGNhbWlsYS5j
YW1hcmdvZGVtYXRvc0BjYW5vbmljYWwuY29tAAoJEG8wLQh+i6k2F8IH/2Uda2i1
aKDWxU/wfvhoPEqFE6HveduWWGptGcJQuPgNPUYoVvJ+oknsJx2XgYNcKvyt+6ZO
BrH7fYYSpW8wW85yUuswj/rEVam+hgHvkMUT7uMcoK5EDpzxbskYtrmuf/XuQHuI
F8EIm2mUu51/kdqCbofQlgBoQGW9cyZfXNaIyvkUpS3jCbR1JiO3FF0G50zbo1ry
fxhy+meMnU+ByYzGXicVswD4wLKGpjpofMFUqlWG8amQR3fW5HfMrKvP3674LEk1
5FLR+bVjls5IWR0qM/rx+sTs8felokyWW1ghyIB4unzYpk1TABBPH+gFGJjmF3d/
fgkDkGx1Z51zDas=
=RpTd
-----END PGP SIGNATURE-----
