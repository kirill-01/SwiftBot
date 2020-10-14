# SWIFT.bot - Automated trading system for cryptocurrency exchanges written in C++

[![GitHub release](https://github.com/Wounderer/SwiftBot/wiki/https://img.shields.io/github/release/Naereen/StrapDown.js.svg)](https://github.com/Wounderer/SwiftBot/wiki/https://github.com/Wounderer/SwiftBot/releases)
[![GPLv3 license](https://github.com/Wounderer/SwiftBot/wiki/https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/Wounderer/SwiftBot/wiki/http://perso.crans.org/besson/LICENSE.html)
[![Open Source? Yes!](https://github.com/Wounderer/SwiftBot/wiki/https://badgen.net/badge/Open%20Source%20%3F/Yes%21/blue?icon=github)](https://github.com/Wounderer/SwiftBot/wiki/https://github.com/Naereen/badges/)

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

---

### [Security rules](https://github.com/Wounderer/SwiftBot/wiki/security) · [System requirements](https://github.com/Wounderer/SwiftBot/wiki/System_requirements) · [Installation](https://github.com/Wounderer/SwiftBot/wiki/Installation) · [API](https://github.com/Wounderer/SwiftBot/wiki/api_methods) · [License](https://github.com/Wounderer/SwiftBot/wiki/LICENSE) · [Support](https://github.com/Wounderer/SwiftBot/wiki/Support)

---

## Exchanges

### List of supported exchanges

| Exchange   | API status | API version | Withdraw whitelists | IP whitelists | Exchange URL |
| ------- | ------------------ | ---- | ---- | ---- | ---- |
| [Bittrex](https://github.com/Wounderer/SwiftBot/wiki/bittrex) | :heavy_check_mark: | v1.0 | :heavy_check_mark: | :heavy_check_mark: | https://bittrex.com |
| [Bitfinex](https://github.com/Wounderer/SwiftBot/wiki/bitfinex)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://bitfinex.com |
| [Kraken](https://github.com/Wounderer/SwiftBot/wiki/kraken)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://kraken.com |
| [KuCoin](https://github.com/Wounderer/SwiftBot/wiki/kucoin)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://kucoin.com |
| [zb.com](https://github.com/Wounderer/SwiftBot/wiki/zb)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://zb.com |
| [Binance](https://github.com/Wounderer/SwiftBot/wiki/binance) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://binance.com |
| [HitBTC](https://github.com/Wounderer/SwiftBot/wiki/hitbtc)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://hitbtc.com |
| [Huobi](https://github.com/Wounderer/SwiftBot/wiki/huobi) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://huobi.com |
| [Cex.io](https://github.com/Wounderer/SwiftBot/wiki/cexio)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://cex.io |
| [Exmo](https://github.com/Wounderer/SwiftBot/wiki/exmo)  | :heavy_check_mark: | v1.0| :heavy_multiplication_x: | :heavy_multiplication_x: | https://exmo.me |
| [Livecoin](https://github.com/Wounderer/SwiftBot/wiki/livecoin)  | :heavy_check_mark: | v1.0| :heavy_multiplication_x: | :heavy_check_mark: | https://livecoin.ru |
| [OkCoin](https://github.com/Wounderer/SwiftBot/wiki/okcoin)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://okcoin.com |
| [LakeBTC](https://github.com/Wounderer/SwiftBot/wiki/lakebtc) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://lakebtc.com |
| [LBank](https://github.com/Wounderer/SwiftBot/wiki/lbank) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://lbank.com |


### [Security settings recommendations](https://github.com/Wounderer/SwiftBot/wiki/exchanges_security)

--- 

## Trading strategies

Out of the box SWIFT.bot can use two main arbitrage strategies

### [Classic inter-exchange arbitrage](https://github.com/Wounderer/SwiftBot/wiki/classic)
### [Statistical inter-exchange arbitrage](https://github.com/Wounderer/SwiftBot/wiki/statistical)

By using SWIFT.bot **[API interface](https://github.com/Wounderer/SwiftBot/wiki/api_methods)**, you can create your own trading systems on top of the system to implement any other trading algorithms in the shortest possible time.

---

## System

### [Building from sources](https://github.com/Wounderer/SwiftBot/wiki/build_from_source)

### [Settings](https://github.com/Wounderer/SwiftBot/wiki/Settings)

### [Best practice](https://github.com/Wounderer/SwiftBot/wiki/Best_practice)

---

## API interfaces

### [Configuring API server](https://github.com/Wounderer/SwiftBot/wiki/api_config)

### [API usage examples](https://github.com/Wounderer/SwiftBot/wiki/api_examples)

---

## Contribution

### [Implementing new exchange API interface](https://github.com/Wounderer/SwiftBot/wiki/implement_api)

---

## Android app

In current release, android 8.1+ is required!

### [Download android app](https://github.com/Wounderer/SwiftBot/wiki/https://github.com/Wounderer/SwiftBot/releases/download/v2.0.2046/swift2.apk)
### [Configure android app and connect it to server](https://github.com/Wounderer/SwiftBot/wiki/android_manual)

## Contacts

### Author
Email: **kirill.kkn@gmail.com**

GitHub: [Wounderer](https://github.com/Wounderer/SwiftBot/wiki/https://github.com/Wounderer)
