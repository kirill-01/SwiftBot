#include "swiftapiparserbinance.h"


QString SwiftApiParserBinance::getExchangeName() const
{
    return "binance";
}

void SwiftApiParserBinance::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    QJsonObject j_ret;
    j_ret["async_uuid"] = QString::number( uuid );
    j_ret["success"] = true;

    const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
    if ( j_resp.contains("code") && j_resp.contains("msg") ) {
        j_ret["success"] = false;
        j_ret["error"] = QString( j_resp.value("code").toString()+" : "+j_resp.value("msg").toString());
        emit resultParsed( uuid, j_ret );
    } else if ( j_resp.contains("msg") && j_resp.value("success").toBool(true) == false ) {
        j_ret["success"] = false;
        j_ret["error"] = QString( j_resp.value("msg").toString() );
        emit resultParsed( uuid, j_ret );
    }
    else {
        if ( method == SwiftApiClient::AsyncMethods::GetOrderbooks ) {

        } else if ( method == SwiftApiClient::AsyncMethods::GetMarkets ) {

        } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["created_at"] = QString::number( QDateTime::currentMSecsSinceEpoch() );
            j_ret["exchange_id"] = QString::number( getExchangeId() );
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["amount_left"] = QString::number( j_resp.value("origQty").toString().toDouble() - j_resp.value("executedQty").toString().toDouble(), 'f', 8 );
            j_ret["remote_id"] = QString::number( j_resp.value("orderId").toVariant().toUInt( ) );
            j_ret["local_id"] = j_resp.value("clientOrderId").toString();
            j_ret["status"] = parseStatus( j_resp.value("status").toString() );
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["exchange_id"] = QString::number( getExchangeId() );
            j_ret["remote_id"] = QString::number( j_resp.value("orderId").toVariant().toULongLong() );
            j_ret["local_id"] = j_resp.value("clientOrderId").toString();
            j_ret["status"] = "3";
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderGet) {
            QJsonObject j_curr(  j_resp );
            QJsonObject j_ret;
            j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
           const QString market_name( j_curr.value("symbol").toString()  );
           const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
               j_ret["exchange_id"] = QString::number( getExchangeId() );
               j_ret["market_id"] = QString::number( market_id );
               j_ret["created_at"] = QString::number( j_curr.value("time").toVariant().toULongLong() );
               j_ret["closed_at"] =  QString::number( j_curr.value("updateTime").toVariant().toULongLong() );
               j_ret["status"] = parseStatus( j_curr.value("status").toString() );
               j_ret["local_id"] = j_curr.value("clientOrderId").toString();
               j_ret["remote_id"] =  QString::number( j_curr.value("orderId").toVariant().toLongLong() );
               j_ret["price"] = QString::number( j_curr.value("price").toString().toDouble() * j_curr.value("origQty").toString().toDouble(), 'f', 8 );
               j_ret["rate"] = QString::number( j_curr.value("price").toString().toDouble(), 'f', 8 );
               j_ret["amount"] = QString::number( j_curr.value("origQty").toString().toDouble(), 'f', 8 );
               j_ret["amount_left"] = QString::number( j_curr.value("origQty").toString().toDouble() - j_curr.value("executedQty").toString().toDouble(), 'f', 8 );
               j_ret["fee"] = QString::number( SwiftLib::getExchangeFee( getExchangeId() ), 'f', 8 );
               j_ret["type"] =  j_curr.value("side").toString().toLower();

            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders ) {
            QJsonArray j_objects( QJsonDocument::fromJson( data ).array() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString market_name( j_curr.value("symbol").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
                if ( market_id > 0  ) {
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["market_id"] = QString::number( market_id );
                    j_item["created_at"] = QString::number( j_curr.value("time").toVariant().toULongLong() );
                    j_item["updated_at"] = QString::number( j_curr.value("time").toVariant().toULongLong() );
                    j_item["status"] = parseStatus( j_curr.value("status").toString() );
                    j_item["local_id"] = j_curr.value("clientOrderId").toString();
                    j_item["remote_id"] =  QString::number( j_curr.value("orderId").toVariant().toUInt() );
                    j_item["price"] = QString::number( j_curr.value("price").toString().toDouble() * j_curr.value("origQty").toString().toDouble(), 'f', 8 );
                    j_item["rate"] = QString::number( j_curr.value("price").toString().toDouble(), 'f', 8 );
                    j_item["amount"] = QString::number( j_curr.value("origQty").toString().toDouble(), 'f', 8 );
                    j_item["amount_left"] = QString::number( j_curr.value("origQty").toString().toDouble() - j_curr.value("executedQty").toString().toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( SwiftLib::getExchangeFee( getExchangeId() ), 'f', 8 );
                    j_item["type"] =  j_curr.value("side").toString().toLower();
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["orders"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory ) {
            QJsonArray j_objects( QJsonDocument::fromJson( data ).array() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString market_name( j_curr.value("symbol").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
                if ( parseStatus( j_curr.value("status").toString() ) != "1" ) {
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["market_id"] = QString::number( market_id );
                    j_item["created_at"] = QString::number( j_curr.value("time").toVariant().toULongLong() );
                    j_item["updated_at"] = QString::number( j_curr.value("updateTime").toVariant().toULongLong() );
                    j_item["status"] = parseStatus( j_curr.value("status").toString() );
                    j_item["local_id"] = j_curr.value("clientOrderId").toString();
                    j_item["remote_id"] =  QString::number( j_curr.value("orderId").toVariant().toUInt() );
                    j_item["price"] = QString::number( j_curr.value("price").toString().toDouble() * j_curr.value("origQty").toString().toDouble(), 'f', 8 );
                    j_item["rate"] = QString::number( j_curr.value("price").toString().toDouble(), 'f', 8 );
                    j_item["amount"] = QString::number( j_curr.value("origQty").toString().toDouble(), 'f', 8 );
                    j_item["amount_left"] = QString::number( j_curr.value("origQty").toString().toDouble() - j_curr.value("executedQty").toString().toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( SwiftLib::getExchangeFee( getExchangeId() ), 'f', 8 );
                    j_item["type"] =  j_curr.value("side").toString().toLower();
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["orders"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::GetCurrencies ) {
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray currs;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject itm;
                itm["exchange_id"] = QString::number( getExchangeId() );
                itm["name"] = j_curr.value("Currency").toString();
                itm["precision"] = QString::number( 8 );
                itm["withdraw_enabled"] = true;
                itm["withdraw_fee"] = QString::number(j_curr.value("TxFee").toDouble(),'f',8 );
                itm["withdraw_min"] = QString::number( 0.1, 'f',8 );
                itm["withdraw_limit_daily"] = QString::number( 0 );
                itm["deposit_address"] = "";
                currs.push_back( itm );
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["currencies"] = currs;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
            QJsonArray j_objects(  j_resp.value("balances").toArray() );
            QJsonArray bals;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString currency_name( j_curr.value("asset").toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["name"] = currency_name;
                    j_item["total"] = QString::number( j_curr.value("free").toString().toDouble() + j_curr.value("locked").toString().toDouble(), 'f', 8 );
                    j_item["available"] = QString::number( j_curr.value("free").toString().toDouble(), 'f', 8 );
                    j_item["in_orders"] =QString::number( j_curr.value("locked").toString().toDouble(), 'f', 8 );
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
        } else if ( method == SwiftApiClient::AsyncMethods::GetDeposits ) {
            const QJsonArray j_objects( j_resp.value("depositList").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString currency_name( j_curr.value("asset").toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["created_at"] = QString::number( j_curr.value("insertTime").toVariant().toULongLong() / 1000 );
                    j_item["confirmed_at"] =  QString::number( j_curr.value("insertTime").toVariant().toULongLong() / 1000 );
                    j_item["status"] = j_curr.value("status").toInt() == 1 ? "ok" : "pending";
                    j_item["remote_id"] = j_curr.value("txId").toString();
                    j_item["local_id"] = j_curr.value("txId").toString();
                    j_item["amount"] = QString::number( j_curr.value("amount").toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( 0 );
                    j_item["tx"] = j_curr.value("txId").toString();
                    QString address(j_curr.value("address").toString() );
                    if ( j_curr.value("addressTag").toString() != "" ) {
                        address += ":"+j_curr.value("addressTag").toString();
                    }
                    j_item["deposit_address"] = address;
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["deposits"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::WithdrawList || method == SwiftApiClient::AsyncMethods::WithdrawHistory ) {
            QJsonArray j_objects( j_resp.value("withdrawList").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );

                const QString currency_name( j_curr.value("asset").toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    QJsonObject j_item;
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["created_at"] = QString::number( j_curr.value("applyTime").toVariant().toULongLong() / 1000 );
                    // j_item["confirmed_at"] = QString::number( j_curr.value("applyTime").toVariant().toULongLong()  / 1000 );
                    j_item["status"] = j_curr.value("status").toInt() == 6 ? "completed" : "pending";
                    j_item["remote_id"] =  j_curr.value("id").toString();
                    j_item["local_id"] = j_curr.value("id").toString();
                    j_item["amount"] = QString::number( j_curr.value("amount").toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( j_curr.value("transactionFee").toDouble(), 'f', 8 );
                    j_item["tx"] = j_curr.value("txId").toString();
                    j_item["deposit_address"] =  j_curr.value("address").toString();
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["withdraws"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::GetDepositAddress ) {

        } else if ( method == SwiftApiClient::AsyncMethods::WithdrawGetFee ) {
            QJsonArray j_objects( QJsonDocument::fromJson( data ).array() );
            QJsonArray items;
            for ( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QString cname(  it->toObject().value("coin").toString() );
                const quint32 cid = SwiftLib::getAssets()->getCurrencyIdByName(cname, getExchangeId() );
                if ( cid > 0 ) {
                   QJsonObject j_it;
                   QJsonArray networks( it->toObject().value("networkList").toArray() );
                   if ( !networks.isEmpty() ) {
                       for( auto i2 = networks.begin(); i2 != networks.end(); i2++ ) {
                           if ( i2->toObject().value("withdrawEnable").toBool(false) ) {
                               QJsonObject itm;
                               itm["id"] = QString::number( cid );
                               itm["exchange_id"] = QString::number( getExchangeId() );
                               itm["name"] = cname;
                               itm["withdraw_fee"] = QString::number( i2->toObject().value("withdrawFee").toString().toDouble(),'f',8 );
                               items.push_back( itm );
                           }

                       }
                   }
                }
            }
           j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
           j_ret["currencies"] = items;
           emit resultParsed( uuid, j_ret );
       }  else if ( method == SwiftApiClient::AsyncMethods::TradeGetFee ) {
             QJsonArray j_objects( j_resp.value("tradeFee").toArray() );
             QJsonArray items;
             for ( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                 const quint32 pid = SwiftLib::getAssets()->getMarketIdByName( it->toObject().value("symbol").toString(), getExchangeId() );
                 if ( pid > 0 ) {
                    QJsonObject j_it;
                    j_it["id"] = QString::number( pid );
                    j_it["fee"] = QString::number( it->toObject().value("taker").toDouble(), 'f', 4 );
                    items.push_back( j_it );
                 }
             }
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["fees"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::WithdrawCreate ) {
            QJsonObject j_curr(  j_resp.value("result").toObject() );
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["remote_id"] = j_curr.value("uuid").toString();
            emit resultParsed( uuid, j_ret );
        }
        else {
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["error"] = "UNKNOWN METHOD PARSING"+QString::number( method );
            emit resultParsed( uuid, j_ret );
        }

    }
}

QString SwiftApiParserBinance::parseStatus(const QString &stat) {
    if ( stat == "NEW" ) {
        return "1";
    } else if ( stat == "PARTIALLY_FILLED" ) {
        return "2";
    } else if ( stat == "FILLED" ) {
        return "2";
    } else if ( stat == "CANCELED" ) {
        return "3";
    } else if ( stat == "REJECTED" ) {
        return "4";
    } else if ( stat == "NEWEXPIRED" ) {
        return "1";
    } else {
        return  "4";
    }
}
