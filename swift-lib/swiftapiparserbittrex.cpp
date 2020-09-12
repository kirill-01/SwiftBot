#include "swiftapiparserbittrex.h"
#include <QTimeZone>

SwiftApiParserBittrex::SwiftApiParserBittrex(QObject *parent) : SwiftApiParser(parent)
{

}


QString SwiftApiParserBittrex::getExchangeName() const
{
    return "bittrex";
}

void SwiftApiParserBittrex::parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    if ( method == SwiftApiClient::AsyncMethods::GetCurrencies ) {
        QJsonArray j_objects( QJsonDocument::fromJson( data ).array() );
        QJsonArray currs;
        for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
            const QJsonObject j_curr( it->toObject() );
            const quint32 cid = SwiftLib::getAssets()->getCurrencyIdByName( j_curr.value("symbol").toString(), getExchangeId() );
            if ( cid > 0 ) {
                QJsonObject itm;
                itm["id"] = QString::number( cid );
                itm["exchange_id"] = QString::number( getExchangeId() );
                itm["name"] = j_curr.value("symbol").toString();
                itm["withdraw_fee"] = QString::number(j_curr.value("txFee").toString().toDouble(),'f',8 );
                currs.push_back( itm );
            }

        }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["currencies"] = currs;
        emit resultParsed( uuid, j_ret );
        return;
    }
    const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
    if ( j_resp.contains("success") &&  j_resp.value("success").toBool(false) == false ) {
            QJsonObject ret;
            ret["success"] = false;
            ret["async_uuid"] = QString::number( uuid );
            ret["error"] = j_resp.value("message").toString();
            emit resultParsed( uuid,  ret );

    } else {
        if ( method == SwiftApiClient::AsyncMethods::GetOrderbooks ) {
            const QJsonObject response( j_resp.value("result").toObject() );
            const QJsonArray j_bids = response.value("buy").toArray();
            const QJsonArray j_asks = response.value("sell").toArray();
            if ( j_bids.isEmpty() && j_asks.isEmpty() ) {
                return;
            }
            QJsonArray::const_iterator _ai;
            QJsonArray asks;
            QJsonArray bids;
            for( auto _ai = j_asks.begin(); _ai != j_asks.end(); _ai++ ) {
                QJsonArray item;
                item.push_back( QString::number( 4 ) );
                item.push_back( QString::number( _ai->toObject().value("Rate").toDouble(), 'f', 8 ) );
                item.push_back( QString::number( _ai->toObject().value("Quantity").toDouble(), 'f', 8 ) );
                asks.push_back( item );
            }
            for( auto _ai = j_bids.begin(); _ai != j_bids.end(); _ai++ ) {
                QJsonArray item;
                item.push_back( QString::number( 4 ) );
                item.push_back( QString::number( _ai->toObject().value("Rate").toDouble(), 'f', 8 ) );
                item.push_back( QString::number( _ai->toObject().value("Quantity").toDouble(), 'f', 8 ) );
                bids.push_back( item );
            }
            QJsonObject j_ret;
            j_ret["a"] = asks;
            j_ret["b"] = bids;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::GetMarkets ) {
            QJsonObject j_ret;
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                const QString market_name( j_curr.value("MarketName").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName(market_name, getExchangeId() );
                const quint32 mcid = SwiftLib::getAssets()->getCurrencyIdByName( j_curr.value("BaseCurrency").toString(), getExchangeId() );
                const quint32 bcid = SwiftLib::getAssets()->getCurrencyIdByName( j_curr.value("MarketCurrency").toString(), getExchangeId() );
                if ( market_id > 0  && bcid > 0 && mcid > 0) {
                    QJsonObject itm;
                    itm["exchange_id"] = QString::number( getExchangeId() );
                    itm["name"] =  market_name;
                    itm["market_id"] = QString::number( market_id );
                    itm["base_currency_id"] = QString::number( bcid );
                    itm["market_currency_id"] = QString::number( mcid );
                    itm["trade_fee"] = QString::number( 0.0003, 'f', 8 );
                    itm["min_trade_size"] = QString::number(  j_curr.value("MinTradeSize").toDouble(), 'f', 8 );
                    items.push_back( itm );
                }
            }
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["markets"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["created_at"] = QString::number( QDateTime::currentSecsSinceEpoch() );
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["remote_id"] = j_resp.value("result").toObject().value("uuid");
            j_ret["status"] = "1";
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["remote_id"] = j_resp.value("result").toObject().value("uuid");
            j_ret["status"] = "3";
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderGet) {
            QJsonObject j_curr(  j_resp.value("result").toObject() );
            QJsonObject j_ret;
            j_ret["success"] = true;
           j_ret["async_uuid"] = QString::number( uuid );
            const QString market_name( j_curr.value("Exchange").toString()  );
            const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
                j_ret["exchange_id"] = QString::number( getExchangeId() );
                j_ret["market_id"] = QString::number( market_id );
                j_ret["created_at"] = QString::number( QDateTime::fromString( j_curr.value("Opened").toString(), Qt::ISODate ).toMSecsSinceEpoch() );
                j_ret["closed_at"] =  QString::number( QDateTime::fromString( j_curr.value("Closed").toString(), Qt::ISODate ).toMSecsSinceEpoch() );
                j_ret["status"] = j_curr.value("Closed").isNull() ? "1" : "2";
                j_ret["remote_id"] =  j_curr.value("OrderUuid").toString();
                j_ret["price"] = QString::number(  j_curr.value("Limit").toDouble() * j_curr.value("Quantity").toDouble(), 'f', 8 );
                j_ret["rate"] = QString::number( j_curr.value("Limit").toDouble(), 'f', 8 );
                j_ret["amount"] = QString::number( j_curr.value("Quantity").toDouble(), 'f', 8 );
                j_ret["amount_left"] = QString::number( j_curr.value("QuantityRemaining").toDouble(), 'f', 8 );
                j_ret["fee"] = QString::number( j_curr.value("CommissionPaid").toDouble(), 'f', 8 );
                j_ret["type"] =  j_curr.value("Type").toString() == "LIMIT_SELL" ? "sell" : "buy";
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders ) {
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString market_name( j_curr.value("Exchange").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
                if ( market_id > 0  ) {
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["market_id"] = QString::number( market_id );
                    j_item["created_at"] = QString::number( QDateTime::fromString( j_curr.value("Opened").toString(),Qt::ISODate).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["closed_at"] =  j_curr.value("Closed").isNull() ? "" : QString::number( QDateTime::fromString( j_curr.value("Closed").toString(), Qt::ISODate ).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["status"] = j_curr.value("Closed").isNull() ? "1" : "2";
                    j_item["remote_id"] =  j_curr.value("OrderUuid").toString();
                    j_item["price"] = QString::number( j_curr.value("Price").toDouble(), 'f', 8 );
                    j_item["rate"] = QString::number( j_curr.value("Limit").toDouble(), 'f', 8 );
                    j_item["amount"] = QString::number( j_curr.value("Quantity").toDouble(), 'f', 8 );
                    j_item["amount_left"] = QString::number( j_curr.value("QuantityRemaining").toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( j_curr.value("CommissionPaid").toDouble(), 'f', 8 );
                    j_item["type"] =  j_curr.value("OrderType").toString() == "LIMIT_SELL" ? "sell" : "buy";
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["orders"] = items;
             emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory ) {
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString market_name( j_curr.value("Exchange").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["market_id"] = QString::number( market_id );
                    j_item["created_at"] = QString::number( QDateTime::fromString( j_curr.value("Closed").toString(),Qt::ISODate ).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["closed_at"] =  j_curr.value("Closed").isNull() ? "" : QString::number( QDateTime::fromString( j_curr.value("Closed").toString(),Qt::ISODate  ).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["status"] = j_curr.value("Closed").isNull() ? "1" : "2";
                    j_item["remote_id"] =  j_curr.value("OrderUuid").toString();
                    j_item["price"] = QString::number( j_curr.value("Price").toDouble(), 'f', 8 );
                    j_item["rate"] = QString::number( j_curr.value("PricePerUnit").toDouble(), 'f', 8 );
                    j_item["amount"] = QString::number( j_curr.value("Quantity").toDouble(), 'f', 8 );
                    j_item["amount_left"] = QString::number( j_curr.value("QuantityRemaining").toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( j_curr.value("CommissionPaid").toDouble(), 'f', 8 );
                    j_item["type"] =  j_curr.value("OrderType").toString() == "LIMIT_SELL" ? "sell" : "buy";
                    items.push_back( j_item );
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["orders"] = items;
             emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray bals;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString currency_name( j_curr.value("Currency").toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["name"] = currency_name;
                    j_item["total"] = QString::number( j_curr.value("Balance").toDouble(), 'f', 8 );
                    j_item["available"] = QString::number( j_curr.value("Available").toDouble(), 'f', 8 );
                    j_item["in_orders"] = QString::number( j_curr.value("Balance").toDouble() - j_curr.value("Available").toDouble(), 'f', 8 );
                    j_item["reserved"] = QString::number( 0 );
                    j_item["pending"] = QString::number( j_curr.value("Pending").toDouble(), 'f', 8 );
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
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString currency_name( j_curr.value("Currency").toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["created_at"] = QString::number( QDateTime::fromString( j_curr.value("LastUpdated").toString(), Qt::ISODate ).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["confirmed_at"] = QString::number( QDateTime::fromString( j_curr.value("LastUpdated").toString(), Qt::ISODate ).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["status"] = j_curr.value("Confirmations").toInt() >= 2 ? "ok" : "pending";
                    j_item["remote_id"] =  QString::number( j_curr.value("Id").toInt() );
                    j_item["local_id"] = QString::number( 0 );
                    j_item["amount"] = QString::number( j_curr.value("Amount").toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( 0 );
                    j_item["tx"] = j_curr.value("TxId").toString();
                    j_item["deposit_address"] =  j_curr.value("CryptoAddress").toString();
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["deposits"] = items;
            emit resultParsed( uuid, j_ret );
        } else if (method == SwiftApiClient::AsyncMethods::WithdrawList  || method == SwiftApiClient::AsyncMethods::WithdrawHistory ) {
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );

                const QString currency_name( j_curr.value("Currency").toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    QJsonObject j_item;
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["created_at"] = QString::number( QDateTime::fromString( j_curr.value("Opened").toString(), Qt::ISODate ).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch()  );
                    j_item["confirmed_at"] = QString::number( QDateTime::fromString( j_curr.value("Opened").toString(), Qt::ISODate ).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["status"] = j_curr.value("Authorized").toBool() == true ? ( j_curr.value("PendingPayment").toBool() == true ? "sending" : "completed" ) : "pending";
                    j_item["remote_id"] =  j_curr.value("PaymentUuid").toString();
                    j_item["local_id"] = QString::number( 0 );
                    j_item["amount"] = QString::number( j_curr.value("Amount").toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( j_curr.value("TxCost").toDouble(), 'f', 8 );
                    j_item["tx"] = j_curr.value("TxId").toString();
                    j_item["deposit_address"] =  j_curr.value("Address").toString();
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["withdraws"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::WithdrawHistory ) {
            QJsonArray j_objects( j_resp.value("result").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString currency_name( j_curr.value("Currency").toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["created_at"] = QString::number( QDateTime::fromString( j_curr.value("Opened").toString() ,Qt::ISODate).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["confirmed_at"] = QString::number( QDateTime::fromString( j_curr.value("Opened").toString() ,Qt::ISODate).toTimeZone( QTimeZone("UTC+03:00") ).toSecsSinceEpoch() );
                    j_item["status"] = j_curr.value("Authorized").toBool() == true ? ( j_curr.value("PendingPayment").toBool() == true ? "sending" : "completed" ) : "pending";
                    j_item["remote_id"] =  j_curr.value("PaymentUuid").toString();
                    j_item["amount"] = QString::number( j_curr.value("Amount").toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( j_curr.value("TxCost").toDouble(), 'f', 8 );
                     j_item["tx"] = j_curr.value("TxId").toString();
                    j_item["deposit_address"] =  j_curr.value("Address").toString();
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["withdraws"] = items;
            emit resultParsed( uuid, j_ret );
        }  else if ( method == SwiftApiClient::AsyncMethods::GetDepositAddress ) {

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
