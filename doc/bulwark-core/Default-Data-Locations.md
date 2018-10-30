# Default Data Locations

The data directory is the location where Bulwark's data files are stored, including the wallet data file.

## Windows

Go to Start -> Run (or press WinKey+R) and run this:

 %APPDATA%\bulwark

Bulwark's data folder will open. For most users, this is the following locations:

C:\Documents and Settings\YourUserName\Application data\bulwark (XP)

 C:\Users\YourUserName\Appdata\Roaming\bulwark (Vista and 7)

"AppData" and "Application data" are hidden by default.

You can also store bulwark data files in any other drive or folder.

If you have already downloaded the data then you will have to move the data to the new folder.
If you want to store them in D:\bulwarkData then click on "Properties" of a shortcut to bulwark-qt.exe and
add -datadir=D:\bulwarkData at the end as an example:

 "C:\Program Files (x86)\bulwark\bulwark-qt.exe" -datadir=d:\bulwarkData

Start bulwark, now you will see all the files are created in the new data directory.

## Linux

By default bulwark will put its data here:

 ~/.bulwark/

You need to do a "ls -a" to see directories that start with a dot.

If that's not it, you can do a search like this:

 find / -name wallet.dat -print 2>/dev/null

## Mac

By default bulwark will put its data here:

 ~/Library/Application Support/bulwark/

# Directory Contents

## Files

An overview of these is in [https://github.com/bulwark-crypto/bulwark/blob/master/doc/files.md]([Wallet|wallet]]) in the bulwark Core documentation.

* .lock
    * bulwark data directory lock file
* bulwark.conf [    *Contains [[Running-bulwark#bulwarkconf_Configuration_File|configuration options](optional])].
* __db.''xxx''
    *Used by BDB
* db.log
* debug.log
    *bulwark's verbose log file. Automatically trimmed from time to time.
* wallet.dat
    *Storage for keys, transactions, metadata, and options. <span style="color:red">Please be sure to make backups of this file.  It contains the keys necessary for spending your BWK.</span>
* peers.dat
    * Storage for peer information to make a reconnect easier.  This file uses a bitcoin-standard file format, unrelated to any database system<ref>[Ultraprune merged in mainline](http://bitcointalk.org/index.php?topic=119525.msg1287284#msg1287284)</ref>.
* fee_estimates.dat
    * Statistics used to estimate fees and priorities. Saved just before program shutdown, and read in at startup.
* budget.dat
    * Stores data for budget objects
* masternode.conf
    * Contains configuration settings for remote masternodes
* mncache.dat
    * Stores data for masternode list
* mnpayments.dat
    * Stores data for masternode payments
The data, index and log files are used by Oracle [Berkeley DB](http://en.wikipedia.org/wiki/Berkeley_DB), the embedded key/value data store that bulwark uses.

## database subdirectory
Contains BDB journaling files

## testnet4 subdirectory
Contains testnet versions of these files (if running with -testnet)

## blocks subdirectory
Contains blockchain data.

* blk*.dat
    * Stored are actual Bitcoin blocks, in network format, dumped to disk raw.  They are only needed for re-scanning missing transactions in a wallet, reorganizing to a different part of the chain, and serving the block data to other nodes that are synchronizing.

* blocks/index subdirectory
    * A LevelDB database that contains metadata about all known blocks, and where to find them on disk. Without this, finding a block would be very slow.

## chainstate subdirectory
A LevelDB database with a compact representation of all currently unspent transaction outputs and some metadata about the transactions they are from. The data here is necessary for validating new incoming blocks and transactions. It can theoretically be rebuilt from the block data (see the -reindex command line option), but this takes a rather long time. Without it, you could still theoretically do validation indeed, but it would mean a full scan through the blocks (150 GB as of Nov 2017) for every output being spent.

## locks subdirectory
Contains "undo" data.

* rev*.dat
You can see blocks as 'patches' to the chain state (they consume some unspent outputs, and produce new ones), and see the undo data as reverse patches. They are necessary for rolling back the chainstate, which is necessary in case of reorganizations.

## sporks subdirectory
A LevelDB database containing information pertaining to the various sporks and their current values. This data is used during and after syncing as part of the network's consensus rules.

## Personally identifiable data
This section may be of use to you if you wish to send a friend the blockchain, avoiding them a hefty download.

*wallet.dat
    *Contains addresses and transactions linked to them. <span style="color:red">Please be sure to make backups of this file.  It contains the keys necessary for spending your BWK.</span> You should not transfer this file to any third party or they may be able to access your BWK.
*db.log
    *May contain information pertaining to your wallet. It may be safely deleted.
*debug.log
    *May contain IP addresses and transaction ID's. It may be safely deleted.
*database/ folder
    *This should only exist when bulwark-qt is currently running. It contains information (BDB state) relating to your wallet.
*peers.dat
    *Unknown whether this contains personally identifiable data. It may be safely deleted.

Other files and folders (blocks, blocks/index, chainstate, sporks, zerocoin) may be safely transferred/archived as they contain information pertaining only to the public blockchain.

# Transferability

The database files in the "blocks", "chainstate", "sporks", and "zerocoin" directories are cross-platform, and can be copied between different installations. These files, known collectively as a node's "block database", represent all of the information downloaded by a node during the syncing process. In other words, if you copy installation A's block database into installation B, installation B will then have the same syncing percentage as installation A. This is usually ''far'' faster than doing the normal initial sync over again. However, when you copy someone's database in this way, you are trusting them '''absolutely'''. bulwark Core treats its block database files as 100% accurate and trustworthy, whereas during the normal initial sync it treats each block offered by a peer as invalid until proven otherwise. If an attacker is able to modify your block database files, then they can do all sorts of evil things which could cause you to lose BWK. Therefore, you should only copy block databases from bulwark installations under your personal control, and only over a secure connection.

Each node has a unique block database, and all of the files are highly connected. So if you copy just a few files from one installation's "blocks", "chainstate", "sporks", or "zerocoin" directories into another installation, this will almost certainly cause the second node to crash or get stuck at some random point in the future. If you want to copy a block database from one installation to another, you have to delete the old database and copy ''all'' of the files at once. Both nodes have to be shut down while copying.

Only the file with the highest number in the "blocks" directory is ever written to. The earlier files will never change. Also, when these blk*.dat files are accessed, they are usually accessed in a highly sequential manner. Therefore, it's possible to symlink the "blocks" directory or some subset of the blk*.dat files individually onto a magnetic storage drive without much loss in performance, and if two installations start out with identical block databases (due to the copying described previously), subsequent runs of rsync will be very efficient.

# See Also

* [What is the database for?](http://bitcoin.stackexchange.com/a/11108/153) Question on Bitcoin Stack Exchange
