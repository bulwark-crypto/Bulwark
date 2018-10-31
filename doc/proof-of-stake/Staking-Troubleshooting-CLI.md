# Staking Troubleshooting - CLI

**This guide is for the Windows, Linux, Mac and Raspbian CLI wallets.**  
**The commands specificed will not work as listed for GUI interfaces**

To make sure your wallet is staking, the 1st step is to use the following command:

`bulwark-cli getstakingstatus`

This should hopefully look like this:

`{

    "validtime": true,

    "haveconnections": true,

    "walletunlocked": true,

    "mintablecoins": true,

    "enoughcoins": true,

    "mnsync": true,

    "staking status": true,
}`

But let's go through how to fix each line.

## validtime

This should always be true, this just shows that the POS period is active for the coin.

## haveconnections

This makes sure you have valid peers, if this is showing as false I'd recommend the following command:

`rm -Rf ~/.bulwark/peers.dat`

and then running:

`systemctl restart bulwarkd`

## walletunlocked

This means your wallet isn't unlocked, just run the command below:

`bulwark-cli walletpassphrase '<YOUR PASSWORD>' 99999999 true`

This will unlock your wallet for staking only for a long time period. Unlocking for staking only means your funds are safe even if a malicious entity got access to your VPS.

## mintablecoins

This is asking if your transaction is old enough to be staked. It takes 475 confirmations, roughly 12 hours, for a transaction to be able to be staked. Just wait and this should correct to true.

## enoughcoins

This is making sure you have more than 1 BWK in the wallet. If this is appearing false there are a wide number of potential problems. It's best to come ask us in [Discord](https://discord.me/bulwarkcrypto) or [Telegram](https://t.me/bulwarkcrypto) if you have issues with this.

## mnsync

This just makes sure your wallet is fully synced, if you appear to be fully synced I'd recommend typing:

`bulwark-cli mnsync reset`

and then closing the wallet with

`systemctl stop bulwarkd`

After this, wait a minute, then open it again with:

`systemctl start bulwarkd`

then wait 10 minutes more, before unlocking the wallet with the command:

`bulwark-cli walletpassphrase '<YOUR PASSWORD>' 99999999 true`

On a local wallet, try restarting it, if that doesn't help, resync your chain.

## staking status

Staking status should be true, when staking=1 in your bulwark.conf, and when all other options are also true.

If you find yourself in a situation where this is false while all other indicators are true, type the below:

`cat ~/.bulwark/bulwark.conf`

and confirm the output from this command includes "staking=1".

If it does, follow the below steps:

`systemctl stop bulwarkd`

After this, wait a minute, then open it again with:

`systemctl start bulwarkd`

then wait 10 minutes more, before unlocking the wallet with the command:

`bulwark-cli walletpassphrase '<YOUR PASSWORD>' 99999999 true`

Then, after a few more minutes of the network accepting your stakes, you should find everything to be true when you run

`bulwark-cli getstakingstatus`

