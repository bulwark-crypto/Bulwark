
Debian
====================
This directory contains files used to package bulwarkd/bulwark-qt
for Debian-based Linux systems. If you compile bulwarkd/bulwark-qt yourself, there are some useful files here.

## bulwark: URI support ##


bulwark-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install bulwark-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your bulwarkqt binary to `/usr/bin`
and the `../../share/pixmaps/bulwark128.png` to `/usr/share/pixmaps`

bulwark-qt.protocol (KDE)

