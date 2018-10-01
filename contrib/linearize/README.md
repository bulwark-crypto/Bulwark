# Linearize
Construct a linear, no-fork, best version of the Bulwark blockchain. The scripts
run using Python 3 but are compatible with Python 2.

## Prerequisites

[nist5_hash](https://github.com/bulwark-crypto/nist5_hash) must be installed.

## Configuration

Copy example-linearize.cfg to linearize.cfg and edit.

### Required configuration file settings
* `datadir` (Required if `rpcuser` and `rpcpassword` are not specified)
* `rpcuser`, `rpcpassword` (Required if `datadir` is not specified)
* `output_file`: The file that will contain the final blockchain.

### Optional config file settings
* `host`  (Default: `127.0.0.1`)
* `port`  (Default: `8332`)
* `min_height`, `max_height`: Height of the blockchain to be considered.
* `rev_hash_bytes`: If true, the written block hash list will be byte-reversed. (In other words, the hash returned by getblockhash will have its bytes reversed.) False by default. Intended for generation of standalone hash lists but safe to use with linearize-data.py, which will output the same data no matter which byte format is chosen.
* `debug_output`: Some printouts may not always be desired. If true, such output will be printed.
* `file_timestamp`: Set each file's last-accessed and last-modified times, respectively, to the current time and to the timestamp of the most recent block written to the script's blockchain.
* `genesis`: The hash of the genesis block in the blockchain.
* `input`: bulwarkd blocks/ directory containing blkNNNNN.dat
* `hashlist`: text file containing list of block hashes created by linearize-hashes.py.
* `max_out_sz`: Maximum size for files created by the `output_file` option. (Default: `1000*1000*1000 bytes`)
* `netmagic`: Network magic number.
* `out_of_order_cache_sz`: If out-of-order blocks are being read, the block can be written to a cache so that the blockchain doesn't have to be sought again. This option specifies the cache size. (Default: `100*1000*1000 bytes`)
* `rev_hash_bytes`: If true, the block hash list written by linearize-hashes.py will be byte-reversed when read by linearize-data.py.
* `split_timestamp`: Split blockchain files when a new month is first seen, in addition to reaching a maximum 
file size (`max_out_sz`).

## Usage

The script requires a connection, local or remote, to a JSON-RPC server. Running `bulwarkd` or `bulwark-qt -server` will be sufficient.

To create the bootstrap.dat, run

``` bash
buildbootstrap.sh BLOCKHEIGHT
```

where _BLOCKHEIGHT_ is the newest block you want to add to the bootstrap. Example:

``` bash
buildbootstrap.sh 200000
```
