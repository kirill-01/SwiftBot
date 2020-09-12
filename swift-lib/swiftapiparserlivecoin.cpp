#include "swiftapiparserlivecoin.h"
#include <QJsonDocument>

SwiftApiParserLivecoin::SwiftApiParserLivecoin(QObject *parent) : SwiftApiParser(parent)
{

}

QString SwiftApiParserLivecoin::getExchangeName() const
{
    return "livecoin";
}

void SwiftApiParserLivecoin::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    //qWarning() << data;
    const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
    QJsonObject j_result;
    j_result["async_uuid"] = QString::number( uuid );
    if ( j_resp.contains("success") && j_resp.value("success").toBool(false) == false ) {
        j_result["success"] = false;
        j_result["error"] = QString( QJsonDocument( j_resp ).toJson( QJsonDocument::Compact ) );
        emit resultParsed( uuid, j_result );
    } else {
       if ( method == SwiftApiClient::AsyncMethods::OrderPlace )  {

          // {"added":false,"exception":null,"orderId":100889220001,"success":true}

          if ( j_resp.value("success").toBool( false ) == true ) {

                j_result["remote_id"] = QString::number( j_resp.value("orderId").toVariant().toULongLong() );
              if ( j_resp.value("added").toBool( false ) == true ) {
                j_result["status"] = QString::number( 1 );
              } else {
                j_result["status"] = QString::number( 2 );
              }
          } else {
            j_result["status"] = QString::number( 3 );

          }
          j_result["success"] = j_resp.value("success");
          j_result["created_at"] = QString::number( QDateTime::currentSecsSinceEpoch() );
          emit resultParsed( uuid, j_result );
       } else if ( method == SwiftApiClient::AsyncMethods::OrderGet ) {
           j_result["remote_id"] = QString::number( j_resp.value("orderId").toInt() );
           j_result["status"] = j_resp.value("status").toString() == "CANCELED" ? QString::number( 3 ) : j_resp.value("status").toString() == "CLOSED" ? QString::number( 2 ) : j_resp.value("status").toString() == "OPEN" ? QString::number( 1 ) :  QString::number( 2 );
           j_result["rate"] =QString::number( j_resp.value("price").toDouble(), 'f', 8 );
           j_result["amount_left"] = QString::number( j_resp.value("quantity").toDouble(), 'f', 8 );
           j_result["amount"] =QString::number( j_resp.value("remaining_quantity").toDouble(), 'f', 8 );
           j_result["fee"] = QString::number( j_resp.value("commission_rate").toDouble(), 'f', 8 );
           j_result["success"] = true;
           emit resultParsed( uuid, j_result );
           /*{
               "id": 88504958,
               "client_id": 1150,
               "status": "CANCELLED",
               "symbol": "DASH/USD",
               "price": 1.5,
               "quantity": 1.2,
               "remaining_quantity": 1.2,
               "blocked": 1.8018,
               "blocked_remain": 0,
               "commission_rate": 0.001,
               "trades": null
           }*/
       } else if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
           if ( j_resp.value("cancelled").toBool(false) == true ) {
                j_result["status"] = QString::number( 3 );
                j_result["amount_left"] = QString::number( j_resp.value("quantity").toDouble(), 'f', 8 );
           }
           j_result["success"] = true;
           emit resultParsed( uuid, j_result );
         /*  {
               "success": true,
               "cancelled": true,
               "exception": null,
               "quantity": 0.0005,
               "tradeQuantity": 0
           }*/
       } else if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
           QHash<QString,double> _totals;
           QHash<QString,double> _available;
           QHash<QString,double> _orders;
           QHash<QString,double> _available_w;
           QJsonArray j_items( QJsonDocument::fromJson( data ).array() );
           for( auto it = j_items.begin(); it != j_items.end(); it++ ) {
               QJsonObject j( it->toObject() );
               if ( j.value("type").toString() == "total" ) {
                   _totals[ j.value("currency").toString() ] = j.value("value").toDouble();
               } else if ( j.value("type").toString() == "trade" ) {
                    _orders[ j.value("currency").toString() ] = j.value("value").toDouble();
               } else if ( j.value("type").toString() == "available" ) {
                    _available[ j.value("currency").toString() ] = j.value("value").toDouble();
               } else if ( j.value("type").toString() == "available_withdrawal" ) {
                   _available_w[ j.value("currency").toString() ] = j.value("value").toDouble();
              }
           }
           QJsonArray bals;
           for( auto it = _totals.begin(); it != _totals.end(); it++ ) {
               QJsonObject j_item;
               const QString currency_name( it.key()  );
               const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
               if ( currency_id > 0  ) {
                   j_item["currency_id"] = QString::number( currency_id );
                   j_item["name"] = currency_name;
                   j_item["total"] = QString::number( it.value(), 'f', 8 );
                   j_item["available"] = QString::number( _available.value(it.key(), 0), 'f', 8 );
                   j_item["available_w"] = QString::number( _available_w.value(it.key(), 0), 'f', 8 );
                   j_item["in_orders"] = QString::number( _orders.value(it.key(), 0), 'f', 8 );
                   j_item["reserved"] = QString::number( 0 );
                   j_item["pending"] = QString::number( 0, 'f', 8 );
                   j_item["today_in_trades"] = QString::number( 0 );
                   j_item["required"] = QString::number( 0 );
                   bals.push_back( j_item );
               }
           }
           QJsonObject j_ret;
           j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
           j_ret["balances"] = bals;
           emit resultParsed( uuid, j_ret );

       } else if ( method == SwiftApiClient::AsyncMethods::WithdrawCreate ) {
           QJsonObject j_ret;
           j_ret["success"] = j_resp.value("fault").isNull() ? true : false;
           j_ret["remote_id"] = j_resp.value("externalKey").toString();
           j_ret["status"] = j_resp.value("state").toString();
           j_ret["raw"] = j_resp;
           j_ret["deposit_address"] = j_resp.value("wallet").toString();
           j_ret["async_uuid"] = QString::number( uuid );
           emit resultParsed( uuid, j_ret );
       } else if ( method == SwiftApiClient::AsyncMethods::TradeGetFee ) {
           QJsonObject fees;
           fees["amount"] = QString::number( j_resp.value("last30DaysAmountAsUSD").toString().toDouble(), 'f', 8 );
           fees["fee"] = QString::number( j_resp.value("commission").toString().toDouble(), 'f', 8 );
           j_result["fees"] = fees;
           j_result["success"] = true;
           emit resultParsed( uuid, j_result );
       } else if ( method == SwiftApiClient::AsyncMethods::WithdrawList  || method == SwiftApiClient::AsyncMethods::WithdrawHistory ) {
           QJsonArray items( QJsonDocument::fromJson( data ).array() );
           QJsonArray ret;
           if ( !items.isEmpty() ) {
               for( auto it = items.begin(); it != items.end(); it++ ) {
                   const QJsonObject j_itm( it->toObject() );
                   QJsonObject j_dep_rec;
                   j_dep_rec["exchange_id"] = QString::number( getExchangeId() );
                   j_dep_rec["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( j_itm.value("fixedCurrency").toString(), getExchangeId() ) );
                   j_dep_rec["created_at"] = QString::number( j_itm.value("date").toVariant().toULongLong() / 1000 );
                   j_dep_rec["confirmed_at"] = QString::number( j_itm.value("date").toVariant().toULongLong() / 1000 );
                   j_dep_rec["status"] = "done";
                   j_dep_rec["remote_id"] = j_itm.value("externalKey").toString();
                   j_dep_rec["local_id"] = j_itm.value("externalKey").toString();
                   j_dep_rec["amount"] = QString::number( j_itm.value("amount").toDouble(), 'f', 8);
                   j_dep_rec["fee"] = QString::number( j_itm.value("fee").toDouble(), 'f', 8);
                   j_dep_rec["deposit_address"] = j_itm.value("id").toString();
                   ret.push_back(j_dep_rec);
               }
           }
           QJsonObject j_ret;
           j_ret["success"] = true;
           j_ret["raw"] = items;
           j_ret["async_uuid"] = QString::number( uuid );
           j_ret["withdraws"] = ret;
           emit resultParsed( uuid, j_ret );
       } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders ) {
           QJsonObject j_ret;
           j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
           QJsonArray j_orders;
           QJsonArray j_resp_orders( j_resp.value("data").toArray( ) );
           for( auto it = j_resp_orders.begin(); it != j_resp_orders.end(); it++ ) {
               const QJsonObject j_order( it->toObject() );
               QJsonObject j_item;
               if ( j_order.value("orderStatus").toString() != "PARTIALLY_FILLED_AND_CANCELLED" && j_order.value("orderStatus").toString() != "EXECUTED" && j_order.value("orderStatus").toString() != "CANCELLED" ) {
                   j_item["exchange_id"] = QString::number( getExchangeId() );
                   j_item["exchange_name"] = getExchangeName();
                   j_item["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_order.value("currencyPair").toString(), getExchangeId() ) );
                   j_item["created_at"] = QString::number(j_order.value("issueTime").toVariant().toULongLong() );
                   //j_item["closed_at"] =  j_curr.value("Closed").isNull() ? "" : QString::number( QDateTime::fromString( j_curr.value("Closed").toString(), Qt::ISODate ).toMSecsSinceEpoch() );
                   j_item["status"] = "1";
                   j_item["remote_id"] = QString::number( j_order.value("id").toVariant().toULongLong() );
                   j_item["price"] = QString::number( j_order.value("quantity").toDouble() * j_order.value("price").toDouble(), 'f',8 );
                   j_item["rate"] = QString::number( j_order.value("price").toDouble(), 'f', 8 );
                   j_item["amount"] = QString::number( j_order.value("quantity").toDouble(), 'f', 8 );
                   j_item["amount_left"] = QString::number( j_order.value("remainingQuantity").toDouble(), 'f', 8 );
                   j_item["fee"] = QString::number( j_order.value("commissionByTrade").toDouble(), 'f', 8 );
                   j_item["type"] =  j_order.value("type").toString() == "LIMIT_SELL" ? "sell" : "buy";
                   j_orders.push_back( j_item );
               }
           }
           j_ret["orders"] = j_orders;
           emit resultParsed( uuid, j_ret );
       } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory ) {
           QJsonObject j_ret;
           j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
           QJsonArray j_orders;
           QJsonArray j_resp_orders( j_resp.value("data").toArray( ) );
           for( auto it = j_resp_orders.begin(); it != j_resp_orders.end(); it++ ) {
               const QJsonObject j_order( it->toObject() );
               QJsonObject j_item;
               if ( j_order.value("orderStatus").toString() == "EXECUTED" ) {
                   j_item["exchange_id"] = QString::number( getExchangeId() );
                   j_item["exchange_name"] = getExchangeName();
                   j_item["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_order.value("currencyPair").toString(), getExchangeId() ) );
                   j_item["created_at"] = QString::number(j_order.value("issueTime").toVariant().toULongLong() );
                   j_item["closed_at"] = QString::number(j_order.value("lastModificationTime").toVariant().toULongLong() );
                   j_item["status"] = "2";
                   j_item["remote_id"] = QString::number( j_order.value("id").toVariant().toULongLong() );
                   j_item["rate"] = QString::number( j_order.value("price").toDouble(), 'f', 8 );
                   j_item["price"] = QString::number( j_order.value("quantity").toDouble() * j_order.value("price").toDouble(), 'f',8 );
                   j_item["amount"] = QString::number( j_order.value("quantity").toDouble(), 'f', 8 );
                   j_item["amount_left"] = QString::number( j_order.value("remainingQuantity").toDouble(), 'f', 8 );
                   j_item["fee"] = QString::number( j_order.value("commissionByTrade").toDouble(), 'f', 8 );
                   j_item["type"] =  j_order.value("type").toString() == "LIMIT_SELL" ? "sell" : "buy";
                   j_orders.push_back( j_item );
               } else if ( j_order.value("orderStatus").toString() == "CANCELED" || j_order.value("orderStatus").toString() == "PARTIALLY_FILLED_AND_CANCELLED"  ) {
                   j_item["exchange_id"] = QString::number( getExchangeId() );
                   j_item["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_order.value("currencyPair").toString(), getExchangeId() ) );
                   j_item["created_at"] = QString::number(j_order.value("issueTime").toVariant().toULongLong() );
                   j_item["closed_at"] = QString::number(j_order.value("lastModificationTime").toVariant().toULongLong() );
                   j_item["status"] = "3";
                   j_item["remote_id"] = QString::number( j_order.value("id").toVariant().toULongLong() );
                   j_item["rate"] = QString::number( j_order.value("price").toDouble(), 'f', 8 );
                   j_item["price"] = QString::number( j_order.value("quantity").toDouble() * j_order.value("price").toDouble(), 'f',8 );
                   j_item["amount"] = QString::number( j_order.value("quantity").toDouble(), 'f', 8 );
                   j_item["amount_left"] = QString::number( j_order.value("remainingQuantity").toDouble(), 'f', 8 );
                   j_item["fee"] = QString::number( j_order.value("commissionByTrade").toDouble(), 'f', 8 );
                   j_item["type"] =  j_order.value("type").toString() == "LIMIT_SELL" ? "sell" : "buy";
                   j_orders.push_back( j_item );
               }
           }
           j_ret["orders"] = j_orders;
           emit resultParsed( uuid, j_ret );
       } else if ( method == SwiftApiClient::AsyncMethods::GetDeposits ) {
           QJsonArray items( QJsonDocument::fromJson( data ).array() );
           QJsonArray ret;
           if ( !items.isEmpty() ) {
               for( auto it = items.begin(); it != items.end(); it++ ) {
                   const QJsonObject j_itm( it->toObject() );
                   QJsonObject j_dep_rec;
                   j_dep_rec["exchange_id"] = QString::number( getExchangeId() );
                   j_dep_rec["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( j_itm.value("fixedCurrency").toString(), getExchangeId() ) );
                   j_dep_rec["created_at"] = QString::number( j_itm.value("date").toVariant().toULongLong() / 1000 );
                   j_dep_rec["confirmed_at"] = QString::number( j_itm.value("date").toVariant().toULongLong() / 1000 );
                   j_dep_rec["status"] = "done";
                   j_dep_rec["remote_id"] = j_itm.value("id").toString();
                   j_dep_rec["local_id"] = "";
                   j_dep_rec["amount"] = QString::number( j_itm.value("amount").toDouble(), 'f', 8);
                   j_dep_rec["fee"] = QString::number( j_itm.value("fee").toDouble(), 'f', 8);
                   j_dep_rec["deposit_address"] = "";
                   ret.push_back(j_dep_rec);
               }
           }
           QJsonObject j_ret;
           j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
           j_ret["deposits"] = ret;
           emit resultParsed( uuid, j_ret );
       } else if ( method == SwiftApiClient::AsyncMethods::GetDepositAddress ) {
           QJsonObject j_ret;
           QJsonArray j_addrs;
           for ( auto it = j_resp.begin(); it != j_resp.end(); it++ ) {
               QJsonObject j_itm;
               j_itm["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( it.key(), getExchangeId() ) );
               j_itm["address"] = it.value();
               j_addrs.push_back( j_itm );
           }
           j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
           j_ret["data"] = QString(data);
           emit resultParsed( uuid, j_ret );
       } else {

           //qWarning() << data;
       }
    }
}
