# API reference
## Current API version: 2.0

## Websocket entrypoint

*Also, RAW socket*

https://**hostname.com**/ws

tcp://127.0.0.1:8081

tcp://123.456.789.012:8081

## HTTP entrypoint ##

https://**hostname.com**/call

## Response types ##

* VariantList
* JSON Array
* Json Object
* Integer
* Double
* String
* Boolean

https://hostname.com/call

# System methods ##
 
## RPC commands

## Get module information

Getting actual status of running module

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.system.rpc.module.status.[trade, arbitrage, balance, logs, apinodes]** |
| Request params                 | **None** |
| Request params possible values | **None** |
| Request example                |  **swift.system.rpc.module.status.arbitrage** |
| Response type                  |  [VariantList](#response-types) |
#### Response example:
```
0 // Status  ( 0 - Pause, 1 - Active)
example.com // Hostname ( Or ip address of host machine)
2.0.265 // Module build version
"{}" // Json object data string
```



## Run module command
Execute internal module built-in command

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.system.rpc.module.cmd[trade, arbitrage, balance, logs, apinodes]** |
| Request params                 | **COMMAND** |
| Request params possible values | **[pause,start,restart]** |
| Request example                |  **swift.system.rpc.module.cmd.arbitrage** |
| Response type                  |  [Boolean](#resonse-types) |
#### Response example:
```
TRUE
```

## Get last logs or errors
Receive logs or errors records list

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.system.rpc.logs.[errors,last]** |
| Request params                 | ount of records (By default: 25) |
| Request params possible values | **Integer** |
| Request example                |  **swift.system.rpc.logs.last** |
| Response type                  |  [Json array](#resonse-types) |

#### Response example:
```
"[]"
```

## Add error or log
Insert new log or error record into system internal journal

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.system.rpc.logs.[err,log]** |
| Request params                 | **SENDER,GROUP,MESSAGE** |
| Request params possible values | **Sender: String, GROUP: String, Message: String** |
| Request example                |  **swift.system.rpc.logs.err "Bittrex_API","INFO","EXAMPLE MESSAGE"** |
| Response type                  |  [Boolean](#resonse-types) |

#### Response example:
```
TRUE
```


## Run shell command
Execute custom shell command on host machine. 
All commands will be executed by **root** user!

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.system.rpc.shell** |
| Request params                 | **COMMAND** |
| Request params possible values | [String](#resonse-types) |
| Request example                |  **swift.system.rpc.shell "ls -lat /tmp/"** |
| Response type                  |  [String](#resonse-types) |

#### Response example:
```shell script
total 120
drwxrwxrwt 24 root       root       16384 окт 13 23:32 .
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-bolt.service-HTXIAh
drwxrwxrwt  2 root       root        4096 окт 13 21:31 .ICE-unix
drwx------  3 root       root        4096 окт 13 21:31 snap.snap-store
drwxrwxrwt  2 root       root        4096 окт 13 21:31 .X11-unix
drwx------  2 gdm        gdm         4096 окт 13 21:31 tracker-extract-files.124
-r--r--r--  1 gdm        gdm           11 окт 13 21:31 .X1024-lock
-r--r--r--  1 gdm        gdm           11 окт 13 21:31 .X1025-lock
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-upower.service-ivCLZh
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-ModemManager.service-yxGbyi
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-redis-server.service-MrOadh
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-colord.service-LzMszh
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-systemd-logind.service-PaqkDh
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-switcheroo-control.service-VdBVIf
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-iio-sensor-proxy.service-UAJIIg
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-systemd-timesyncd.service-H2hifi
drwx------  3 root       root        4096 окт 13 21:31 systemd-private-0e2074fce3c740b398bb16e3675acb93-systemd-resolved.service-qa4ysh
drwxrwxrwt  2 root       root        4096 окт 13 21:31 .font-unix
drwxrwxrwt  2 root       root        4096 окт 13 21:31 .Test-unix
drwxrwxrwt  2 root       root        4096 окт 13 21:31 .XIM-unix
```


## Pub/Sub channels:

### System events
System events channel

|  |  |
|--------------------------------|---| 
| Channel path                   | **swift.system.feed.events** |
| Message type                  |  [String](#resonse-types), [JSON object](#resonse-types) |



#### Message example:
```json
0, // event id ( 0 - module started, 1 - module restarted, 2 - module paused )
{"module_name": "modulename"}
```

### Logs
One log in each message

|  |  |
|--------------------------------|---| 
| Channel path                   | **swift.system.feed.logs** |
| Message type                  |  [VariantList](#resonse-types) |

#### Message example:
```
"Bittrex_API", # SENDER NAME
"INFO", # GROUP, - Debug,Info,Warning,Fatal
123123123123, # TITESTAMP
"MESSAGE EXAMPLE" # MESSAGE STRING
```

### Errors
One error in each message

|  |  |
|--------------------------------|---| 
| Channel path                   | **swift.system.feed.errors** |
| Message type                  |  [VariantList](#resonse-types) |

#### Message example:
```
"Bittrex_API", # SENDER NAME
"Warning", # GROUP, Debug,Info,Warning,Fatal
123123123123, # TITESTAMP
"ERROR EXAMPLE" # ERROR MESSAGE STRING
```

### Messages/Notifications
Messages for active users attention

|  |  |
|--------------------------------|---| 
| Channel path                   | **swift.system.feed.messages** |
| Message type                  |  [VariantList](#resonse-types) |

#### Message example:
```
"Notification title", # TITLE
"Notification text", # Text
```


# Assets
Getting and settings system data assets. 

## RPC commands
### Get assets
Get all system defined assets, such as enabled exchanges, currencies, coins, pairs, arbitrage_pairs and their params

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.assets.rpc.get** |
| Request params                 | **ASSET_NAME** |
| Request params possible values | **currencies,exchanges,coins,arbitrage_pairs,pairs** (Return all available assets by default)  |
| Request example                |  **swift.assets.rpc.get** |
| Response type                  |  [JSON object](#resonse-types) |
#### Response example:
```json5
{
   "currencies":{
      "1":{
         "coin_id":"2",
         "exchange_id":"1",
         "fee":"0.0420",
         "id":"1",
         "is_enabled":"1",
         "name":"ETH"
      },
      "10":{
         "coin_id":"2",
         "exchange_id":"10",
         "fee":"0.0100",
         "id":"10",
         "is_enabled":"1",
         "name":"ETH"
      }
   }
}
```


# Settings
User settings management system

## RPC commands

### Get param

Receive current parameter value 

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.assets.rpc.settings.get** |
| Request params                 | **PARAM_NAME_COMMA_SEPARATED (Return all available params in format PARAM_NAME=PARAM_VALUE)** |
| Request params possible values | **[VariantList](#resonse-types)** |
| Request example                |  **swift.assets.rpc.settings.get MYSQL_USERNAME** |
| Response type                  |  [VariantList](#resonse-types) |

#### Response example:
```
MYSQL_USERNAME=CURRENT_MYSQL_USERNAME_STRING
```


### Set param

Set new value for parameter 

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.assets.rpc.settings.set** |
| Request params                 | **COMMA_SEPARATED_STRING** |
| Request params possible values | **[VariantList](#resonse-types)** |
| Request example                |  **swift.assets.rpc.settings.set MYSQL_USERNAME=NEW_USERNAME** |
| Response type                  |  [Boolean](#resonse-types) |

#### Response example:
```
TRUE
```

## Pub/Sub channels

### swift.assets.feed.settings
Listen changes of params

|  |  |
|--------------------------------|---| 
| Channel path                   | **swift.assets.feed.settings** |
| Message type                  |  [VariantList](#resonse-types) |

#### Message example:
```
"PARAM_NAME", # Changed parameter name string
"NEW_VALUE", # String with new value
```


# Balances and accounts

Section describe how to work with exchanges accounts settings and balances information including deposits and withdraws

## RPC commands

### Get exchanges accounts params
Get exchanges API keys, API secrets and additional data ( e.g. username if it required by exchange API )

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.assets.rpc.accounts.get** |
| Request params                 | **None** |
| Request params possible values | **None**  |
| Request example                |  **swift.assets.rpc.accounts.get** |
| Response type                  |  [JSON array](#resonse-types) |
#### Response example:
```json5
[
{
      "api_key":"apikey",
      "api_secret":"apisecret",
      "api_username": "apiadditionalinfo",
      "exchange_id": 1,
      "exchange_name": "bittrex"    
},
{
      "api_key":"apikey",
      "api_secret":"apisecret",
      "api_username": "apiadditionalinfo",
      "exchange_id": 2,
      "exchange_name": "bitfinex"    
}
]
```

### Get exchanges trading fees
Get current information about exchanges trading fees

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.assets.rpc.fee.trade.get** |
| Request params                 | **None** |
| Request params possible values | **None**  |
| Request example                |  **swift.assets.rpc.fee.trade.get** |
| Response type                  |  [JSON array](#resonse-types) |
#### Response example:

```json5
[
]
```


### Get currencies withdraw fees
Get current information about currency withdraw fees

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.assets.rpc.fee.withdraw.get** |
| Request params                 | **None** |
| Request params possible values | **None**  |
| Request example                |  **swift.assets.rpc.fee.withdraw.get** |
| Response type                  |  [JSON array](#resonse-types) |

#### Response example:
```json5
[

]
```


### Set fees settings
Upade fees values


|  |  |
|--------------------------------|---| 
| Request path                   | **swift.assets.rpc.fee.set** |
| Request params                 | **Trading fees, Withdrawal fees** |
| Request params possible values | **JSON object, JSON object**  |
| Request example                |  **swift.assets.rpc.fee.set "{\"exchange_id\":\"1\", \"fee\":\"0.0018\"}","{\"currency_id\":\"1\", \"fee\":\"0.5\"}"** |
| Response type                  |  [Boolean](#resonse-types) |

#### Response example:
```
TRUE
```

# Currency deposit addresses management

### Get addresses
swift.assets.rpc.address.get

### Set address
swift.assets.rpc.address.set

# Balances

### Get currencies balances info

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.balance.rpc.get** |
| Request params                 | **CURRENCY_ID ( If not set - return array of all available balances)** |
| Request params possible values | **JSON array**  |
| Request example                |  **swift.balance.rpc.get 1,5,6** |
| Response type                  |  [JSON Array](#resonse-types) |

#### Response example:
```json5
[
   [
      [
         1, // Currency ID
         "0.02859955", // Available
         "0.02859955", // Total
         "9.72384700", // Amount in USD
         "0", // Coin id
         "1" // Exchange id
      ],
      [
         5,
         "0.36183409",
         "0.36183409",
         "123.02359060",
         "0",
         "5"
      ],
      [
         6,
         "0.40585160",
         "0.40585160",
         "137.98954400",
         "0",
         "6"
      ]
   ]
]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ]
```

### Get currencies balances info

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.balance.rpc.history** |
| Request params                 | **CURRENCY_ID ( If not set - return array of all available balances)** |
| Request params possible values | **JSON array**  |
| Request example                |  **swift.balance.rpc.history 1,5,6** |
| Response type                  |  [JSON Array](#resonse-types) |

#### Response example:
```json5
[]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ]
```

### Get required balances info

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.balance.rpc.required** |
| Request params                 | **CURRENCY_ID ( If not set - return array of all available balances)** |
| Request params possible values | **JSON array**  |
| Request example                |  **swift.balance.rpc.required 1,5,6** |
| Response type                  |  [JSON Array](#resonse-types) |

#### Response example:
```json5
[
   [
      [
         1, // Currency ID
         "0.02859955", // Required amount
         "9.72384700", // Required amount in USD
         "0", // Coin id
         "1" // Exchange id
      ],
      [
         5,
         "0.36183409",    
         "123.02359060",
         "0",
         "5"
      ],
      [
         6,
         "0.40585160",        
         "137.98954400",
         "0",
         "6"
      ]
   ]
]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ]
```



### Get deposits history

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.balance.rpc.deposits** |
| Request params                 | **CURRENCY_IDS,EXCHANGE_IDS** |
| Request params possible values | **JSON array, JSON array**  |
| Request example                |  **swift.balance.rpc.deposits [1,5,6], [1,2]** |
| Response type                  |  [JSON Array](#resonse-types) |

#### Response example:
```json5
[]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ]
```


### Get withdrawals history

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.balance.rpc.withdraws** |
| Request params                 | **CURRENCY_IDS,EXCHANGE_IDS** |
| Request params possible values | **JSON array, JSON array**  |
| Request example                |  **swift.balance.rpc.withdraws [1,5,6], [1,2]** |
| Response type                  |  [JSON Array](#resonse-types) |

#### Response example:
```json5
[]                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               ]
```



### Get deposit record information

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.balance.rpc.deposit.get ** |
| Request params                 | **RECORD_ID** |
| Request params possible values | **JSON object**  |
| Request example                |  **swift.balance.rpc.deposit.get 123** |
| Response type                  |  [JSON Array](#resonse-types) |

#### Response example:
```json5
{}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           ]
```

### Get withdraw record information

|  |  |
|--------------------------------|---| 
| Request path                   | **swift.balance.rpc.withdraw.get** |
| Request params                 | **RECORD_ID** |
| Request params possible values | **JSON object**  |
| Request example                |  **swift.balance.rpc.withdraw.get 123** |
| Response type                  |  [JSON Array](#resonse-types) |

#### Response example:
```json5
{}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        ]
```


## Pub/Sub channels

### Balance events channel
Information about balance events

|  |  |
|--------------------------------|---| 
| Channel path                   | **swift.balance.feed.events** |
| Message type                  |  [VariantList](#resonse-types) |

#### Message example:
```JSON
"UPDATE", # EVENT NAME (UPDATE,WITHDRAW,WITHDRAWS,DEPOSIT,DEPOSITS) 
{
  "exchange":"bittrex", # GROUP, - Debug,Info,Warning,Fatal
  "items":[{},{}], # DATA ITEMS
  "uuid":"122222222" # Unique request id
}
```


# Trading

### Get active orders
swift.trade.rpc.active
[EXCHANGE]

### Get trade history
swift.trade.rpc.history
[PAIR_ID, DATE]

### Get trading summary info
swift.trade.rpc.summary
[DATE]


### Get info about arbitrage window
swift.trade.rpc.window
[IDS]


### Get rates info
swift.trade.rpc.rates

## Pub/Sub channels

### Orders events channel

swift.orders.feed.events
[EVENT, JSONDATA]

EVENT
ERROR
PLACED
CANCELED
COMPLETED
UPDATED
HISTOTY
ACTIVE
JSONDATA
exchange
items
uuid

### Arbitrage windows data channel

swift.orders.feed.windows
[WID, JSONDATA]

### Arbitrage windows profitable channel

Канал с периодическим обновлением снимка всех профитных окон
swift.orders.feed.windows.snapshot
[JSONDATA]


# Unified exchange api interface

### API Client status
swift.api.rpc.0.status

### Make synchronous request to api and wait for result 
swift.api.rpc.await
[EXCHANGE, METHOD,JSONPARAMS]

### Get orders history

swift.api.rpc.0.trade.history

### Get active orders
swift.api.rpc.0.trade.active

### Create new limit order
swift.api.rpc.0.trade.place
[JSONPARAMS]

### Get trading fee
swift.api.rpc.0.trade.fee
[JSONPARAMS

### Cancel active order
swift.api.rpc.0.trade.cancel
[JSONPARAMS]


### Get info about order
swift.api.rpc.0.trade.order
[REMOTEID]


### Get balance info
swift.api.rpc.0.balance.get


### Get withdrawals history
swift.api.rpc.0.balance.withdraws

### Create new withdrawal
swift.api.rpc.0.balance.withdraw
{“currency_id”:”3”,”address”:””,”local_id”:”someanothertest”,”amount”:”0.12”}

### Get withdrawal limits
swift.api.rpc.0.balance.withdraw.limits

### Create new internal withdrawal
swift.api.rpc.0.balance.withdraw.inner

### Get deposits history
swift.api.rpc.0.balance.deposits

### Get deposit address
swift.api.rpc.0.balance.address

## Pub/Sub channels

### Async results of API methods call

swift.api.feed.results


## Usage and availability 

### Get api status info

swift.system.rpc.api.status

### Set result of method execution

swift.system.rpc.api.method [EXCHANGE,METHOD,RESULT]

# Modules watchdog

Every **n** second each module reporting self availability by publishing message in this channel.
If module does not report more than seconds from settings param, module will be force restarted.

|  |  |
|--------------------------------|---| 
| Channel path                   | **swift.system.feed.watchdog** |
| Message type                  |  [String](#resonse-types) |

#### Message example:
```
"api_nodes"
```

# Android Firebase

### Set new user token

swift.system.firebase.token [TOKEN]

### Send notification to all stored tokens

swift.system.firebase.notify [TITLE, MESSAGE]


# OrderBooks data

### Get cached orderbooks data
swiftbot.orderbooks.cache

### Get orderbooks proxy status
swift.orderbooks.rpc.status

### Exec custom shell command on orderbooks proxy host machine
swift.orderbooks.rpc.cmd

## Pub/Sub channels
### Main orderbooks channel

swift.orderbooks.feed




