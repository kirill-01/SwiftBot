# Clickhouse server configuration

### [Clickhouse homepage](https://clickhouse.tech/)
### [Official documentation](https://clickhouse.tech/docs/en/)

### <span style="color:red">*All shell command below must be executed with root rights (e.g. sudo)*</span>

## Install server


```shell script
apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv E0C56BD4
echo "deb https://repo.clickhouse.tech/deb/stable/ main/" | sudo tee  /etc/apt/sources.list.d/clickhouse.list
apt-get update && apt-get install -y clickhouse-server clickhouse-client
```

## Edit config

[Config file official documentation](https://clickhouse.tech/docs/ru/operations/server-configuration-parameters/settings)

Now you need to enable mysql interface in clickhouse. 
Do it inside default config file, located at: **/opt/clickhouse/default.xml**

[Mysql port param description](https://clickhouse.tech/docs/ru/operations/server-configuration-parameters/settings/#server_configuration_parameters-mysql_port)
```xml
<mysql_port>9004</mysql_port>
```

## Start server

```shell script
service clickhouse-server start
```


## Create required tables:

Connect to local clickhouse server using default mysql client:
```shell script
mysql --protocol tcp -u default -P 9004
```

And, create tables inside default database:

`Choose you correct TimeZone if needed!`

```mysql
CREATE TABLE `asks` (`d` Date, `ts` DateTime('Europe/Moscow'),`pair` Int32,`rate` Decimal128(8),`amount` Decimal128(8)) ENGINE = MergeTree(d, (pair, rate), 8192);
CREATE TABLE `bids` (`d` Date, `ts` DateTime('Europe/Moscow'),`pair` Int32,`rate` Decimal128(8),`amount` Decimal128(8)) ENGINE = MergeTree(d, (pair, rate), 8192);
CREATE TABLE `balances` (`d` Date, `ts` DateTime('Europe/Moscow'),`currency` Int32,`coin` Int32,`balance` Decimal128(8),`usd` Decimal128(8)) ENGINE = MergeTree(d, (currency, balance), 8192);
```

## Generate required SSL certificates

```shell script
certbot certonly
```

## SWIFT.bot - [Back to home](HomePage.md)