# SWIFT.Bot

Cryptocurrency trading system


## Installation
### All commands required sudo permissions 

```sh
# Depencies

apt update && apt install curl certbot nginx php7.4 php7.4-fpm mysql-server composer python3-pip build-essential python-dev  libssl-dev libffi-dev libreadline-dev libbz2-dev libsqlite3-dev libncurses5-dev libsnappy-dev libunwind-dev apt-transport-https ca-certificates dirmngr

# Clickhouse server setup 

apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv E0C56BD4
echo "deb https://repo.clickhouse.tech/deb/stable/ main/" | sudo tee  /etc/apt/sources.list.d/clickhouse.list
apt-get update && apt-get install -y clickhouse-server clickhouse-client && 
service clickhouse-server start

# Enable services

systemctl enable swift-worker-logs && systemctl enable swift-orderbooks-proxy && systemctl enable swift-bot && systemctl enable swift-api && systemctl enable swift-worker-settingshost && systemctl enable swift-worker-balances && systemctl enable swift-worker-orders

# Start services

systemctl restart swift-orderbooks-proxy swift-worker-settingshost swift-worker-logs swift-worker-balances swift-worker-orders swift-bot swift-api
```

## Documentation

### Installation process

https://docs.google.com/document/d/1cK5e9Q1O2OfMgEGBx3CpqvqJr5x3ikUm7ufjR7mppx0/edit?usp=sharing

### API reference

https://docs.google.com/document/d/1Fupb4w1gRrT3TAlQh-mUhiQwkZrRnUxDBGTlZWI57dA/edit?usp=sharing


## Supported exchanges

| Exchange   | API status | API version | Exchange URL |
| ------- | ------------------ | ---- | ---- |
| Bittrex | OK :white_check_mark: | v1.0| https://bittrex.com |
| Bitfinex  | OK :white_check_mark: | v1.0| https://bitfinex.com |
| Kraken  | OK :white_check_mark: | v1.0| https://kraken.com |
| KuCoin  | OK :white_check_mark: | v1.0| https://kucoin.com |
| zb.com  | OK :white_check_mark: | v1.0| https://zb.com |
| Binance | OK :white_check_mark: | v1.0| https://binance.com |
| HitBTC  | OK :white_check_mark: | v1.0| https://hitbtc.com |
| Huobi | OK :white_check_mark: | v1.0| https://huobi.com |
| Cex.io  | OK :white_check_mark: | v1.0| https://cex.io |
| Exmo  | OK :white_check_mark: | v1.0| https://exmo.me |
| Livecoin  | OK :white_check_mark: | v1.0| https://livecoin.ru |
| OkCoin  | OK :white_check_mark: | v1.0| https://okcoin.com |
| LakeBTC | OK :white_check_mark: | v1.0| https://lakebtc.com |
| LBank | OK :white_check_mark: | v1.0| https://lbank.com |

