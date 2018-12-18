![Bulwark Logo](https://bulwarkcrypto.com/wp-content/uploads/2018/10/Symbol-Color2.svg)

Bulwark Cryptocurrency
&middot;
[![GitHub license](https://img.shields.io/github/license/bulwark-crypto/Bulwark.svg)](https://github.com/bulwark-crypto/Bulwark/blob/master/COPYING) [![GitHub version](https://badge.fury.io/gh/bulwark-crypto%2FBulwark.svg)](https://badge.fury.io/gh/bulwark-crypto%2FBulwark) [![Discord](https://img.shields.io/discord/374271866308919296.svg)](https://discord.me/bulwarkcrypto)
=====

Bulwark (BWK) is a privacy oriented digital currency with Masternodes implemented.
Bulwark offers both SwiftTX and Obfuscation to enable speedy transactions with a Seesaw Staking / Masternode reward balancing mechanism to promote strong network security after the Proof of Work phase.

__Note: The `master` branch may contain development that is in progress, it is recommended to use the latest tagged version or a binary release.__

## Table of Contents
- [Specifications](#specifications)
- [PoS Staking Requirements](#pos-staking-requirements)
- [PoS Block Rewards](#pos-block-rewards)

<a name="specifications"></a>
## Specifications

| Specification         | Descriptor                              |
|-----------------------|-----------------------------------------|
| Ticker                | BWK                                     |
| Algorithm             | NIST5                                   |
| RPC Port              | 52541                                   |
| P2P Port              | 52543                                   |
| Block Spacing         | 90 Seconds                              |
| Difficulty Algorithm  | Dark Gravity Wave v3.0                  |
| Block Size            | 1MB                                     |
| Mined/Minted Maturity | 67 Blocks (~100 Minutes)                |
| Confirmation          | 6 Blocks (~9 Minutes)                   |
| Circulation (1 Year)  | 14,505,720 BWK                          |
| Circulation (5 Years) | 27,668,220 BWK                          |
| PoW Period            | nHeight ≤ 182,700                       |
| PoS Period            | nHeight ≥ 182,701                       |
| Protocol Support      | IPV4, IPV6, TOR, I2P                    |
| PoS                   | SeeSaw & Split Rewards                  |

<a name="pos-staking-requirements"></a>
## PoS Staking Requirements

| Requirement   | Details              |
|---------------|----------------------|
| Confirmations | 475 Blocks           |
| Amount        | 100 BWK (Per Input)  |
| Wallet        | Unlocked for Staking |

<a name="pos-block-rewards"></a>
## PoS Block Rewards

| Subsidy | Block           | Budget | Circulation    | Note          |
|---------|-----------------|--------|----------------|---------------|
| 37.500  | 182701-199999   | 0%     | SeeSaw         | Year 1        |
| 37.500  | 200000-259200   | 0%     | 35/65          | Year 1        |
| 31.250  | 259201-345600   | 0%     | 35/65          | Year 1        |
| 25.000  | 345601-432000   | 10%    | 30/60          | Year 2        |
| 21.875  | 432001-518400   | 10%    | 30/60          | Year 2        |
| 18.750  | 518401-604800   | 10%    | 30/60          | Year 2        |
| 15.625  | 604801-691200   | 10%    | 30/60          | Year 2        |
| 12.50   | 691201-777600   | 10%    | 30/60          | Year 3        |
| 10.938  | 777601-864000   | 10%    | 30/60          | Year 3        |
| 9.3750  | 864001-950400   | 10%    | 30/60          | Year 3        |
| 7.8120  | 950401-1036800  | 10%    | 30/60          | Year 3        |
| 6.2500  | 1036801-1123200 | 10%    | 30/60          | Year 4        |
| 5.4690  | 1123201-1209600 | 10%    | 30/60          | Year 4        |
| 4.6880  | 1209601-1296000 | 10%    | 30/60          | Year 4        |
| 3.9060  | 1296000-1382400 | 10%    | 30/60          | Year 4        |
| 3.1250  | 1382401-1468800 | 10%    | 30/60          | Year 5        |
| 2.7340  | 1468801-1555200 | 10%    | 30/60          | Year 5        |
| 2.3440  | 1555201-1641600 | 10%    | 30/60          | Year 5        |
| 1.9530  | 1641601-1728000 | 10%    | 30/60          | Year 5        |
| 1.6250  | 1728000+        | 10%    | 30/60          | In Perpetuity |
