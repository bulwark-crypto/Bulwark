# macOS Build Guide

**BEFORE YOU START:** Please back up your wallet.dat file. While we're pretty sure that this will in no way affect your wallet, it's always better to have a backup, and we're not responsible for any loss of Bulwark or sanity on your part.

## How to compile Bulwark

First off, you need to install [Homebrew](https://brew.sh/index_de.html) - we're also gonna use a shell, by default this is in /Applications/Utilities/Terminal.app - everything you do here will run and then return you back to your input prompt - **don't start a new command before the previous one has finished!**

**Note:** If you already have homebrew installed, make sure you uninstall boost first! We need version 1.57 to compile the wallet. If you don't know what that means, just ignore this sentence and continue on. :)

After you install XCode, start a shell and install Homebrew:

```/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"```

Homebrew is like an App Store for your command line, and allows us to install various Unix tools that we're going to need. Just follow the instructions on the screen and you'll be good. Once you're done, we need to install some stuff:

```brew install autoconf automake berkeley-db@4 boost@1.57 git libevent libtool miniupnpc openssl pkg-config protobuf qt zeromq librsvg```

We need a specific version of boost to build the current Bulwark wallet, and now that it's installed, we need to link it:

```brew link boost@1.57 --force```

Now we're going to switch into your Downloads folder:

```cd ~/Downloads```

The next step is to download the current version of the wallet from Github and go into that directory:

```git clone https://github.com/bulwark-crypto/Bulwark.git && cd Bulwark```

We're in the Bulwark directory now, before we can build our wallet, we need to set some configuration flags:

```export LDFLAGS=-L/usr/local/opt/openssl/lib;export CPPFLAGS=-I/usr/local/opt/openssl/include```

Then we autogen some additonal files...

```./autogen.sh```

...configure everything (*note the dot and slash at the beginning of the line!*)...

```./configure```

...and finally build our wallet (this is gonna take a while):

```make```

If you want to use Bulwark as a regular macOS app, continue with "How to get a Bulwark-QT App". If, for whatever reason, you prefer to use the command line tools, continue with "How to use the command line tools".

## How to get a Bulwark-QT App

After `make` is finished, you can create an App bundle inside a disk image with:

```make deploy```

Once this is done, you'll find `Bulwark-Qt.dmg` inside your Bulwark folder. Open and install as usual.

## How to use the command line tools

Once the build is complete, switch into the *src/qt* subdirectory:

```cd src/qt```

And there you have your wallet - you can start it by running:

```./bulwark-qt```

If you want to start your wallet in the future, open Terminal.app and run this command:

```~/Downloads/Bulwark/src/qt/bulwark-qt```

## FAQ

##### **Q:** I get the error "Assertion failed" ?

**A:** You need to start your wallet with -forcestart: `./bulwark-qt -forcestart`
