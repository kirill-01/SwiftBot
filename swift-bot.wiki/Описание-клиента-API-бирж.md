# ExchangeApi интерфейс
Для реализации API интерфейса биржи

## Описание



## API методы, обязательные для реализации

### Методы без авторизации:

* [getCurrencies](Json-структуры#currencies)
* [getMarkets](Json-структуры#markets)
* [getOrderbooks](Json-структуры#orderbooks)


### Приватные методы

**Ордера**

* [order_place](#order)
* [order_get](#orderitem)
* [order_cancel](#orderitem)
* [order_replace](#orderitem)

**Вывод средств**

* [withdraw_list](#withdraw) 
* [withdraw_history](#withdraw) 
* [withdraw_cancel](#withdraw) 
* [withdraw_create](#withdraw) 
* [withdraw_inner](#withdraw) 
* [withdraw_get_fee](#withdrawsettings) 
* [withdraw_get_limits](#withdrawsettings) 

**Балансы**

* [balances_get](#balances) 
* [balances_deposits](#deposits)
* [balances_get_deposit_address](#depositaddress) 

**История торгов**

* [trade_history](#order) 
* [trade_open_orders](#order) 
* trade_get_fee
* trade_get_limits

# RPC Методы для реализации

**Ордера**

* [order_place](#order) : **swiftbot.api.rpc.EXCHANGEID.order.place**
* [order_get](#orderitem) : **swiftbot.api.rpc.EXCHANGEID.order.get**
* [order_cancel](#orderitem) : **swiftbot.api.rpc.EXCHANGEID.order.cancel**
* [order_replace](#orderitem) : **swiftbot.api.rpc.EXCHANGEID.order.replace**

**Вывод средств**

* [withdraw_list](#withdraw) : **swiftbot.api.rpc.EXCHANGEID.withdraw.list**
* [withdraw_history](#withdraw) : **swiftbot.api.rpc.EXCHANGEID.withdraw.history**
* [withdraw_cancel](#withdraw) : **swiftbot.api.rpc.EXCHANGEID.withdraw.cancel**
* [withdraw_create](#withdraw) : **swiftbot.api.rpc.EXCHANGEID.withdraw.create**
* [withdraw_inner](#withdraw) : **swiftbot.api.rpc.EXCHANGEID.withdraw.inner**
* [withdraw_get_fee](#withdrawsettings) : **swiftbot.api.rpc.EXCHANGEID.withdraw.fee**
* [withdraw_get_limits](#withdrawsettings) : **swiftbot.api.rpc.EXCHANGEID.withdraw.limits**

**Балансы**

* [balances_get](#balances) : **swiftbot.api.rpc.EXCHANGEID.balances.get** 
* [balances_deposits](#deposits) : **swiftbot.api.rpc.EXCHANGEID.balances.deposit.list**
* [balances_get_deposit_address](#depositaddress) : **swiftbot.api.rpc.EXCHANGEID.balances.deposit.address**

**История торгов**

* [trade_history](#order) : **swiftbot.api.rpc.EXCHANGEID.trade.history**
* [trade_open_orders](#order) : **swiftbot.api.rpc.EXCHANGEID.trade.active**
* trade_get_fee : **swiftbot.api.rpc.EXCHANGEID.trade.fee**
* trade_get_limits : **swiftbot.api.rpc.EXCHANGEID.trade.limits**


