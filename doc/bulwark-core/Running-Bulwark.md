# Running Bulwark

There are two variations of the bulwark program available; one with a graphical user interface (usually referred to as just bulwark), and a 'headless' version (called [They are completely compatible with each other, and take the same command-line arguments, read the same configuration file, and read and write the same data files. You can run one copy of either bulwark or bulwarkd on your system at a time (if you accidentally try to launch another, the copy will let you know that bulwark or bulwarkd is already running and will exit).


# Linux Quickstart

The simplest way to start from scratch with the command line client, automatically syncing blockchain and creating a wallet, is to just run this command (without arguments) from the directory containing your bulwarkd binary:

  ./bulwarkd

To run with the standard GUI interface:

  ./bulwark-qt

# Command-line arguments

These commands are accurate as of bulwark Core version '''v2.0.0'''.

<table>
<tbody><tr>
<th>

</th>
<th>
Command
</th>
<th>
Description
</th>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-?
</td>
<td>
Print this help message and exit
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-version
</td>
<td>
Print version and exit
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-alertnotify=&lt;cmd&gt;
</td>
<td>
Execute command when a relevant alert is received or we see a really long fork (%s in cmd is replaced by message)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-alerts
</td>
<td>
Receive and display P2P network alerts (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-blocknotify=&lt;cmd&gt;
</td>
<td>
Execute command when the best block changes (%s in cmd is replaced by block hash)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-checkblocks=&lt;n&gt;
</td>
<td>
How many blocks to check at startup (default: 500, 0 = all)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-conf=&lt;file&gt;
</td>
<td>
Specify configuration file (default: bulwark.conf)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-datadir=&lt;dir&gt;
</td>
<td>
Specify data directory
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-dbcache=&lt;n&gt;
</td>
<td>
Set database cache size in megabytes (4 to 4096, default: 100)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-loadblock=&lt;file&gt;
</td>
<td>
Imports blocks from external blk000??.dat file on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-maxreorg=&lt;n&gt;
</td>
<td>
Set the Maximum reorg depth (default: 100)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-maxorphantx=&lt;n&gt;
</td>
<td>
Keep at most &lt;n&gt; unconnectable transactions in memory (default: 100)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-par=&lt;n&gt;
</td>
<td>
Set the number of script verification threads (-8 to 16, 0 = auto, &lt;0 = leave that many cores free, default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-pid=&lt;file&gt;
</td>
<td>
Specify pid file (default: bulwarkd.pid)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-reindex
</td>
<td>
Rebuild block chain index from current blk000??.dat files on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-reindexaccumulators
</td>
<td>
Reindex the accumulator database on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-reindexmoneysupply
</td>
<td>
Reindex the BWK and zBWK money supply statistics on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-resync
</td>
<td>
Delete blockchain folders and resync from scratch on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-sysperms
</td>
<td>
Create new files with system default permissions, instead of umask 077 (only effective with disabled wallet functionality)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-txindex
</td>
<td>
Maintain a full transaction index, used by the getrawtransaction rpc call (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-forcestart
</td>
<td>
Attempt to force blockchain corruption recovery on startup
</td>
</tr>
<tr><td colspan="3">
<br> <b>Connection options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-addnode=&lt;ip&gt;
</td>
<td>
Add a node to connect to and attempt to keep the connection open
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-banscore=&lt;n&gt;
</td>
<td>
Threshold for disconnecting misbehaving peers (default: 100)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-bantime=&lt;n&gt;
</td>
<td>
Number of seconds to keep misbehaving peers from reconnecting (default: 86400)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-bind=&lt;addr&gt;
</td>
<td>
Bind to given address and always listen on it. Use [host]:port notation for IPv6
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-connect=&lt;ip&gt;
</td>
<td>
Connect only to the specified node(s)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-discover
</td>
<td>
Discover own IP address (default: 1 when listening and no -externalip)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-dns
</td>
<td>
Allow DNS lookups for -addnode, -seednode and -connect (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-dnsseed
</td>
<td>
Query for peer addresses via DNS lookup, if low on addresses (default: 1 unless -connect)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-externalip=&lt;ip&gt;
</td>
<td>
Specify your own public address
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-forcednsseed
</td>
<td>
Always query for peer addresses via DNS lookup (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-listen
</td>
<td>
Accept connections from outside (default: 1 if no -proxy or -connect)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-listenonion
</td>
<td>
Automatically create Tor hidden service (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-maxconnections=&lt;n&gt;
</td>
<td>
Maintain at most &lt;n&gt; connections to peers (default: 125)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-maxreceivebuffer=&lt;n&gt;
</td>
<td>
Maximum per-connection receive buffer, &lt;n&gt;*1000 bytes (default: 5000)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-maxsendbuffer=&lt;n&gt;
</td>
<td>
Maximum per-connection send buffer, &lt;n&gt;*1000 bytes (default: 1000)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-onion=&lt;ip:port&gt;
</td>
<td>
Use separate SOCKS5 proxy to reach peers via Tor hidden services (default: -proxy)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-onlynet=&lt;net&gt;
</td>
<td>
Only connect to nodes in network &lt;net&gt; (ipv4, ipv6 or onion)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-permitbaremultisig
</td>
<td>
Relay non-P2SH multisig (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-peerbloomfilters
</td>
<td>
Support filtering of blocks and transaction with bloom filters (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-port=&lt;port&gt;
</td>
<td>
Listen for connections on &lt;port&gt; (default: 51472 or testnet: 51474)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-proxy=&amp;ltip:port&gt;
</td>
<td>
Connect through SOCKS5 proxy
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-proxyrandomize
</td>
<td>
Randomize credentials for every proxy connection. This enables Tor stream isolation (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-seednode=&lt;ip&gt;
</td>
<td>
Connect to a node to retrieve peer addresses, and disconnect
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-timeout=&lt;n&gt;
</td>
<td>
Specify connection timeout in milliseconds (minimum: 1, default: 5000)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-torcontrol=&lt;ip:port&gt;
</td>
<td>
Tor control port to use if onion listening enabled (default: 127.0.0.1:9051)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-torpassword=&lt;pass&gt;
</td>
<td>
Tor control port password (default: empty)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-upnp
</td>
<td>
Use UPnP to map the listening port (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-whitebind=&lt;addr&gt;
</td>
<td>
Bind to given address and whitelist peers connecting to it. Use [host]:port notation for IPv6
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-whitelist=&lt;netmask&gt;
</td>
<td>
Whitelist peers connecting from the given netmask or IP address. Can be specified multiple times. Whitelisted peers cannot be DoS banned and their transactions are always relayed, even if they are already in the mempool, useful e.g. for a gateway
</td>
</tr>
<tr><td colspan="3">
<br> <b>Wallet options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-createwalletbackups=&lt;n&gt;
</td>
<td>
Number of automatic wallet backups (default: 10)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-disablewallet
</td>
<td>
Do not load the wallet and disable wallet RPC calls
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-keypool=&lt;n&gt;
</td>
<td>
Set key pool size to &lt;n&gt;&lt;/n&gt; (default: 1000)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-paytxfee=&lt;amt&gt;
</td>
<td>
Fee (in BWK/kB) to add to transactions you send (default: 0.0001)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rescan
</td>
<td>
Rescan the block chain for missing wallet transactions on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-salvagewallet
</td>
<td>
Attempt to recover private keys from a corrupt wallet.dat on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-sendfreetransactions
</td>
<td>
Send transactions as zero-fee transactions if possible (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-spendzeroconfchange
</td>
<td>
Spend unconfirmed change when sending transactions (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-disablesystemnotifications
</td>
<td>
Disable OS notifications for incoming transactions (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-txconfirmtarget=&lt;n&gt;
</td>
<td>
If paytxfee is not set, include enough fee so transactions begin confirmation on average within n blocks (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-maxtxfee=&lt;amn&gt;
</td>
<td>
Maximum total fees to use in a single wallet transaction, setting too low may abort large transactions (default: 1.00)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-upgradewallet
</td>
<td>
Upgrade wallet to latest format on startup
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-wallet=&lt;file&gt;
</td>
<td>
Specify wallet file (within data directory) (default: wallet.dat)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-walletnotify=&lt;cmd&gt;
</td>
<td>
Execute command when a wallet transaction changes (%s in cmd is replaced by TxID)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-windowtitle=&lt;name&gt;
</td>
<td>
Wallet window title
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-zapwallettxes=&lt;mode&gt;
</td>
<td>
Delete all wallet transactions and only recover those parts of the blockchain through -rescan on startup (1 = keep tx meta data e.g. account owner and payment request information, 2 = drop tx meta data)
</td>
</tr>
<tr><td colspan="3">
<br> <b>ZeroMQ notification options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-zmqpubhashblock=&lt;address&gt;
</td>
<td>
Enable publish hash block in &lt;address&gt;
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-zmqpubhashtx=&lt;address&gt;
</td>
<td>
Enable publish hash transaction in &lt;address&gt;
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-zmqpubhashtxlock=&lt;address&gt;
</td>
<td>
Enable publish hash transaction (locked via SwiftX) in &lt;address&gt;
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-zmqpubrawblock=&lt;address&gt;
</td>
<td>
Enable publish raw block in &lt;address&gt;
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-zmqpubrawtx=&lt;address&gt;
</td>
<td>
Enable publish raw transaction in &lt;address&gt;
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-zmqpubrawtxlock=&lt;address&gt;
</td>
<td>
Enable publish raw transaction (locked via SwiftX) in &lt;address&gt;
</td>
</tr>
<tr><td colspan="3">
<br> <b>Debugging/Testing options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-debug=&lt;category&gt;
</td>
<td>
Output debugging information (default: 0, supplying &lt;category&gt; is optional). If &lt;category&gt; is not supplied, output all debugging information.&lt;category&gt; can be: addrman, alert, bench, coindb, db, lock, rand, rpc, selectcoins, tor, mempool, net, proxy, bulwark, (obfuscation, swiftx, masternode, mnpayments, mnbudget, zero), qt.
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-gen
</td>
<td>
Generate coins (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-genproclimit
</td>
<td>
Set the number of threads for coin generation if enabled (-1 = all cores, default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-help-debug
</td>
<td>
Show all debugging options (usage: --help -help-debug)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-logips
</td>
<td>
Include IP addresses in debug output (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-logtimestamps
</td>
<td>
Prepend debug output with timestamp (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-minrelaytxfee=&lt;amt&gt;
</td>
<td>
Fees (in BWK/Kb) smaller than this are considered zero fee for relaying (default: 0.0001)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-printtoconsole
</td>
<td>
Send trace/debug info to console instead of debug.log file (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-shrinkdebugfile
</td>
<td>
Shrink debug.log file on client startup (default: 1 when no -debug)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-testnet
</td>
<td>
Use the test network
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-litemode=&lt;n&gt;
</td>
<td>
Disable all Bulwark specific functionality (Masternodes, Zerocoin, SwiftX, Budgeting) (0-1, default: 0)
</td>
</tr>
<tr><td colspan="3">
<br> <b>Staking options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-staking=&lt;n&gt;
</td>
<td>
Enable staking functionality (0-1, default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-reservebalance=&lt;amt&gt;
</td>
<td>
Keep the specified amount available for spending at all times (default: 0)
</td>
</tr>
<tr><td colspan="3">
<br> <b>Masternode options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-masternode=&lt;n&gt;
</td>
<td>
Enable the client to act as a masternode (0-1, default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-mnconf=&lt;file&gt;
</td>
<td>
Specify masternode configuration file (default: masternode.conf)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-mnconflock=&lt;n&gt;
</td>
<td>
Lock masternodes from masternode configuration file (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-masternodeprivkey=&lt;privkey&gt;
</td>
<td>
Set the masternode private key
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-masternodeaddr=&lt;addr&gt;
</td>
<td>
Set external address:port to get to this masternode (example: 128.127.106.235:51472)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-budgetvotemode=&lt;mode&gt;
</td>
<td>
Change automatic finalized budget voting behavior. mode=auto: Vote for only exact finalized budget match to my generated budget. (string, default: auto)
</td>
</tr>
<tr><td colspan="3">
<br> <b>SwiftX options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-enableswifttx=&lt;n&gt;
</td>
<td>
Enable SwiftX, show confirmations for locked transactions (bool, default: true)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-swifttxdepth=&lt;n&gt;
</td>
<td>
Show N confirmations for a successfully locked transaction (0-9999, default: 5)
</td>
</tr>
<tr><td colspan="3">
<br> <b>Node relay options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-datacarrier
</td>
<td>
Relay and mine data carrier transactions (default: 1)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-datacarriersize=&lt;n&gt;
</td>
<td>
Maximum size of data in data carrier transactions we relay and mine (default: 83)
</td>
</tr>
<tr><td colspan="3">
<br> <b>Block creation options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-blockminsize=&lt;n&gt;
</td>
<td>
Set minimum block size in bytes (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-blockmaxsize=&lt;n&gt;
</td>
<td>
Set maximum block size in bytes (default: 750000)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-blockprioritysize=&lt;n&gt;
</td>
<td>
Set maximum size of high-priority/low-fee transactions in bytes (default: 50000)
</td>
</tr>
<tr><td colspan="3">
<br> <b>RPC server options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-server
</td>
<td>
Accept command line and JSON-RPC commands
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rest
</td>
<td>
Accept public REST requests (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rpcbind=&lt;addr&gt;
</td>
<td>
Bind to given address to listen for JSON-RPC connections. Use [host]:port notation for IPv6. This option can be specified multiple times (default: bind to all interfaces)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rpcuser=&lt;user&gt;
</td>
<td>
Username for JSON-RPC connections
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rpcpassword=&lt;pw&gt;
</td>
<td>
Password for JSON-RPC connections
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rpcport=&lt;port&gt;
</td>
<td>
Listen for JSON-RPC connections on &lt;port&gt;&lt;/port&gt; (default: 51473 or testnet: 51475)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rpcallowip=&lt;ip&gt;
</td>
<td>
Allow JSON-RPC connections from specified source. Valid for &lt;ip&gt; are a single IP (e.g. 1.2.3.4), a network/netmask (e.g. 1.2.3.4/255.255.255.0) or a network/CIDR (e.g. 1.2.3.4/24). This option can be specified multiple times
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rpcthreads=&lt;n&gt;
</td>
<td>
Set the number of threads to service RPC calls (default: 4)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rpckeepalive
</td>
<td>
RPC support for HTTP persistent connections (default: 1)
</td>
</tr>
<tr><td colspan="3">
<br> <b>UI Options:</b>
</td></tr>
<tr>
<td>
&nbsp;
</td>
<td>
-choosedatadir
</td>
<td>
Choose data directory on startup (default: 0)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-lang=&lt;lang&gt;
</td>
<td>
Set language, for example "de_DE" (default: system locale)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-min
</td>
<td>
Start minimized
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-rootcertificates=&lt;file&gt;
</td>
<td>
Set SSL root certificates for payment request (default: -system-)
</td>
</tr>
<tr>
<td>
&nbsp;
</td>
<td>
-splash
</td>
<td>
Show splash screen on startup (default: 1)
</td>
</tr>
</tbody></table>

Many of the boolean options can also be set to off by specifying them with a "no" prefix: e.g. -nodnseed.

# bulwark.conf Configuration File

All command-line options (except for -conf) may be specified in a configuration file, and all configuration file options may also be specified on the command line. Command-line options override values set in the configuration file.

The configuration file is a list of setting=value pairs, one per line, with optional comments starting with the '#' character.

An empty configuration file is automatically created on first run. By default, bulwark (or bulwarkd) will look for a file named 'bulwark.conf' in the bulwark [directory]([data)], but both the data directory and the configuration file path may be changed using the -datadir and -conf command-line arguments.

<table>
<tbody><tr>
<th>
Operating System
</th>
<th>
Default Bulwark datadir
</th>
<th>
Typical path to configuration file
</th>
</tr>
<tr>
<td>
Windows
</td>
<td>
%APPDATA%\Bulwark\
</td>
<td>
C:\Users\username\AppData\Roaming\Bulwark\bulwark.conf
</td>
</tr>
<tr>
<td>
Linux
</td>
<td>
$HOME/.bulwark/
</td>
<td>
/home/username/.bulwark/bulwark.conf
</td>
</tr>
<tr>
<td>
Mac OSX
</td>
<td>
$HOME/Library/Application Support/Bulwark/
</td>
<td>
/Users/username/Library/Application Support/Bulwark/bulwark.conf
</td>
</tr>
</tbody></table>

Note: if running bulwark in testnet mode, the sub-folder "testnet4" will be appended to the data directory automatically.

# Sample bulwark.conf

See [https://github.com/bulwark-crypto/bulwark/blob/master/contrib/debian/examples/bulwark.conf]() for an up-to-date sample.

# Platforms

## Windows

### Start automatically


To configure the bulwark client to start automatically:

You might use the configuration-file, or the GUI-Settings:

Settings -> Options

then mark the checkbox titled:
 [Start bulwark on system startup



### Batch automation


To work with batch, you have to start the daemon (bulwarkd.exe). The bulwarkd.exe run with option "-server" will respond with GUI-messages you are not able to process its answers.
