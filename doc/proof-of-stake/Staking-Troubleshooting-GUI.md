# Staking Troubleshooting - GUI

**This guide is for the Windows, Linux and Mac GUI wallets.**  
**The commands specified will not work as listed for CLI interfaces**

To make sure your wallet is staking, the 1st step is to use the following command in your debug console:

`getstakingstatus`

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

This makes sure you have valid peers, if this is showing as false I'd recommend following the instructions listed in: [Wallet and Syncing Issues](https://kb.bulwarkcrypto.com/Guides/Wallet-Syncing-Issues/)

## walletunlocked

This means your wallet isn't unlocked, just unlock the wallet with your passphrase.

You can check the box that says "for staking and anonymization only". Unlocking for staking only means your funds are safe even if a malicious entity got access to your PC.

## mintablecoins

This is asking if your transaction is old enough to be staked. It takes 475 confirmations, or roughly 12 hours, for a transaction to be able to be staked. Just wait and this should correct to true.

## enoughcoins

This is making sure you have more than 100 BWK in the wallet. If this is appearing false there are a wide number of potential problems. It's best to come ask us in [Discord](https://discord.me/bulwarkcrypto) or [Telegram](https://t.me/bulwarkcrypto) if you have issues with this.

## mnsync

This just makes sure your wallet is fully synced, if you appear to be fully synced I'd recommend following the steps listed in: [Wallet and Syncing Issues](https://kb.bulwarkcrypto.com/Guides/Wallet-Syncing-Issues/)

## staking status

Staking status should be true if everything above is listed as true. If it is still listed as False, close your wallet, re-open it, wait for it to fully sync, and then unlock your wallet for staking only again.
