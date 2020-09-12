#include "swiftapiparsercexio.h"
#include "systemevent.h"

QString SwiftApiParserCexio::getExchangeName() const
{
    return "cexio";
}

void SwiftApiParserCexio::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
    if ( j_resp.contains("error") ) {
        QJsonObject j_ret;
        j_ret["success"] = false;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["error"] = j_resp.value("error").toString();
        emit resultParsed( uuid, j_ret );
        return;
    }
        if ( method == SwiftApiClient::AsyncMethods::GetOrderbooks ) {
            QJsonArray j_bids = j_resp.value("bids").toArray();
            QJsonArray j_asks = j_resp.value("asks").toArray();
            if ( j_bids.isEmpty() && j_asks.isEmpty() ) {
                return;
            }
            QJsonArray::const_iterator _ai;
            QJsonArray asks;
            QJsonArray bids;
            for( auto  _ai = j_asks.begin(); _ai != j_asks.end(); ++_ai ) {
                QJsonArray item;
                item.push_back( QString::number( 14 ) );
                item.push_back( QString::number( _ai->toArray().at(0).toDouble(), 'f', 8 ) );
                item.push_back( QString::number( _ai->toArray().at(1).toDouble(), 'f', 8 ) );
                asks.push_back( item );
            }
            for( auto _ai = j_bids.begin(); _ai != j_bids.end(); ++_ai ) {
                QJsonArray item;
                item.push_back( QString::number( 14 ) );
                item.push_back( QString::number( _ai->toArray().at(0).toDouble(), 'f', 8 ) );
                item.push_back( QString::number( _ai->toArray().at(1).toDouble(), 'f', 8 ) );
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
            QJsonArray j_objects( j_resp.value("data").toObject().value("pairs").toArray() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                const QString market_name( j_curr.value("symbol1").toString()+":"+j_curr.value("symbol2").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName(market_name, getExchangeId() );
                const quint32 mcid = SwiftLib::getAssets()->getCurrencyIdByName( j_curr.value("symbol1").toString(), getExchangeId() );
                const quint32 bcid = SwiftLib::getAssets()->getCurrencyIdByName( j_curr.value("symbol2").toString(), getExchangeId() );
                if ( market_id > 0  && bcid > 0 && mcid > 0) {
                    QJsonObject itm;
                    itm["exchange_id"] = QString::number( getExchangeId() );
                    itm["name"] =  market_name;
                    itm["market_id"] = QString::number( market_id );
                    itm["base_currency_id"] = QString::number( bcid );
                    itm["market_currency_id"] = QString::number( mcid );
                    itm["trade_fee"] = QString::number( 0.0003, 'f', 8 );
                    itm["min_trade_size"] = QString::number(  j_curr.value("minLotSize").toDouble(), 'f', 8 );
                    items.push_back( itm );
                }
            }
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["markets"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderGet ) {
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["remote_id"] = j_resp.value("id").toString();
            j_ret["market_id"] = "14";
            j_ret["created_at"] = j_resp.value("time");
            j_ret["amount"] = QString::number( j_resp.value("amount").toString().toDouble(),'f',8 );
            j_ret["amount_left"] =QString::number( j_resp.value("remains").toString().toDouble(),'f',8 );
            j_ret["rate"] = QString::number( j_resp.value("price").toString().toDouble(),'f',8 );
            j_ret["type"] =  j_resp.value("type").toString();
            j_ret["fee"] = QString::number( 0.0003, 'f', 8 );
            j_ret["status"] = j_resp.value("status").toString() == "a" ?  "1" : (  j_resp.value("status").toString()  == "c" ? "3" : "2");
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["market_id"] = "14";
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["remote_id"] = j_resp.value("id").toString();
            j_ret["created_at"] =  QString::number( QDateTime::currentMSecsSinceEpoch() );
            j_ret["amount_left"] =QString::number( j_resp.value("remains").toString().toDouble(),'f',8 );
            j_ret["ts"] =    QString::number( QDateTime::currentMSecsSinceEpoch() );
            j_ret["amount"] = QString::number( j_resp.value("amount").toString().toDouble(),'f',8 );
            j_ret["price"] = QString::number( j_resp.value("price").toString().toDouble() * j_resp.value("amount").toString().toDouble() , 'f', 8 );
            j_ret["rate"] = QString::number( j_resp.value("price").toString().toDouble(),'f',8 );
            j_ret["type"] =  j_resp.value("type").toString();
            j_ret["status"] = j_resp.value("complete").toBool() == true ?  "2" : "1";
            j_ret["fee"] = QString::number( 0.0003, 'f', 8 );
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
            QJsonObject j_ret;
            if ( static_cast<QVariant>( QString(data) ).toBool() == true ) {
                j_ret["status"] = "3";
                j_ret["success"] = true;
            } else {
                j_ret["status"] = "4";
                j_ret["success"] = false;
            }
            j_ret["async_uuid"] = QString::number( uuid );
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::TradeGetFee ) {
            QJsonObject j_objects( QJsonDocument::fromJson( data ).object() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                if ( it.key() == "BTC:USD" ) {
                    QJsonObject itm;
                    itm["amount"] = QString::number( 0 );
                    itm["taker"] = QString::number( it.value().toObject().value("buy").toString().toDouble() * 100, 'f',8 );
                    items.push_back( itm );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["fees"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders ) {
            QJsonArray j_objects( QJsonDocument::fromJson( data ).array() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString market_name( j_curr.value("symbol1").toString()+":"+j_curr.value("symbol2").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
                if ( market_id > 0  ) {
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["market_id"] = QString::number( market_id );
                    j_item["created_at"] =  j_curr.value("time");
                    j_item["closed_at"] =  QString::number( 0 );
                    j_item["market_id"] = "14";
                    j_item["status"] = "1";
                    j_item["remote_id"] =  j_curr.value("id").toString();
                    j_item["price"] = QString::number( j_curr.value("price").toString().toDouble() * j_curr.value("amount").toString().toDouble() , 'f', 8 );
                    j_item["rate"] = QString::number( j_curr.value("price").toString().toDouble() , 'f', 8 );
                    j_item["amount"] = QString::number( j_curr.value("amount").toString().toDouble(), 'f', 8 );
                    j_item["amount_left"] = QString::number( j_curr.value("pending").toString().toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( 0.0003, 'f', 8 );
                    j_item["type"] =  j_curr.value("type").toString();
                    items.push_back( j_item );
                }
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["orders"] = items;
             emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory  ) {
            QJsonArray j_objects( QJsonDocument::fromJson( data ).array() );
            QJsonArray items;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                QJsonObject j_item;
                const QString market_name( j_curr.value("symbol1").toString()+":"+j_curr.value("symbol2").toString()  );
                const quint32 market_id = SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() );
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["market_id"] = QString::number( market_id );
                    j_item["created_at"] =  QString::number( QDateTime::fromString( j_curr.value("time").toString(), Qt::ISODate  ) .toMSecsSinceEpoch() );
                    j_item["closed_at"] =  QString::number( QDateTime::fromString( j_curr.value("time").toString(), Qt::ISODate  ) .toMSecsSinceEpoch() );
                    j_item["ts"] = j_curr.value("time");
                    j_item["market_id"] = "14";
                    j_item["status"] = j_curr.value("Closed").isNull() ? "1" : "2";
                    j_item["remote_id"] =  j_curr.value("orderId").toString();
                    j_item["price"] = QString::number( j_curr.value("price").toString().toDouble() * j_curr.value("amount").toString().toDouble() , 'f', 8 );
                    j_item["rate"] = QString::number( j_curr.value("price").toString().toDouble() , 'f', 8 );
                    j_item["amount"] = QString::number( j_curr.value("amount").toString().toDouble(), 'f', 8 );
                    j_item["amount_left"] = QString::number( j_curr.value("remains").toString().toDouble(), 'f', 8 );
                    j_item["fee"] = QString::number( j_curr.value("tradingFeeTaker").toString().toDouble() / 100 , 'f', 8 );
                    j_item["type"] =  j_curr.value("type").toString();
                    items.push_back( j_item );
            }
            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["orders"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::GetCurrencies ) {
            QJsonObject j_ret;
            QJsonArray j_objects( j_resp.value("data").toObject().value("pairs").toArray() );
            QJsonArray items;
            QList<quint32> _parsed;
            for( auto it = j_objects.begin(); it != j_objects.end(); it++ ) {

                const QJsonObject j_curr( it->toObject() );
                const QString market_name( j_curr.value("symbol1").toString()+":"+j_curr.value("symbol2").toString()  );
                const quint32 mcid = SwiftLib::getAssets()->getCurrencyIdByName( j_curr.value("symbol1").toString(), getExchangeId() );
                const quint32 bcid = SwiftLib::getAssets()->getCurrencyIdByName( j_curr.value("symbol2").toString(), getExchangeId() );
                if ( !_parsed.contains( mcid ) ) {
                    QJsonObject itm;
                    itm["exchange_id"] = QString::number( getExchangeId() );
                    itm["name"] = j_curr.value("symbol2").toString();
                    itm["currency_id"] = QString::number( mcid );
                    itm["precision"] = QString::number( 8 );
                    itm["withdraw_enabled"] = false;
                    itm["withdraw_fee"] = QString::number( 0 );
                    itm["withdraw_min"] = QString::number( 0 );
                    itm["withdraw_limit_daily"] = QString::number( 0 );
                    itm["deposit_address"] = "";
                    items.push_back( itm );
                    _parsed.push_back( mcid );
                }
                if ( !_parsed.contains( bcid ) ) {
                    QJsonObject itm;
                    itm["exchange_id"] = QString::number( getExchangeId() );
                    itm["name"] = j_curr.value("symbol1").toString();
                    itm["currency_id"] = QString::number( bcid );
                    itm["precision"] = QString::number( 8 );
                    itm["withdraw_enabled"] = false;
                    itm["withdraw_fee"] = QString::number( 0 );
                    itm["withdraw_min"] = QString::number( 0 );
                    itm["withdraw_limit_daily"] = QString::number( 0 );
                    itm["deposit_address"] = "";
                    items.push_back( itm );
                    _parsed.push_back( bcid );
                }
            }
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["currencies"] = items;
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
            QStringList keys( j_resp.keys() );
            keys.removeAll("timestamp");
            keys.removeAll("username");
            QJsonArray bals;
            for( auto it = keys.begin(); it != keys.end(); it++ ) {
                const QJsonObject j_curr( j_resp.value( *it ).toObject( ) );
                QJsonObject j_item;
                const QString currency_name( *it );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                if ( currency_id > 0  ) {
                    j_item["currency_id"] = QString::number( currency_id );
                    j_item["name"] = currency_name;
                    j_item["total"] = QString::number( j_curr.value("available").toString().toDouble() +  j_curr.value("orders").toString().toDouble(), 'f', 8 );
                    j_item["available"] = QString::number( j_curr.value("available").toString().toDouble(), 'f', 8 );
                    j_item["in_orders"] = QString::number( j_curr.value("orders").toString().toDouble(), 'f', 8 );
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
            QJsonObject j_ret;
            j_ret["success"] = false;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["error"] = "UNIMPLEMENTED METHOD";
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::WithdrawList ) {
            QJsonObject j_ret;
            j_ret["success"] = false;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["error"] = "UNIMPLEMENTED METHOD";
            emit resultParsed( uuid, j_ret );
        } else if ( method == SwiftApiClient::AsyncMethods::WithdrawHistory ) {
            QJsonObject j_ret;
            j_ret["success"] = false;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["error"] = "UNIMPLEMENTED METHOD";
            emit resultParsed( uuid, j_ret );
        } else {
            QJsonObject j_ret;
            j_ret["success"] = false;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["error"] = "UNKNOWN METHOD";
            emit resultParsed( uuid, j_ret );
        }

}
