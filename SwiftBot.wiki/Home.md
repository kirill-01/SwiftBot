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

## [Security rules](security)


---

### [License](LICENSE)
### [Support](Support)

---

## Exchanges

### List of supported exchanges

| Exchange   | API status | API version | Withdraw whitelists | IP whitelists | Exchange URL |
| ------- | ------------------ | ---- | ---- | ---- | ---- |
| [Bittrex](bittrex) | :heavy_check_mark: | v1.0 | :heavy_check_mark: | :heavy_check_mark: | https://bittrex.com |
| [Bitfinex](bitfinex)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://bitfinex.com |
| [Kraken](kraken)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://kraken.com |
| [KuCoin](kucoin)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://kucoin.com |
| [zb.com](zb)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://zb.com |
| [Binance](binance) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://binance.com |
| [HitBTC](hitbtc)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://hitbtc.com |
| [Huobi](huobi) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://huobi.com |
| [Cex.io](cexio)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://cex.io |
| [Exmo](exmo)  | :heavy_check_mark: | v1.0| :heavy_multiplication_x: | :heavy_multiplication_x: | https://exmo.me |
| [Livecoin](livecoin)  | :heavy_check_mark: | v1.0| :heavy_multiplication_x: | :heavy_check_mark: | https://livecoin.ru |
| [OkCoin](okcoin)  | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://okcoin.com |
| [LakeBTC](lakebtc) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://lakebtc.com |
| [LBank](lbank) | :heavy_check_mark: | v1.0| :heavy_check_mark: | :heavy_check_mark: | https://lbank.com |


### [Security settings recommendations](exchanges_security)

--- 

## Trading strategies

Out of the box SWIFT.bot can use two main arbitrage strategies

### [Classic inter-exchange arbitrage](classic)
### [Statistical inter-exchange arbitrage](statistical)

By using SWIFT.bot **[API interface](api_methods)**, you can create your own trading systems on top of the system to implement any other trading algorithms in the shortest possible time.

---

## System

### [Building from sources](build_from_source)
### [Installation](Installation)

### [Settings](Settings)

### [System requirements](System_requirements)

### [Best practice](Best_practice)

---

## API interfaces

### [Configuring API server](api_config)

### [API usage examples](api_examples)

### [API reference](api_methods)

---

## Contribution

### [Implementing new exchange API interface](implement_api)

---

## Android app

In current release, android 8.1+ is required!

### [Download android app](https://github.com/Wounderer/SwiftBot/releases/download/v2.0.2046/swift2.apk)
### [Configure android app and connect it to server](android_manual)

## Contacts

### Author
Email: **kirill.kkn@gmail.com**

GitHub: [Wounderer](https://github.com/Wounderer)