# Bulwark API Calls

## Common operations 

## Listing my Bulwark addresses

Listing the Bulwark in your wallet is easily done via

''listreceivedbyaddress''.

It normally lists only addresses which already have received transactions, however you can list all the addresses by setting the first argument to 0, and the second one to true.

# Full list

Required arguments are denoted inside &lt; and &gt; Optional arguments are inside [and] ().

<table>
<tbody><tr>
<th>
Command
</th>
<th>
Parameters
</th>
<th>
Description
</th>
<th>
Requires unlocked wallet?
</th>
</tr>
<tr><th colspan="4">
Blockchain
</th></tr>
<tr>
<td>
<code>getbestblockhash</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns the hash of the best (tip) block in the longest block chain.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getblock</code>
</td>
<td>
&lt;hash&gt; [verbose]
</td>
<td>
Returns information about the block with the given hash.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getblockchaininfo</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns an object containing various state info regarding block chain processing.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getblockcount</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns the number of blocks in the longest block chain.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getblockhash</code>
</td>
<td>
&lt;index&gt;
</td>
<td>
Returns hash of block in best-block-chain at index provided.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getblockheader</code>
</td>
<td>
&lt;hash&gt; [verbose]
</td>
<td>
If verbose is false, returns a string that is serialized, hex-encoded data for block 'hash' header. If verbose is true, returns an Object with information about block &lt;hash&gt; header.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getchaintips</code>
</td>
<td>
&nbsp;
</td>
<td>
Return information about all known tips in the block tree, including the main chain as well as orphaned branches.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getdifficulty</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns the proof-of-work difficulty as a multiple of the minimum difficulty.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getmempoolinfo</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns details on the active state of the TX memory pool.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getrawmempool</code>
</td>
<td>
[verbose]
</td>
<td>
Returns all transaction ids in memory pool as a json array of string transaction ids.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>gettxout</code>
</td>
<td>
&lt;txid&gt; &lt;n&gt; [includemempool=true]
</td>
<td>
Returns details about an unspent transaction output.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>gettxoutsetinfo</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns statistics about the unspent transaction output set.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>invalidateblock</code>
</td>
<td>
&lt;hash&gt;
</td>
<td>
Permanently marks a block as invalid, as if it violated a consensus rule.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>reconsiderblock</code>
</td>
<td>
&lt;hash&gt;
</td>
<td>
Removes invalidity status of a block and its descendants, reconsider them for activation.
This can be used to undo the effects of invalidateblock.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>verifychain</code>
</td>
<td>
[numblocks=288]
</td>
<td>
Verifies blockchain database.
</td>
<td>
N
</td>
</tr>
<tr><th colspan="4">
Control
</th></tr>
<tr>
<td>
<code>getinfo</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns an object containing various state info.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>help</code>
</td>
<td>
[command]
</td>
<td>
List all commands, or get help for a specified command.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>stop</code>
</td>
<td>
&nbsp;
</td>
<td>
Stop bulwark server.
</td>
<td>
N
</td>
</tr>
<tr><th colspan="4">
Generating
</th></tr>
<tr>
<td>
<code>getgenerate</code>
</td>
<td>
&nbsp;
</td>
<td>
<b>PoW Only</b> Return if the server is set to generate coins or not. The default is false. It is set with the command line argument -gen (or bulwark.conf setting gen) It can also be set with the setgenerate call.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>setgenerate</code>
</td>
<td>
&lt;generate&gt; [genproclimit]
</td>
<td>
<b>PoW Only</b> Set 'generate' true or false to turn generation on or off. Generation is limited to 'genproclimit' processors, -1 is unlimited. See the getgenerate call for the current setting.
</td>
<td>
N
</td>
</tr>
<tr><th colspan="4">
Mining
</th></tr>
<tr>
<td>
<code>getblocktemplate</code>
</td>
<td>
[jsonrequestobject]
</td>
<td>
<b>PoW Only</b> Returns data needed to construct a block to work on.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getmininginfo</code>
</td>
<td>
&nbsp;
</td>
<td>
<b>PoW Only</b> Returns a json object containing mining-related information.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getnetworkhashps</code>
</td>
<td>
[blocks] [height]
</td>
<td>
<b>PoW Only</b> Returns the estimated network hashes per second based on the last n blocks.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>prioritisetransaction</code>
</td>
<td>
&lt;txid&gt; &lt;priority delta&gt; &lt;fee delta&gt;
</td>
<td>
Accepts the transaction into mined blocks at a higher (or lower) priority
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>reservebalance</code>
</td>
<td>
[reserve] [amount]
</td>
<td>
Show or set the reserve amount not participating in network protection. If no parameters provided current setting is printed.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>submitblock</code>
</td>
<td>
&lt;hexdata&gt; [jsonparamatersobject]
</td>
<td>
<b>PoW Only</b> Attempts to submit new block to network.
</td>
<td>
N
</td>
</tr>
<tr><th colspan="4">
Network
</th></tr>
<tr>
<td>
<code>addnode</code>
</td>
<td>
&lt;node&gt; &lt;add|remove|onetry&gt;
</td>
<td>
Attempts add or remove a node from the addnode list. Or try a connection to a node once.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>clearbanned</code>
</td>
<td>
&nbsp;
</td>
<td>
<b>version 3.1.0</b> Clear all banned IPs.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>disconnectnode</code>
</td>
<td>
&lt;node&gt;
</td>
<td>
<b>version 3.1.0</b> Immediately disconnects from the specified node.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getaddednodeinfo</code>
</td>
<td>
&lt;dns&gt; [node]
</td>
<td>
Returns information about the given added node, or all added nodes.
(note that onetry addnodes are not listed here)
If dns is false, only a list of added nodes will be provided,
otherwise connected information will also be available.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getconnectioncount</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns the number of connections to other nodes.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getnettotals</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns information about network traffic, including bytes in, bytes out, and current time.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getnetworkinfo</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns an object containing various state info regarding P2P networking.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getpeerinfo</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns data about each connected network node as a json array of objects.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listbanned</code>
</td>
<td>
&nbsp;
</td>
<td>
<b>version 3.1.0</b> List all banned IPs/Subnets.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>ping</code>
</td>
<td>
&nbsp;
</td>
<td>
Requests that a ping be sent to all other nodes, to measure ping time.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>setban</code>
</td>
<td>
&lt;ip(/netmask)&gt; &lt;add|remove&gt; [bantime] [absolute]
</td>
<td>
<b>version 3.1.0</b> Attempts add or remove a IP/Subnet from the banned list.
</td>
<td>
N
</td>
</tr>
<tr><th colspan="4">
bulwark
</th></tr>
<tr>
<td>
<code>checkbudgets</code>
</td>
<td>
&nbsp;
</td>
<td>
Initiates a buddget check cycle manually.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>createmasternodekey</code>
</td>
<td>
&nbsp;
</td>
<td>
Create a new masternode private key.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getbudgetinfo</code>
</td>
<td>
[proposal]
</td>
<td>
Show current masternode budgets with optional filter by proposal name.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getbudgetprojection</code>
</td>
<td>
&nbsp;
</td>
<td>
Show the projection of which proposals will be paid the next cycle.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getbudgetvotes</code>
</td>
<td>
&lt;proposal&gt;
</td>
<td>
Print vote information for a budget proposal.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getmasternodecount</code>
</td>
<td>
&nbsp;
</td>
<td>
Get masternode count values.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getmasternodeoutputs</code>
</td>
<td>
&nbsp;
</td>
<td>
Print all masternode transaction outputs.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getmasternodescores</code>
</td>
<td>
[blocks=10]
</td>
<td>
Print list of winning masternode by score.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getmasternodestatus</code>
</td>
<td>
&nbsp;
</td>
<td>
Print masternode status.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getmasternodewinners</code>
</td>
<td>
[blocks=10] [filter]
</td>
<td>
Print the masternode winners for the last <i>n</i> blocks
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getnextsuperblock</code>
</td>
<td>
&nbsp;
</td>
<td>
Print the next super block height
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getpoolinfo</code>
</td>
<td>
&nbsp;
</td>
<td>
<b>Depreciated in version 3.0.0</b> Returns anonymous pool-related information
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listmasternodeconf</code>
</td>
<td>
[filter]
</td>
<td>
Print masternode.conf in JSON format.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listmasternodes</code>
</td>
<td>
[filter]
</td>
<td>
Get a ranked list of masternodes. Optional filter by txhash, status, or payment address.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>masternode</code>
</td>
<td>
&lt;command&gt;
</td>
<td>
<b>Depreciated in version 2.3.1</b> Set of commands to execute masternode related actions.
</td>
<td>
Y/N
</td>
</tr>
<tr>
<td>
<code>masternodeconnect</code>
</td>
<td>
&lt;address&gt;
</td>
<td>
Attempts to connect to specified masternode address.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>masternodecurrent</code>
</td>
<td>
&amp;nbsp
</td>
<td>
Get current masternode winner.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>masternodedebug</code>
</td>
<td>
&amp;nbsp
</td>
<td>
Print masternode status.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>mnbudget</code>
</td>
<td>
&lt;command&gt; [passphrase]
</td>
<td>
<b>Depreciated in version 2.3.1</b> Vote or show current budgets.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>mnbudgetrawvote</code>
</td>
<td>
&lt;masternode-tx-hash&gt; &lt;masternode-tx-index&gt; &lt;proposal-hash&gt; &lt;yes|no&gt; &lt;time&gt; &lt;vote-sig&gt;
</td>
<td>
Compile and relay a proposal vote with provided external signature instead of signing vote internally.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>mnbudgetvote</code>
</td>
<td>
&lt;local|many|alias&gt; &lt;votehash&gt; &lt;yes|no&gt; [alias]
</td>
<td>
Vote on a budget proposal.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>mnfinalbudget</code>
</td>
<td>
&lt;command&gt; [passphrase]
</td>
<td>
Vote or show current budgets.
</td>
<td>
Y/N
</td>
</tr>
<tr>
<td>
<code>mnsync</code>
</td>
<td>
&lt;status|reset&gt;
</td>
<td>
Returns the sync status or resets sync.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>preparebudget</code>
</td>
<td>
&lt;proposal-name&gt; &lt;url&gt; &lt;payment-count&gt; &lt;block-start&gt; &lt;bulwark-address&gt; &lt;monthly-payment&gt;
</td>
<td>
Prepare proposal for network by signing and creating tx.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>spork</code>
</td>
<td>
&lt;show|active&gt;
</td>
<td>
Print raw value or active status of sporks.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>startmasternode</code>
</td>
<td>
&lt;local|all|many|missing|disabled|alias&gt; &lt;lockwallet&gt; [alias]
</td>
<td>
Attempts to start one or more masternode(s).
</td>
<td>
Y/N
</td>
</tr>
<tr>
<td>
<code>submitbudget</code>
</td>
<td>
&lt;proposal-name&gt; &lt;url&gt; &lt;payment-count&gt; &lt;block-start&gt; &lt;bulwark-address&gt; &lt;monthly-payment&gt; &lt;fee-tx&gt;
</td>
<td>
Submit proposal to the network.
</td>
<td>
N
</td>
</tr>
<tr><th colspan="4">
Raw Transactions
</th></tr>
<tr>
<td>
<code>createrawtransaction</code>
</td>
<td>
[{"txid":txid,"vout":n},...] {address:amount,...}
</td>
<td>
Creates a raw transaction spending given inputs.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>decoderawtransaction</code>
</td>
<td>
&lt;hex string&gt;
</td>
<td>
Produces a human-readable JSON object for a raw transaction.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>decodescript</code>
</td>
<td>
&lt;hex&gt;
</td>
<td>
Decode a hex-encoded script.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getrawtransaction</code>
</td>
<td>
&lt;txid&gt; [verbose=0]
</td>
<td>
Returns raw transaction representation for given transaction id.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>sendrawtransaction</code>
</td>
<td>
&lt;hexstring&gt; [allowhighfees=false] [swiftx=false]
</td>
<td>
Submits raw transaction (serialized, hex-encoded) to local node and network.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>signrawtransaction</code>
</td>
<td>
&lt;hexstring&amp;rt; [{"txid":txid,"vout":n,"scriptPubKey":hex},...] [&lt;privatekey1&gt;,...] [sighashtype=ALL]
</td>
<td>
Adds signatures to a raw transaction and returns the resulting raw transaction.
</td>
<td>
Y/N
</td>
</tr>
<tr><th colspan="4">
Utility
</th></tr>
<tr>
<td>
<code>createmultisig</code>
</td>
<td>
&lt;nrequired&gt; &lt;'["key",...]'&gt;
</td>
<td>
Creates a multi-signature address with n signature of m keys required.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>estimatefee</code>
</td>
<td>
&lt;nblocks&gt;
</td>
<td>
Estimates the approximate fee per kilobyte needed for a transaction to begin confirmation within nblocks blocks.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>estimatepriority</code>
</td>
<td>
&lt;nblocks&gt;
</td>
<td>
Estimates the approximate priority a zero-fee transaction needs to begin confirmation within nblocks blocks.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>validateaddress</code>
</td>
<td>
&lt;bulwarkaddress&gt;
</td>
<td>
Return information about the given bulwark address.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>verifymessage</code>
</td>
<td>
&lt;bulwarkaddress&gt; &lt;signature&gt; &lt;message&gt;
</td>
<td>
Verify a signed message.
</td>
<td>
N
</td>
</tr>
<tr><th colspan="4">
Wallet
</th></tr>
<tr>
<td>
<code>addmultisigaddress</code>
</td>
<td>
&lt;nrequired&gt; &lt;'["key",...]'&gt; [account]
</td>
<td>
Add a nrequired-to-sign multisignature address to the wallet.
Each key is a bulwark address or hex-encoded public key.
If 'account' is specified, assign address to that account.
</td>
<td>
Y/N
</td>
</tr>
<tr>
<td>
<code>autocombinerewards</code>
</td>
<td>
&lt;true|false&gt; [threshold]
</td>
<td>
Wallet will automatically monitor for any coins with value below the threshold amount, and combine them if they reside with the same bulwark address.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>backupwallet</code>
</td>
<td>
&lt;destination&gt;
</td>
<td>
Safely copies wallet.dat to destination, which can be a directory or a path with filename.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>bip38decrypt</code>
</td>
<td>
&lt;bulwarkaddress&gt; &lt;passphrase&gt;
</td>
<td>
Decrypts and then imports password protected private key.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>bip38encrypt</code>
</td>
<td>
&lt;bulwarkaddress&gt; &lt;passphrase&gt;
</td>
<td>
Encrypts a private key corresponding to 'bulwarkaddress'.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>dumpprivkey</code>
</td>
<td>
&lt;bulwarkaddress&gt;
</td>
<td>
Reveals the private key corresponding to 'bulwarkaddress'.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>dumpwallet</code>
</td>
<td>
&lt;filename&gt;
</td>
<td>
Dumps all wallet keys in a human-readable format.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>encryptwallet</code>
</td>
<td>
&lt;passphrase&gt;
</td>
<td>
Encrypts the wallet with &lt;passphrase&gt;.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getaccount</code>
</td>
<td>
&lt;bulwarkaddress&gt;
</td>
<td>
Returns the account associated with the given address.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getaccountaddress</code>
</td>
<td>
&lt;account&gt;
</td>
<td>
Returns the current bitcoin address for receiving payments to this account. If &lt;account&gt; does not exist, it will be created along with an associated new address that will be returned.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getaddressesbyaccount</code>
</td>
<td>
&lt;account&gt;
</td>
<td>
Returns the list of addresses for the given account.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getbalance</code>
</td>
<td>
[account] [minconf=1] [includeWatchonly=false]
</td>
<td>
If [account] is not specified, returns the server's total available balance.<br>If [account] is specified, returns the balance in the account.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getnewaddress</code>
</td>
<td>
[account]
</td>
<td>
Returns a new bulwark address for receiving payments.  If [account] is specified payments received with the address will be credited to [account].
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>getrawchangeaddress</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns a new bulwark address, for receiving change.  This is for use with raw transactions, NOT normal use.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getreceivedbyaccount</code>
</td>
<td>
[account] [minconf=1]
</td>
<td>
Returns the total amount received by addresses with [account] in transactions with at least [minconf] confirmations. If [account] not provided return will include all transactions to all accounts.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getreceivedbyaddress</code>
</td>
<td>
&lt;bulwarkaddress&gt; [minconf=1]
</td>
<td>
Returns the amount received by &lt;bulwarkaddress&gt; in transactions with at least [minconf] confirmations. It correctly handles the case where someone has sent to the address in multiple transactions. Keep in mind that addresses are only ever used for receiving transactions. Works only for addresses in the local wallet, external addresses will always show 0.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getstakesplitthreshold</code>
</td>
<td>
&amp;nbsp
</td>
<td>
Returns the threshold for stake splitting.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getstakingstatus</code>
</td>
<td>
&amp;nbsp
</td>
<td>
Returns an object containing various staking information.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>gettransaction</code>
</td>
<td>
&lt;txid&gt; [includeWatchonly]
</td>
<td>
Get detailed information about in-wallet transaction &lt;txid&gt;.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getunconfirmedbalance</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns the server's total unconfirmed balance
.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>getwalletinfo</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns an object containing various wallet state info.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>importaddress</code>
</td>
<td>
&lt;address&gt; [label] [rescan=true]
</td>
<td>
Adds an address or script (in hex) that can be watched as if it were in your wallet but cannot be used to spend.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>importprivkey</code>
</td>
<td>
&lt;bulwarkprivkey&gt; [label] [rescan=true]
</td>
<td>
Adds a private key (as returned by dumpprivkey) to your wallet.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>importwallet</code>
</td>
<td>
&lt;filename&gt;
</td>
<td>
Imports keys from a wallet dump file (see dumpwallet).
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>keypoolrefill</code>
</td>
<td>
&lt;newsize&gt;
</td>
<td>
Fills the keypool.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>listaccounts</code>
</td>
<td>
[minconf] [includeWatchonly=false]
</td>
<td>
Returns Object that has account names as keys, account balances as values.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listaddressgroupings</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns all addresses in the wallet and info used for coincontrol.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listlockunspent</code>
</td>
<td>
&nbsp;
</td>
<td>
Returns list of temporarily unspendable outputs.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listreceivedbyaccount</code>
</td>
<td>
[minconf=1] [includeempty=false] [includeWatchonly=false]
</td>
<td>
List balances by account.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listreceivedbyaddress</code>
</td>
<td>
[minconf=1] [includeempty=false] [includeWatchonly=false]
</td>
<td>
List balances by receiving address.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listsinceblock</code>
</td>
<td>
[blockhash] [target-confirmations] [includeWatchonly=false]
</td>
<td>
Get all transactions in blocks since block [blockhash], or all transactions if omitted.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listtransactions</code>
</td>
<td>
[account] [count=10] [from=0] [includeWatchonly=false]
</td>
<td>
Returns up to [count] most recent transactions skipping the first [from] transactions for account [account]. If [account] not provided it'll return recent transactions from all accounts.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>listunspent</code>
</td>
<td>
[minconf=1] [maxconf=999999] ['["addresses",...]']
</td>
<td>
Returns array of unspent transaction outputs with between minconf and maxconf (inclusive) confirmations. Optionally filter to only include txouts paid to specified addresses.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>lockunspent</code>
</td>
<td>
&lt;unlock&gt; &lt;'[{"txid":"txid","vout":n},...]'&gt;
</td>
<td>
Updates list of temporarily unspendable outputs.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>move</code>
</td>
<td>
&lt;fromaccount&gt; &lt;toaccount&gt; &lt;amount&gt; [minconf=1] [comment]
</td>
<td>
Move from one account in your wallet to another
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>sendfrom</code>
</td>
<td>
&lt;fromaccount&gt; &lt;tobulwarkaddress&gt; &lt;amount&gt; [minconf=1] [comment] [comment-to]
</td>
<td>
&lt;amount&gt; is a real and is rounded to 8 decimal places. Will send the given amount to the given address, ensuring the account has a valid balance using [minconf] confirmations. Returns the transaction ID if successful (not in JSON object).
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>sendmany</code>
</td>
<td>
&lt;fromaccount&gt; {address:amount,...} [minconf=1] [comment]
</td>
<td>
Send multiple times. Amounts are double-precision floating point numbers
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>sendtoaddress</code>
</td>
<td>
&lt;bulwarkaddress&gt; &lt;amount&gt; [comment] [comment-to]
</td>
<td>
Send an amount to a given address. &lt;amount&gt; is a real and is rounded to 8 decimal places. Returns the transaction ID &lt;txid&gt; if successful.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>sendtoaddressix</code>
</td>
<td>
&lt;bulwarkaddress&gt; &lt;amount&gt; [comment] [comment-to]
</td>
<td>
Send an amount to a given address using SwiftX. &lt;amount&gt; is a real and is rounded to 8 decimal places. Returns the transaction ID &lt;txid&gt; if successful.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>setaccount</code>
</td>
<td>
&lt;bulwarkaddress&gt; &lt;account&gt;
</td>
<td>
Sets the account associated with the given address. Assigning address that is already assigned to the same account will create a new address associated with that account.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>setstakesplitthreshold</code>
</td>
<td>
&lt;value&gt;
</td>
<td>
This will set the output size of your stakes to never be below the given value.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>settxfee</code>
</td>
<td>
&lt;amount&gt;
</td>
<td>
Set the transaction fee per kB.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>signmessage</code>
</td>
<td>
&lt;bulwarkaddress&gt; &lt;message&gt;
</td>
<td>
Sign a message with the private key of an address.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>walletlock</code>
</td>
<td>
&nbsp;
</td>
<td>
Removes the wallet encryption key from memory, locking the wallet.
</td>
<td>
Y
</td>
</tr>
<tr>
<td>
<code>walletpassphrase</code>
</td>
<td>
&lt;passphrase&gt; &lt;timeout&gt; [anonymizeonly=false]
</td>
<td>
Stores the wallet decryption key in memory for 'timeout' seconds.
</td>
<td>
N
</td>
</tr>
<tr>
<td>
<code>walletpassphrasechange</code>
</td>
<td>
&lt;oldpassphrase&gt; &lt;newpassphrase&gt;
</td>
<td>
Changes the wallet passphrase from 'oldpassphrase' to 'newpassphrase'.
</td>
<td>
N
</td>
</tr>
</tbody></table>
