# WINDOWS BUILD NOTES

## Installing Windows Subsystem for Linux

With Windows 10, Microsoft has released a new feature named the [Windows
Subsystem for Linux (WSL)](https://msdn.microsoft.com/commandline/wsl/about). This
feature allows you to run a Bash shell directly on Windows in an Ubuntu-based
environment. Within this environment, you can cross compile for Windows without
the need for a separate Linux VM or server. Note that while WSL can be installed with
other Linux variants, such as OpenSUSE, the following instructions have only been
tested on Ubuntu.

This feature is not supported in versions of Windows prior to Windows 10, or on
Windows Server SKUs. In addition, it is available [only for 64-bit versions of
Windows](https://msdn.microsoft.com/en-us/commandline/wsl/install_guide).

Full instructions to install WSL are available on the above link.
After the Bash shell is active, you can follow the instructions below.

## Cross-compilation for Ubuntu and Windows Subsystem for Linux

At the time of writing, the Windows Subsystem for Linux installs Ubuntu Xenial 16.04.
The Mingw-w64 package for Ubuntu Xenial does not produce working executables for some
of the Bulwark Core applications. It is possible to build on Ubuntu Xenial by installing
the cross compiler packages from Ubuntu Zesty, see the steps below.

First, install the dependencies, and select the appropriate compiler:

~~~~
sudo apt update
sudo apt dist-upgrade
sudo apt install build-essential libtool autotools-dev automake pkg-config bsdmainutils curl git software-properties-common g++-mingw-w64-x86-64 xutils-dev
sudo add-apt-repository "deb http://old-releases.ubuntu.com/ubuntu zesty universe"
sudo apt update && sudo apt dist-upgrade
echo 1 | sudo update-alternatives --config x86_64-w64-mingw32-g++
~~~~

Note that for WSL the Bulwark Core source path **must** be somewhere in the default mount
file system, for example `/home/foo/bulwark`, **and** not under `/mnt/d`. If this is not the case,
the dependency autoconf scripts will fail. This means you cannot use a directory that is
located directly on the host Windows file system to perform the build.

Acquire the source in the usual way:

~~~~
git clone https://github.com/bulwark-crypto/Bulwark
~~~~

Once the source code is ready, the build steps are below (the `-j4` flag corresponds
to the number of CPU cores you have, adjust accordingly).

~~~~
PATH=$(echo "$PATH" | sed -e 's/:\/mnt.*//g')
cd depends
make HOST=x86_64-w64-mingw32 -j4
cd ..
./autogen.sh
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --prefix=/
make -j4
~~~~

## Depends system

For further documentation on the depends system, see [README.md](../depends/README.md)
in the depends directory.

## Installation

After building using the Windows subsystem, it can be useful to copy the compiled
executables to a directory on the Windows drive in the same directory structure
as they appear in the release `.zip` archive. This can be done in the following
way. This will install to `c:\workspace\bulwark`, for example:

~~~~
make install DESTDIR=/mnt/c/workspace/bulwark
~~~~
