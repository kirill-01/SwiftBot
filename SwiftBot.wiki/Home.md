# SWIFT.bot

[![GitHub release](https://img.shields.io/github/release/Naereen/StrapDown.js.svg)](https://github.com/Wounderer/SwiftBot/releases)
[![GPLv3 license](https://img.shields.io/badge/License-GPLv3-blue.svg)](http://perso.crans.org/besson/LICENSE.html)
[![Open Source? Yes!](https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Naereen/badges/)

## Automated trading system for cryptocurrency exchanges written in C++

* Designed to run on a dedicated server
* System security and anonymity are completely in your hands
* The system was designed and developed for the fastest possible interaction with exchanges
* Microservices architecture allows you to scale the system, run it on several independent servers and is maximally fault-tolerant
* Targets arbitrage trading
* Provides a unified API (WebSocket, Rest, Webhooks) for working with all supported exchanges, allowing you to integrate **SWIFT.bot** into your own project.
* Fully independent and isolated
* For developers and integrators - requires skills and knowledge in programming and system administration
* The security of your funds depends entirely on the correctness of the security settings of exchange accounts
* Asynchronous system of calls to API methods of exchanges
* Optimized networking subsystem
* Has android application for system management and constant monitoring

# Very important information!

---

## [Security rules](security.md)


---

### [License](LICENSE.md)
### [Support](Support.md)

---

## Exchanges

### List of supported exchanges

| Exchange   | API status | API version | Withdraw whitelists | IP whitelists | Exchange URL |
| ------- | ------------------ | ---- | ---- | ---- | ---- |
| [Bittrex](exchanges/bittrex.md) | :heavy_check_mark: | v1.0 | :heavy_check_mark: | :heavy_check_mark: | https://bittrex.com |
| [Bitfinex](exchanges/bitfinex.md)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://bitfinex.com |
| [Kraken](exchanges/kraken.md)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://kraken.com |
| [KuCoin](exchanges/kucoin.md)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://kucoin.com |
| [zb.com](exchanges/zb.md)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://zb.com |
| [Binance](exchanges/binance.md) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://binance.com |
| [HitBTC](exchanges/hitbtc.md)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://hitbtc.com |
| [Huobi](exchanges/huobi.md) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://huobi.com |
| [Cex.io](exchanges/cexio.md)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://cex.io |
| [Exmo](exchanges/exmo.md)  | :heavy_check_mark: | v1.0| :heavy_multiplication_x: | :heavy_multiplication_x: | https://exmo.me |
| [Livecoin](exchanges/livecoin.md)  | :heavy_check_mark: | v1.0| :heavy_multiplication_x: | :heavy_check_mark: | https://livecoin.ru |
| [OkCoin](exchanges/okcoin.md)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://okcoin.com |
| [LakeBTC](exchanges/lakebtc.md) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://lakebtc.com |
| [LBank](exchanges/lbank.md) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://lbank.com |


### [Security settings recommendations](exchanges_security.md)

--- 

## Trading strategies

Out of the box SWIFT.bot can use two main arbitrage strategies

### [Classic inter-exchange arbitrage](strategies/classic.md)
### [Statistical inter-exchange arbitrage](strategies/statistical.md)

By using SWIFT.bot **[API interface](api_methods.md)**, you can create your own trading systems on top of the system to implement any other trading algorithms in the shortest possible time.

---

## System

### [Building from sources](build_from_source.md)
### [Installation](Installation.md)

### [Settings](Settings.md)

### [System requirements](System_requirements.md)

### [Best practice](Best_practice.md)

---

## API interfaces

### [Configuring API server](api_config.md)

### [API usage examples](api_examples.md)

### [API reference](api_methods.md)

---

## Contribution

### [Implementing new exchange API interface](implement_api.md)

---

## Android app

In current release, android 8.1+ is required!

### [Download android app](https://github.com/Wounderer/SwiftBot/releases/download/v2.0.2046/swift2.apk)
### [Configure android app and connect it to server](android_manual.md)

## Contacts

### Author
Email: **kirill.kkn@gmail.com**

GitHub: [Wounderer](https://github.com/Wounderer)