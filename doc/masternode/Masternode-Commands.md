# Masternode Commands

`systemctl start bulwarkd`
\- Starts the Bulwark Daemon

`systemctl stop bulwarkd`
\- Stops the Bulwark Daemon

`systemctl restart bulwarkd`
\- Restarts the Bulwark Daemon

`systemctl status bulwarkd`
\- Displays the status of the Bulwark Daemon

`bulwark-cli masternode status`
\- Displays the status of the Bulwark masternode running on the VPS

`bulwark-cli getinfo`
\- Displays general info about the masternode

`bulwark-cli masternodecurrent`
\- Displays additional info about the masternode

`ps aux | grep bulwark`
\- Shows if the bulwarkd process is running

`dmesg | egrep -i 'killed process'`
\- Lets you know whether bulwarkd was killed due to lack of memory

`nano ~/.bulwark/bulwark.conf`
\- Edits your bulwark.conf file

`killall -9 bulwarkd`
\- Force quits bulwarkd (*USE WITH CAUTION*)

`bulwark-cli getpeerinfo | grep synced_headers`
\- Displays synced headers

`bulwark-cli getmasternodecount`
\- Displays count of all masternodes

`bash <( curl https://raw.githubusercontent.com/bulwark-crypto/Bulwark-MN-Install/master/refresh_node.sh )`

Refreshes your node by clearing the chaindata

`bash <( curl https://raw.githubusercontent.com/bulwark-crypto/Bulwark-MN-Install/master/update_node.sh )`

Updates your node to the newest version
