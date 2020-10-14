# System configuration

## Configuration params

Configuration file located in path: **/opt/swiftbot/config.conf**

### Config file content example
```ini
[General]
API_DEBUG=0
ETHUSDPAIR=1
EXCHANGES=""
MIN_ORDER_SIZE=0.1
MIN_PROFIT_SIZE=0.25
MIN_WINDOW_SPREAD=0.05
MYSQL_DBNAME=example
MYSQL_HOST=example.cc.finance
MYSQL_PORT=3306
MYSQL_USERNAME=root
MYSQL_USERPASSWORD=swiftbot
ORDERBOOKS_CHECK_INTERVAL=500
USECLASSIC=0
USERNAME=swiftbot
USESTATISTIC=1
WAMP_DEBUG=0
WAMP_PORT=8081
WAMP_PORT_APINODES=8082
WAMP_PORT_BOT=8095
WAMP_PORT_ORDERBOOKS=8081
WAMP_PORT_ORDERBOOKS_READER=8085
WAMP_PORT_WORKERS_BALANCES=8093
WAMP_PORT_WORKERS_LOGS=8094
WAMP_PORT_WORKERS_ORDERS=8095
WAMP_PORT_WORKERS_SETTINGS=8091
WAMP_PORT_WORKERS_SETTINGSHOST=8096
WAMP_PORT_WORKERS_USERHISTORY=8092
WAMP_REALM=swiftbot
WAMP_RECONNECT_LIMIT=10
WAMP_RECONNECT_TIMEOUT_SECS=2
WAMP_SERVER=example.cc.finance
```

### API_DEBUG
#### Default value: "0"
If API debugging is enabled (1) - system will send a lot of debugging information from API client.
___

### ETHUSDPAIR
#### Default value: "0"
Enable ETH / USDT pair.
___
 
### BTCUSDPAIR
#### Default value: "0"
Enable BTC / USDT pair.  
___

### EXCHANGES
#### Default value: ""
List of enabled exchanges
___   

### MIN_ORDER_SIZE
#### Default value: "0.1"
Minimum order size amount
___

### MIN_PROFIT_SIZE
#### Default value: "0.25"
Minimum profit to place orders
___

### MIN_WINDOW_SPREAD
#### Default value: "0.05"
Minimum window spread to proceed calculation
___

### MYSQL_DBNAME
#### Default value: "swiftbot"
MySQL server database name
___

### MYSQL_HOST
#### Default value: "localhost"
MySQL server hostname
___

### MYSQL_PORT
#### Default value: "3306"
MySQL server port
___

### MYSQL_USERNAME
#### Default value: "swiftbot"
MySQL server username
___

### MYSQL_USERPASSWORD
#### Default value: "swiftbot"
MySQL server user password
___

### ORDERBOOKS_CHECK_INTERVAL
#### Default value: "500"
Interval to recalculate windows by available orderbooks
___

### USECLASSIC
#### Default value: "0"
Use classic arbitrage strategy
___

### USESTATISTIC
#### Default value: "1"
Use statistical arbitrage strategy
___

### WAMP_DEBUG
#### Default value: "0"
Send to logs WAMP debugging information
___

### WAMP_PORT
#### Default value: "8081"
WAMP default main port
___

### WAMP_PORT_APINODES
#### Default value: "8082"
WAMP port for API NODES connection
___

### WAMP_PORT_BOT
#### Default value: "8095"
WAMP port for BOT connection
___

### WAMP_PORT_ORDERBOOKS
#### Default value: "8081"
WAMP port for ORDERBOOKS connection
___

### WAMP_PORT_ORDERBOOKS_READER
#### Default value: "8085"
WAMP port for API ORDERBOOKS READER connection
___

### WAMP_PORT_WORKERS_BALANCES
#### Default value: "8093"
WAMP port for BALANCES MODULE connection
___

### WAMP_PORT_WORKERS_LOGS
#### Default value: "8094"
WAMP port for LOGS MODULE connection
___

### WAMP_PORT_WORKERS_ORDERS
#### Default value: "8095"
WAMP port for ORDERS MODULE connection
___

### WAMP_PORT_WORKERS_SETTINGS
#### Default value: "8091"
WAMP port for SETTINGS MODULE connection
___

### WAMP_REALM
#### Default value: "swiftbot"
WAMP realm name
___

### WAMP_RECONNECT_LIMIT
#### Default value: "10"
WAMP connection retry attemps count
___

### WAMP_RECONNECT_TIMEOUT_SECS
#### Default value: "2"
WAMP reconnect timeout in seconds
___

### WAMP_SERVER
#### Default value: "localhost"
WAMP server name (hostname or address)
___


## SWIFT.bot - [Back to home](HomePage.md)