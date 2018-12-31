# Accounts Explained

Think about what happens when you give a bank teller some cash and ask that it be deposited into your account. You tell them your account number, and they credit your account with that amount of money.

The money itself is mixed up with all of the other money in the bank, and is given out to other customers with other account numbers.

With the Bitcoin "accounts" feature, you are the bank. The balance you see is the total balance for all accounts, and you can create an arbitrary number of accounts.

When you receive bitcoins, they are always assigned to one of your accounts, and you can change which account is credited based on which bitcoin address receives the coins, just like you tell a bank teller which account to credit when you deposit cash in your bank. However, sending bitcoins is like withdrawing cash from the bank; the coins that are sent out and debited from an account are almost always not the same coins that were deposited into that account.

# Accounts

Bitcoin version 0.3.18 and later implements several RPC methods to maintain separate account balances in a single Bitcoin wallet.  The accounts feature makes it easy to keep track of how much money you have received from different sources or to keep track of how much money you have spent on different things.

# Account Weaknesses

Since the accounts feature was introduced, several services have used it to keep track of customer's bitcoin balances and have had the following problems:

* Wallet backups are an issue; if you rely on a good backup of wallet.dat then a backup must be done every time an address is associated with an account and every time the 'move' command is used.
* The accounts code does not scale up to thousands of accounts with tens of thousands of transactions, because by-account (and by-account-by-time) indices are not implemented. So many operations (like computing an account balance) require accessing every wallet transaction.
* Most applications already have a customer database, implemented with MySQL or some other relational database technology. It is awkward at best to keep the bitcoin-maintained Berkely DB wallet database and the application database backed up and synchronized at all times.

# Account Names

Accounts are named with arbitrary strings; you may use any [JSON](http://www.json.org/) string other than "*" (JSON strings are sent and returned as UTF-8 encoded Unicode).

Bitcoin creates two accounts automatically:  it implicitly creates a default account with the empty string as its name, and it explicitly creates an account named '''Your Address''' when a new wallet is created.

# The Default Account

The default account is named with the empty string ("" in JSON).  Generated coins are always credited to the default account, and the ''sendtoaddress'' method always debits the default account.

# Accounts and Receiving Addresses

Each account is associated with zero or more receiving addresses, and every receiving address is associated with exactly one account.  Coins sent to a receiving address in the wallet are credited to the associated account.

Accounts are associated with receiving addresses by using the ''getaccountaddress'', ''getnewaddress'' or ''setaccount'' methods.

''getaccountaddress'' will return the same address until coins are received on that address; once coins have been received, it will generate and return a new address.

''getnewaddress'' always generates and returns a new address.

''setaccount'' changes the account associated with an existing address.  Coins previously received on that address (if any) will be debited from the previous account's balance and credited to the address' new account.  Note that doing so may make the previous account's balance negative.

Use the ''getaddressesbyaccount'' method to list all addresses associated with an account.

# Sending

The ''sendfrom'' method sends coins and debits the specified account.  It does     *not    * change Bitcoin's algorithm for selecting which coins in the wallet are sent-- you should think of the coins in the wallet as being mixed together when they are received.  There is no way to ask Bitcoin to "create a payment transaction using the coins received from these previously received transactions" without using the raw transactions API(which is not part of the account system.)

The ''sendtoaddress'' method works like ''sendfrom'', but always debits the default account.

The send will fail if the account has insufficient funds, with two exceptions:

  - 'sendtoaddress' always succeeds if there are sufficient funds in the
    server's wallet.  For example, if your wallet account balances were 100 BTC in account
    'foo' and 0 BTC in the default account, then the balances after ''sendtoaddress
    1PC9aZC4hNX2rmmrt7uHTfYAS3hRbph4UN 10.00'' would be 100 in account 'foo' and -10.00 in
    the default account (and the overall server balance would go from 100 to 90 BTC).  On
    the other hand, using 'sendfrom' to send from the default account with a zero balance
    will fail with message "Account has insufficient funds".

  - The check for sufficient funds is done before paying transaction fees (if any); if a
    [fee]([transaction)] is needed, and there are sufficient funds in the wallet, then the
    transaction fee will be paid and debited from the account.  For example, if account
    'foo' contains 10 bitcoins, you ''sendfrom foo 15VjRaDX9zpbA8LVnbrCAFzrVzN7ixHNsC 10'',
    and the transaction costs 0.01, 'foo's balance will be -0.01 bitcoins.

# Account -> Account Transfers

Use the ''move'' method to transfer balances from one account to another. Moves are not broadcast to the network, and never incur transaction fees; they just adjust account balances in the wallet.

# Account Balance and History

The ''getbalance'' method returns the bitcoin balance for either the entire wallet (if no argument is given) or for a particular account.

The ''listtransactions <account> [method returns the last N (default 10) transactions that affected the account's balance.  "listtransactions '*' [N](N]'')" will return the last N transactions for all accounts.

# Typical Uses

This section describes how typical web site code running on a web server uses the JSON-RPC API to keep track of customers' accounts.

* Customer '''creates an account''' on the website:  web server either assigns them a unique customer id number or uses their email address or other unique identifier, calls ''getaccountaddress "userid"'' and tells the customer to send to that address to fund their account.
* Customer '''receives coins''' to fund their account: web server isn't involved.
* Customer is shown their '''current balance''':  ''getbalance "userid" 6'' to get their 'confirmed' balance, and subtracts it from ''getbalance "userid" 0'' to get their 'unconfirmed' balance.
* Show the customer an     *itemized list    * of transactions:  ''listtransactions "userid"''
* Customer '''sends coins''' to another bitcoin address: ''sendfrom "userid" <address> <amount>''
* Customer '''transfers coins''' to another customer:  ''move "userid1" "userid2" <amount>''
* You '''make a sale''', paid for with bitcoins in the customer's account:  ''move "userid" "" <amount> 6 "purchased item"'', and if it succeeds, send them the product.
* Customer is '''charged a fee''' for use of the service:  ''move "userid" "FEES" <amount>''  (using special accounts like "FEES" can make your application's logic much simpler)
* Customer '''purchases bitcoins''' from you:  ''move "AVAILABLE" "userid" <amount>'' (assuming the bitcoins you are selling are kept track of in an "AVAILABLE" account)
