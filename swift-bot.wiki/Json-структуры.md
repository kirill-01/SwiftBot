## Основные объекты

# JSON Пакеты 

### Markets

```json
{
  "exchange_id": "1",
  "name" : "BTC-USDT",
  "market_id" : "8",
  "base_currency_id" : "8",
  "market_currency_id" : "8",
  "trade_fee" : "0.00500000",
  "min_trade_size" : "0.01000000"
}
```

### Currencies

```json
{
  "exchange_id": "1",
  "name" : "BTC",
  "precision" : "8",
  "withdraw_enabled" : true,
  "withdraw_fee" : "0.00500000",
  "withdraw_min" : "0.01000000",
  "withdraw_limit_daily" : "10.00000000",
  "deposit_address" : "16BVE3UQ7s4uj5mhbKJao2SjDmMzgYhaxH"
}
```


### Orderbooks

12 - ID Пары

```json
{
  "a" : [
        [12, 7500,0.10000000],[12, 7500,0.10000000]
    ],
  "b" : [
        [12, 7500,0.10000000],[12, 7500,0.10000000]
    ]
}
```

### Balance

```json
{
  "currency_id": "1",
  "name" : "BTC",
  "total" : "8.00000000",
  "available" : "8.00000000",
  "in_orders" : "8.00000000",
  "reserved" : "8.00000000",
  "pending" : "8.00000000",
  "today_in_trades" : "0.00000000",
  "required" : "8.00000000"
}
```

### Deposits

```json
{
  "exchange_id": "1",
  "currency_id" : "1",
  "received_at" : "123323233",
  "confirmed_at" : "123323233",
  "status" : "pending",
  "remote_id" : "xxxxx",
  "local_id" : "xxxxx",
  "amount" : "10.00000000",
  "fee" : "0.00050000",
  "deposit_address" : "16BVE3UQ7s4uj5mhbKJao2SjDmMzgYhaxH"
}
```

### DepositAddress

```json
{
  "exchange_id": "1",
  "currency_id" : "1",
  "deposit_address" : "16BVE3UQ7s4uj5mhbKJao2SjDmMzgYhaxH"
}
```

### Withdraw

```json
{
  "exchange_id": "1",
  "currency_id" : "1",
  "created_at" : "123323233",
  "confirmed_at" : "123323233",
  "status" : "pending",
  "type" : "internal|external",
  "remote_id" : "xxxxx",
  "local_id" : "xxxxx",
  "amount" : "10.00000000",
  "fee" : "0.00050000",
  "deposit_address" : "16BVE3UQ7s4uj5mhbKJao2SjDmMzgYhaxH"
}
```

### WithdrawInner

```json
{
  "exchange_id": "1",
  "currency_id" : "1",
  "amount" : "1000.00000000",
  "from" : "main|trade",
  "to" : "main|trade"
}
```

### Order

```json
{
  "market_id" : "1",
  "remote_id" : "xxxxx",
  "local_id" : "xxxxx",
  "amount" : "10.00000000",
  "amount_left" : "10.00000000",
  "type" : "sell|buy",
  "rate" : "10.00000000",
  "price" : "0.00050000",
  "fee" : "0.00050000",
  "base_currency_id" : "1",
  "market_currency_id" : "2",
  "status" : "0-4",
  "created_at" : "123323233",
  "updated_at" : "123323233"
}
```

### ApiStateReport

```json
{
   "exchange_name" : "bittrex",
   "exchange_id" : "1",
   "started_at" : "12321323",
   "updated_at" : "123123123",
   "api_version" : "1.0",
   "current_rate" : "5000.000",
   "errors" : [
     {}
   ]
}
```
