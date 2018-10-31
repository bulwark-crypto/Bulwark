# Bulwark API call list (as of version 2.0.0)

Common operations
-----------------

### Listing my Bulwark addresses

Listing the Bulwark addresses in your wallet is easily done via
*listreceivedbyaddress*. It normally lists only addresses which already
have received transactions, however you can list all the addresses by
setting the first argument to 0, and the second one to true.

[Accounts](Accounts-Explained.md) are used to organize addresses.

Full list
---------

Required arguments are denoted inside < and > Optional arguments are
inside [ and ].


| Command | Parameters | Description | Requires unlocked wallet? |
| --- | --- | --- | --- |
| Blockchain |
| `getbestblockhash` |   | Returns the hash of the best (tip) block in the longest block chain. | N |
| `getblock` |  [verbose] | Returns information about the block with the given hash. | N |
| `getblockchaininfo` |   | Returns an object containing various state info regarding block chain processing. | N |
| `getblockcount` |   | Returns the number of blocks in the longest block chain. | N |
| `getblockhash` |  | Returns hash of block in best-block-chain at index provided. | N |
| `getblockheader` |  [verbose] | If verbose is false, returns a string that is serialized, hex-encoded data for block 'hash' header. If verbose is true, returns an Object with information about block  header. | N |
| `getchaintips` |   | Return information about all known tips in the block tree, including the main chain as well as orphaned branches. | N |
| `getdifficulty` |   | Returns the proof-of-work difficulty as a multiple of the minimum difficulty. | N |
| `getmempoolinfo` |   | Returns details on the active state of the TX memory pool. | N |
| `getrawmempool` | [verbose] | Returns all transaction ids in memory pool as a json array of string transaction ids. | N |
| `gettxout` |   [includemempool=true] | Returns details about an unspent transaction output. | N |
| `gettxoutsetinfo` |   | Returns statistics about the unspent transaction output set. | N |
| `invalidateblock` |  | Permanently marks a block as invalid, as if it violated a consensus rule. | N |
| `reconsiderblock` |  | Removes invalidity status of a block and its descendants, reconsider them for activation. This can be used to undo the effects of invalidateblock. | N |
| `verifychain` | [numblocks=288] | Verifies blockchain database. | N |
| Control |
| `getinfo` |   | Returns an object containing various state info. | N |
| `help` | [command] | List all commands, or get help for a specified command. | N |
| `stop` |   | Stop bwk server. | N |
| Generating |
| `getgenerate` |   | **PoW Only** Return if the server is set to generate coins or not. The default is false. It is set with the command line argument -gen (or bwk.conf setting gen) It can also be set with the setgenerate call. | N |
| `setgenerate` |  [genproclimit] | **PoW Only** Set 'generate' true or false to turn generation on or off. Generation is limited to 'genproclimit' processors, -1 is unlimited. See the getgenerate call for the current setting. | N |
| Mining |
| `getblocktemplate` | [jsonrequestobject] | **PoW Only** Returns data needed to construct a block to work on. | N |
| `getmininginfo` |   | **PoW Only** Returns a json object containing mining-related information. | N |
| `getnetworkhashps` | [blocks] [height] | **PoW Only** Returns the estimated network hashes per second based on the last n blocks. | N |
| `prioritisetransaction` |    | Accepts the transaction into mined blocks at a higher (or lower) priority | N |
| `reservebalance` | [reserve] [amount] | Show or set the reserve amount not participating in network protection. If no parameters provided current setting is printed. | Y |
| `submitblock` |  [jsonparamatersobject] | **PoW Only** Attempts to submit new block to network. | N |
| Network |
| `addnode` |   | Attempts add or remove a node from the addnode list. Or try a connection to a node once. | N |
| `clearbanned` |   | **version 3.1.0** Clear all banned IPs. | N |
| `disconnectnode` |  | **version 3.1.0** Immediately disconnects from the specified node. | N |
| `getaddednodeinfo` |  [node] | Returns information about the given added node, or all added nodes. (note that onetry addnodes are not listed here) If dns is false, only a list of added nodes will be provided, otherwise connected information will also be available. | N |
| `getconnectioncount` |   | Returns the number of connections to other nodes. | N |
| `getnettotals` |   | Returns information about network traffic, including bytes in, bytes out, and current time. | N |
| `getnetworkinfo` |   | Returns an object containing various state info regarding P2P networking. | N |
| `getpeerinfo` |   | Returns data about each connected network node as a json array of objects. | N |
| `listbanned` |   | **version 3.1.0** List all banned IPs/Subnets. | N |
| `ping` |   | Requests that a ping be sent to all other nodes, to measure ping time. | N |
| `setban` |   [bantime] [absolute] | **version 3.1.0** Attempts add or remove a IP/Subnet from the banned list. | N |
| Bulwark |
| `checkbudgets` |   | Initiates a buddget check cycle manually. | N |
| `createmasternodekey` |   | Create a new masternode private key. | N |
| `getbudgetinfo` | [proposal] | Show current masternode budgets with optional filter by proposal name. | N |
| `getbudgetprojection` |   | Show the projection of which proposals will be paid the next cycle. | N |
| `getbudgetvotes` |  | Print vote information for a budget proposal. | N |
| `getmasternodecount` |   | Get masternode count values. | N |
| `getmasternodeoutputs` |   | Print all masternode transaction outputs. | N |
| `getmasternodescores` | [blocks=10] | Print list of winning masternode by score. | N |
| `getmasternodestatus` |   | Print masternode status. | N |
| `getmasternodewinners` | [blocks=10] [filter] | Print the masternode winners for the last _n_ blocks | N |
| `getnextsuperblock` |   | Print the next super block height | N |
| `getpoolinfo` |   | **Depreciated in version 3.0.0** Returns anonymous pool-related information | N |
| `listmasternodeconf` | [filter] | Print masternode.conf in JSON format. | N |
| `listmasternodes` | [filter] | Get a ranked list of masternodes. Optional filter by txhash, status, or payment address. | N |
| `masternode` |  | **Depreciated in version 2.3.1** Set of commands to execute masternode related actions. | Y/N |
| `masternodeconnect` |  | Attempts to connect to specified masternode address. | N |
| `masternodecurrent` |   | Get current masternode winner. | N |
| `masternodedebug` |   | Print masternode status. | N |
| `mnbudget` |  [passphrase] | **Depreciated in version 2.3.1** Vote or show current budgets. | N |
| `mnbudgetrawvote` |       | Compile and relay a proposal vote with provided external signature instead of signing vote internally. | N |
| `mnbudgetvote` |    [alias] | Vote on a budget proposal. | N |
| `mnfinalbudget` |  [passphrase] | Vote or show current budgets. | Y/N |
| `mnsync` |  | Returns the sync status or resets sync. | N |
| `preparebudget` |       | Prepare proposal for network by signing and creating tx. | Y |
| `spork` |  | Print raw value or active status of sporks. | N |
| `startmasternode` |   [alias] | Attempts to start one or more masternode(s). | Y/N |
| `submitbudget` |        | Submit proposal to the network. | N |
| Raw Transactions |
| `createrawtransaction` | [{"txid":txid,"vout":n},...] {address:amount,...} | Creates a raw transaction spending given inputs. | N |
| `decoderawtransaction` |  | Produces a human-readable JSON object for a raw transaction. | N |
| `decodescript` |  | Decode a hex-encoded script. | N |
| `getrawtransaction` |  [verbose=0] | Returns raw transaction representation for given transaction id. | N |
| `sendrawtransaction` |  [allowhighfees=false] [swiftx=false] | Submits raw transaction (serialized, hex-encoded) to local node and network. | N |
| `signrawtransaction` | ,...] [sighashtype=ALL] | Adds signatures to a raw transaction and returns the resulting raw transaction. | Y/N |
| Utility |
| `createmultisig` |  <'["key",...]'> | Creates a multi-signature address with n signature of m keys required. | N |
| `estimatefee` |  | Estimates the approximate fee per kilobyte needed for a transaction to begin confirmation within nblocks blocks. | N |
| `estimatepriority` |  | Estimates the approximate priority a zero-fee transaction needs to begin confirmation within nblocks blocks. | N |
| `validateaddress` |  | Return information about the given bwk address. | N |
| `verifymessage` |    | Verify a signed message. | N |
| Wallet |
| `addmultisigaddress` |  <'["key",...]'> [account] | Add a nrequired-to-sign multisignature address to the wallet. Each key is a bwk address or hex-encoded public key. If 'account' is specified, assign address to that account. | Y/N |
| `autocombinerewards` |  [threshold] | Wallet will automatically monitor for any coins with value below the threshold amount, and combine them if they reside with the same bwk address. | Y |
| `backupwallet` |  | Safely copies wallet.dat to destination, which can be a directory or a path with filename. | N |
| `bip38decrypt` |   | Decrypts and then imports password protected private key. | Y |
| `bip38encrypt` |   | Encrypts a private key corresponding to 'bwkaddress'. | Y |
| `dumpprivkey` |  | Reveals the private key corresponding to 'bwkaddress'. | Y |
| `dumpwallet` |  | Dumps all wallet keys in a human-readable format. | Y |
| `encryptwallet` |  | Encrypts the wallet with . | N |
| `getaccount` |  | Returns the account associated with the given address. | N |
| `getaccountaddress` |  | Returns the current bitcoin address for receiving payments to this account. If  does not exist, it will be created along with an associated new address that will be returned. | N |
| `getaddressesbyaccount` |  | Returns the list of addresses for the given account. | N |
| `getbalance` | [account] [minconf=1] [includeWatchonly=false] | If [account] is not specified, returns the server's total available balance.  
If [account] is specified, returns the balance in the account. | N |
| `getnewaddress` | [account] | Returns a new bwk address for receiving payments. If [account] is specified payments received with the address will be credited to [account]. | Y |
| `getrawchangeaddress` |   | Returns a new bwk address, for receiving change. This is for use with raw transactions, NOT normal use. | N |
| `getreceivedbyaccount` | [account] [minconf=1] | Returns the total amount received by addresses with [account] in transactions with at least [minconf] confirmations. If [account] not provided return will include all transactions to all accounts. | N |
| `getreceivedbyaddress` |  [minconf=1] | Returns the amount received by  in transactions with at least [minconf] confirmations. It correctly handles the case where someone has sent to the address in multiple transactions. Keep in mind that addresses are only ever used for receiving transactions. Works only for addresses in the local wallet, external addresses will always show 0. | N |
| `getstakesplitthreshold` |   | Returns the threshold for stake splitting. | N |
| `getstakingstatus` |   | Returns an object containing various staking information. | N |
| `gettransaction` |  [includeWatchonly] | Get detailed information about in-wallet transaction . | N |
| `getunconfirmedbalance` |   | Returns the server's total unconfirmed balance . | N |
| `getwalletinfo` |   | Returns an object containing various wallet state info. | N |
| `importaddress` |  [label] [rescan=true] | Adds an address or script (in hex) that can be watched as if it were in your wallet but cannot be used to spend. | Y |
| `importprivkey` |  [label] [rescan=true] | Adds a private key (as returned by dumpprivkey) to your wallet. | Y |
| `importwallet` |  | Imports keys from a wallet dump file (see dumpwallet). | Y |
| `keypoolrefill` |  | Fills the keypool. | Y |
| `listaccounts` | [minconf] [includeWatchonly=false] | Returns Object that has account names as keys, account balances as values. | N |
| `listaddressgroupings` |   | Returns all addresses in the wallet and info used for coincontrol. | N |
| `listlockunspent` |   | Returns list of temporarily unspendable outputs. | N |
| `listreceivedbyaccount` | [minconf=1] [includeempty=false] [includeWatchonly=false] | List balances by account. | N |
| `listreceivedbyaddress` | [minconf=1] [includeempty=false] [includeWatchonly=false] | List balances by receiving address. | N |
| `listsinceblock` | [blockhash] [target-confirmations] [includeWatchonly=false] | Get all transactions in blocks since block [blockhash], or all transactions if omitted. | N |
| `listtransactions` | [account] [count=10] [from=0] [includeWatchonly=false] | Returns up to [count] most recent transactions skipping the first [from] transactions for account [account]. If [account] not provided it'll return recent transactions from all accounts. | N |
| `listunspent` | [minconf=1] [maxconf=999999] ['["addresses",...]'] | Returns array of unspent transaction outputs with between minconf and maxconf (inclusive) confirmations. Optionally filter to only include txouts paid to specified addresses. | N |
| `lockunspent` |  <'[{"txid":"txid","vout":n},...]'> | Updates list of temporarily unspendable outputs. | Y |
| `move` |    [minconf=1] [comment] | Move from one account in your wallet to another | N |
| `sendfrom` |    [minconf=1] [comment] [comment-to] |  is a real and is rounded to 8 decimal places. Will send the given amount to the given address, ensuring the account has a valid balance using [minconf] confirmations. Returns the transaction ID if successful (not in JSON object). | Y |
| `sendmany` |  {address:amount,...} [minconf=1] [comment] | Send multiple times. Amounts are double-precision floating point numbers | Y |
| `sendtoaddress` |   [comment] [comment-to] | Send an amount to a given address.  is a real and is rounded to 8 decimal places. Returns the transaction ID  if successful. | Y |
| `sendtoaddressix` |   [comment] [comment-to] | Send an amount to a given address using SwiftX.  is a real and is rounded to 8 decimal places. Returns the transaction ID  if successful. | Y |
| `setaccount` |   | Sets the account associated with the given address. Assigning address that is already assigned to the same account will create a new address associated with that account. | N |
| `setstakesplitthreshold` |  | This will set the output size of your stakes to never be below the given value. | Y |
| `settxfee` |  | Set the transaction fee per kB. | N |
| `signmessage` |   | Sign a message with the private key of an address. | Y |
| `walletlock` |   | Removes the wallet encryption key from memory, locking the wallet. | Y |
| `walletpassphrase` |   [anonymizeonly=false] | Stores the wallet decryption key in memory for 'timeout' seconds. | N |
| `walletpassphrasechange` |   | Changes the wallet passphrase from 'oldpassphrase' to 'newpassphrase'. | N |
| Zerocoin |
| `exportzerocoins` |  [denomination] | Exports zerocoin mints that are held by this wallet.dat | Y |
| `getarchivedzerocoin` |   | Display zerocoins that were archived because they were believed to be orphans. | Y |
| `getspentzerocoinamount` |   | Returns value of spent zerocoin output designated by transaction hash and input index. | N |
| `getzerocoinbalance` |   | Returns the balance of all unspent zerocoins in this wallet. | Y |
| `importzerocoins` |  | Import zerocoin mints. Adds raw zerocoin mints to the wallet.dat. Note it is recommended to use the json export created from the exportzerocoins RPC call | Y |
| `listmintedzerocoins` |   | Return an array of zerocoin mints. | Y |
| `listspentzerocoins` |   | Return an array of spent zerocoins. | Y |
| `listzerocoinamounts` |   | List the denomination amounts held in this wallet. | Y |
| `mintzerocoin` |  | Mint given amount from PIV to zBWK. | Y |
| `reconsiderzerocoins` |   | Check archived zBWK list to see if any mints were added to the blockchain.. | Y |
| `resetmintzerocoin` | [extended_search] | Scan the blockchain for all of the zerocoins that are held in the wallet.dat. Update any meta-data that is incorrect. | Y |
| `resetspentzerocoin` |   | Scan the blockchain for all of the zerocoins that are held in the wallet.dat. Reset mints that are considered spent that did not make it into the blockchain. | Y |
| `spendzerocoin` |      | Spend zBWK to given address with optional reminting of change. | Y |
