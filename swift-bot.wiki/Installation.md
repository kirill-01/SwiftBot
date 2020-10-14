# System installation process

**Before starting the installation process, please make sure that your system meets the requirements**

### <span style="color:red">*All shell command below must be executed with root rights (e.g. sudo)*</span>

## Dependencies 

Install required dependencies to host system 

```shell script
apt update && apt install curl certbot nginx php7.4 php7.4-fpm mysql-server composer python3-pip build-essential python-dev  libssl-dev libffi-dev libreadline-dev libbz2-dev libsqlite3-dev libncurses5-dev libsnappy-dev libunwind-dev apt-transport-https ca-certificates dirmngr
```

## Clickhouse server

Clickhouse server is used to store the history of quotes and instant analysis of data on rates.
**Please follow the instructions on the link below to install and configure clickhouse:**

[Clickhouse server configuration](clickhouse_config.md)


 
## NGINX

NGINX server used to manage connections to the server. **Please follow the instructions on the link below to correctly configure NGINX**

[NGINX configuration description](nginx_config.md)


## MySQL database

MySQL server used to you data, such as orders history, system settings, accounts data. **Please follow the instructions on the link below to correctly configure MySQL server**

[MySQL configuration description](mysql_config.md)

## Crossbar.io

Crossbar.io is a heart of IPC. **Please follow the instructions on the link below to correctly configure Crossbar.io**

[Crossbar onfiguration description](crossbar_config.md)


## SWIFT.bot package

Package installation process.

#### Download deb package release or [build it local](build_from_source.md)

```shell script
dpkg -i /tmp/swift-core.deb
```

Configure system as it described here:
[SWIFT.BOT system configuration file](Settings.md)


#### Enable services

```shell script
systemctl enable swift-worker-logs && systemctl enable swift-orderbooks-proxy && systemctl enable swift-bot && systemctl enable swift-api && systemctl enable swift-worker-settingshost && systemctl enable swift-worker-balances && systemctl enable swift-worker-orders
```
#### Start services


```shell script
systemctl restart swift-orderbooks-proxy swift-worker-settingshost swift-worker-logs swift-worker-balances swift-worker-orders swift-bot swift-api
```


## SWIFT.bot - [Back to home](HomePage.md)