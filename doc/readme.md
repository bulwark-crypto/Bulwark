Bulwark Core Documentation
=====================

### Tables of Contents

- [Setup](#setup)
- [Running](#running)
	- [Unix](#unix)
	- [Windows](#windows)
	- [OSX](#osx)
	- [Need Help?](#need-help)
	- [Ran into a Bug?](#ran-into-a-bug)
- [Building](#building)
- [Development](#development)
	- [Wallet Information](#wallet-information)
	- [Masternode Information](#masternode-information)
	- [Proof-of-Stake Information](#proof-of-stake-information)
	- [Governance Information](#governance-information)
	- [Transactions](#transactions)
	- [Miscellaneous](#miscellaneous)
- [License](#license)

Setup
---------------------
[Bulwark Core](http://bulwarkcrypto.com.org/#downloads) is the original Bulwark client and it builds the backbone of the network. However, it downloads and stores the entire history of BWK transactions; depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more. Thankfully you only have to do this once.

Running
---------------------
The following are some helpful notes on how to run bwk on your native platform.

### Unix

Unpack the files into a directory and run:

- ./bulwark-qt (GUI) or ./bulwarkd (headless)

### Windows

Unpack the files into a directory, and then run bulwark-qt.exe.

### OSX

Drag Bulwark-Qt to your applications folder, and then run Bulwark-Qt.

### Need Help?

* Ask for help on [Discord](https://discord.me/bulwarkcrypto) or on the [Bulwark Telegram](https://t.me/bulwarkcrypto).
* Join our [Reddit Community](https://www.reddit.com/r/BulwarkCoin/)
* Tweet at [Bulwark-Crypto](https://twitter.com/BulwarkCoin)

### Ran into a Bug?

* Submit an Issue on our [GitHub](https://github.com/bulwark-crypto/Bulwark/issues)

Building
---------------------
The following are developer notes on how to build bwk on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Unix Build Notes](/building/Linux-Build-Guide.md)
- [MacOS Build Notes](/building/MacOS-Build-Guide.md)
- [Windows Build Notes](/building/Windows-Build-Guide.md)
- [NetBSD Build Notes](/building/NetBSD-build-guide.md)
- [OpenBSD Build Notes](/building/OpenBSD-build-guide.md)

Development
---------------------
The Bulwark repo's [root README](https://github.com/bulwark-crypto/Bulwark/blob/master/README.md) contains relevant information our parameters.

- [Developer Notes](/miscellaneous/Developer-Notes.md)
- [Multiwallet Qt Development](/bulwark-core/multiwallet-qt.md)
- [Release Notes](/release-notes/)
- [Unit Tests](/miscellaneous/unit-tests.md)
- [Unauthenticated REST Interface](/bulwark-core/REST-interface.md)
- [Dnsseed Policy](/miscellaneous/dnsseed-policy.md)

### Wallet Information

- [Core Files](/bulwark-core/Bulwark-Core-Files.md)
- [Default Data Locations](/bulwark-core/Default-Data-Locations.md)
- [Running Bulwark](/bulwark-core/Running-Bulwark.md)
- [Init Commands](/bulwark-core/bulwarkd-init.md)
- [API Calls](/bulwark-core/Bulwark-API-Calls.md)
- [Bootstrap](/bulwark-core/Bootstrap.md)
- [Traffic Reduction](/bulwark-core/Reduce-Traffic-in-Bulwark-Core.md)
- [Accounts](/bulwark-core/Accounts-Explained.md)
- [Multisend](/bulwark-core/Multisend-Setup-Guide.md)
- [Data Locations](/bulwark-core/Default-Data-Locations.md)
- [TOR Support](TOR-Support-In-Bulwark.md)

### Masternode Information

- [Masternode Commands](/masternode/Masternode-Commands.md)
- [Installation](/masternode/Masternode-Installation.md)
- [RPC Changes](/masternode/Masternode-RPC-Changes.md)
- [Refresh Guide](/masternode/Refresh-Guide.md)

### Proof-of-Stake Information

- [Staking Guide](/proof-of-stake/Staking-Guide.md)
- [Staking Troubleshooting CLI](/proof-of-stake/Staking-Troubleshooting-CLI.md)
- [Staking Troubleshooting GUI](/proof-of-stake/Staking-Troubleshooting-GUI.md)

### Governance Information

- [Governance](/miscellaneous/Governance.md)

### Transactions

- [ZeroMQ](/transactions/Broadcasting-with-ZeroMQ.md)
- [SwiftTX](/transactions/SwiftTX-Technical-Information.md)

### Miscellaneous

- [Assets Attribution](/miscellaneous/assets-attribution.md)

License
---------------------
Distributed under the [MIT/X11 software license](http://www.opensource.org/licenses/mit-license.php).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.
