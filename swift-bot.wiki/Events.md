# Системные события

В отдельный WAMP топик шлются системные события, которые вызывают соответствующие событию тригеры, - сохраниение в БД нового ордера, изменение информации о балансах, совершении вывода с одной из бирж и т.д..

## Категории событий 

### Order

События, публикуемые при изменении какого либо ордера.

**placed**

**canceled**

**updated**

**error**


### Balance

События изменения балансов

**update**

```json
{
    "event" : "balance",
    "name" : "update",
    "time" : "SECSSINCE",
    "target" : {
        "11" : {
   "total" : "8.00000000",
  "available" : "8.00000000",
  "in_orders" : "8.00000000",
  "reserved" : "8.00000000",
  "pending" : "8.00000000",
  "today_in_trades" : "0.00000000",
  "required" : "8.00000000"
        }
    }
}
```

**DepositWaiting**

**DepositAccepted**

**WithdrawCreated**

**WithdrawConfirmed**


### Account

**UserActionRequired**

**UserAction**

**UserNotify**

**PeriodResult**


### System

**UpdateReleased**

**RestartRequired**

**UpdateStart**

**UpdateDone**



## Публикация новых событий 

swiftbot.system.events.*


## JSON сообщения

```json
{
    "event" : "order|balance|account|system|custom",
    "name" : "eventname",
    "time" : "SECSSINCE",
    "target" : {}
}
```

