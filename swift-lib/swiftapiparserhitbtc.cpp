#include "swiftapiparserhitbtc.h"


QString SwiftApiParserHitbtc::getExchangeName() const
{
    return "hitbtc";
}

void SwiftApiParserHitbtc::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    if ( method == SwiftApiClient::AsyncMethods::OrderGet ) {
        QJsonObject j_order_data;
        QJsonObject j_data( QJsonDocument::fromJson( data ).array().first().toObject() );
        j_order_data["remote_id"] = QString::number( j_data.value("id").toVariant().toUInt() );
        j_order_data["local_id"] = j_data.value("clientOrderId").toString();
        j_order_data["created_at"] = QString::number( QDateTime::fromString( j_data.value("createdAt").toString(), Qt::ISODate  ).toSecsSinceEpoch());
        j_order_data["updated_at"] = QString::number( QDateTime::fromString( j_data.value("updatedAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
        j_order_data["amount"] = j_data.value("quantity").toString();
        j_order_data["amount_left"] = QString::number( j_data.value("quantity").toString().toDouble() - j_data.value("cumQuantity").toString().toDouble(), 'f', 8 );
        j_order_data["rate"] = j_data.value("price").toString();
        j_order_data["status"] = j_data.value("status").toString() == "new" ? "1" : j_data.value("status").toString() == "canceled" ? "3" : "2";
        j_order_data["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_data.value("symbol").toString(), getExchangeId() ) );
        j_order_data["exchange_id"] = QString::number( getExchangeId() );
        j_order_data["type"] = j_data.value("side").toString();
        j_order_data["success"] = true;
        j_order_data["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid, j_order_data );
    } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
        QJsonObject j_order_data;
        QJsonObject j_data( QJsonDocument::fromJson( data ).object() );
        j_order_data["remote_id"] = QString::number( j_data.value("id").toVariant().toUInt() );
        j_order_data["local_id"] = j_data.value("clientOrderId").toString();
        j_order_data["created_at"] = QString::number( QDateTime::fromString( j_data.value("createdAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
        j_order_data["updated_at"] = QString::number( QDateTime::fromString( j_data.value("updatedAt").toString() , Qt::ISODate).toSecsSinceEpoch() );
        j_order_data["amount"] = j_data.value("quantity").toString();
        j_order_data["amount_left"] = QString::number( j_data.value("quantity").toString().toDouble() - j_data.value("cumQuantity").toString().toDouble(), 'f', 8 );
        j_order_data["rate"] = j_data.value("price").toString();
        j_order_data["status"] = j_data.value("status").toString() == "new" ? "1" : j_data.value("status").toString() == "canceled" ? "3" : "2";
        j_order_data["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_data.value("symbol").toString(), getExchangeId() ) );
        j_order_data["exchange_id"] = QString::number( getExchangeId() );
        j_order_data["type"] = j_data.value("side").toString();
        j_order_data["success"] = true;
        j_order_data["async_uuid"] = QString::number( uuid );
        j_order_data["raw"] = j_data;
        emit resultParsed( uuid, j_order_data );
    } else if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
        QJsonObject j_order_data;
        QJsonObject j_data( QJsonDocument::fromJson( data ).object() );
        j_order_data["remote_id"] = QString::number( j_data.value("id").toVariant().toUInt() );
        j_order_data["local_id"] = j_data.value("clientOrderId").toString();
        j_order_data["created_at"] = QString::number( QDateTime::fromString( j_data.value("createdAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
        j_order_data["updated_at"] = QString::number( QDateTime::fromString( j_data.value("updatedAt").toString() , Qt::ISODate).toSecsSinceEpoch() );
        j_order_data["amount"] = j_data.value("quantity").toString();
        j_order_data["amount_left"] = QString::number( j_data.value("quantity").toString().toDouble() - j_data.value("cumQuantity").toString().toDouble(), 'f', 8 );
        j_order_data["rate"] = j_data.value("price").toString();
        j_order_data["status"] = j_data.value("status").toString() == "canceled" ? "3" : "4";
        j_order_data["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_data.value("symbol").toString(), getExchangeId() ) );
        j_order_data["exchange_id"] = QString::number( getExchangeId() );
        j_order_data["type"] = j_data.value("side").toString();
        j_order_data["success"] = true;
        j_order_data["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid, j_order_data );
    } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders ) {

        QJsonArray j_resp_arr( QJsonDocument::fromJson( data ).array() );
        QJsonArray openorders;
        if ( !j_resp_arr.isEmpty() ) {
            for ( auto it = j_resp_arr.begin(); it != j_resp_arr.end(); it++ ) {
                QJsonObject j_data( it->toObject() );
                QJsonObject j_order_data;
                j_order_data["remote_id"] = QString::number( j_data.value("id").toVariant().toUInt() );
                j_order_data["local_id"] = j_data.value("clientOrderId").toString();
                j_order_data["created_at"] = QString::number( QDateTime::fromString( j_data.value("createdAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                j_order_data["updated_at"] = QString::number( QDateTime::fromString( j_data.value("updatedAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                j_order_data["amount"] = j_data.value("quantity").toString();
                j_order_data["amount_left"] = QString::number( j_data.value("quantity").toString().toDouble() - j_data.value("cumQuantity").toString().toDouble(), 'f', 8 );
                j_order_data["rate"] = j_data.value("price").toString();
                j_order_data["status"] = j_data.value("status").toString() == "new" ? "1" : j_data.value("status").toString() == "canceled" ? "3" : "2";
                j_order_data["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_data.value("symbol").toString(), getExchangeId() ) );
                j_order_data["exchange_id"] = QString::number( getExchangeId() );
                j_order_data["type"] = j_data.value("side").toString();
                openorders.push_back( j_order_data );
            }
        }
        QJsonObject j_ret;
        j_ret["raw"] = j_resp_arr;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["orders"] = openorders;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory  ) {
        QJsonArray j_resp_arr( QJsonDocument::fromJson( data ).array() );
        QJsonArray openorders;
        if ( !j_resp_arr.isEmpty() ) {
            for ( auto it = j_resp_arr.begin(); it != j_resp_arr.end(); it++ ) {
                QJsonObject j_data( it->toObject() );
                QJsonObject j_order_data;
                j_order_data["remote_id"] = QString::number( j_data.value("id").toVariant().toUInt() );
                j_order_data["local_id"] = j_data.value("clientOrderId").toString();
                j_order_data["created_at"] = QString::number( QDateTime::fromString( j_data.value("createdAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                j_order_data["updated_at"] = QString::number( QDateTime::fromString( j_data.value("updatedAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                j_order_data["amount"] = j_data.value("quantity").toString();
                j_order_data["amount_left"] = QString::number( j_data.value("quantity").toString().toDouble() - j_data.value("cumQuantity").toString().toDouble(), 'f', 8 );
                j_order_data["rate"] = j_data.value("price").toString();
                j_order_data["status"] = j_data.value("status").toString() == "new" ? "1" : j_data.value("status").toString() == "canceled" ? "3" : "2";
                j_order_data["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( j_data.value("symbol").toString(), getExchangeId() ) );
                j_order_data["exchange_id"] = QString::number( getExchangeId() );
                j_order_data["type"] = j_data.value("side").toString();
                if ( j_data.value("status").toString() == "filled" ||  j_data.value("status").toString() == "canceled" ) {
                    openorders.push_back( j_order_data );
                }
            }
        }
        QJsonObject j_ret;
        j_ret["raw"] = j_resp_arr;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["orders"] = openorders;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
        QJsonArray j_resp_arr( QJsonDocument::fromJson( data ).array() );
        QJsonArray bals;
        if ( !j_resp_arr.isEmpty() ) {
            for ( auto it = j_resp_arr.begin(); it != j_resp_arr.end(); it++ ) {
                const QJsonObject j_itm( it->toObject() );
                const quint32 currid = SwiftLib::getAssets()->getCurrencyIdByName(j_itm.value("currency").toString(), getExchangeId() );
                if ( currid > 0 ) {
                    QJsonObject j_item;
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currid );
                    j_item["name"] = j_itm.value("currency").toString();

                    j_item["available"] = QString::number( j_itm.value("available").toString().toDouble(), 'f', 8 );
                    j_item["in_orders"] = QString::number( j_itm.value("reserved").toString().toDouble(), 'f', 8 );
                    if ( _account_balances_storage.contains( currid ) ) {
                        j_item["reserved"] = QString::number( _account_balances_storage.value( currid ), 'f', 8 );
                        j_item["total"] = QString::number( j_itm.value("available").toString().toDouble() +  j_itm.value("reserved").toString().toDouble() + _account_balances_storage.value( currid ), 'f', 8);
                    } else {
                        j_item["reserved"] = QString::number( 0 );
                        j_item["total"] = QString::number( j_itm.value("available").toString().toDouble() +  j_itm.value("reserved").toString().toDouble(), 'f', 8);
                    }
                    j_item["pending"] = QString::number( 0 );
                    j_item["today_in_trades"] = QString::number( 0 );
                    j_item["required"] = QString::number( 0 );
                    bals.push_back( j_item );
                }
            }
        }
        QJsonObject j_ret;
        j_ret["raw"] = j_resp_arr;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["balances"] = bals;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::GetDeposits ) {
        QJsonArray j_resp_arr( QJsonDocument::fromJson( data ).array() );
        QJsonArray deposits;
        if ( !j_resp_arr.isEmpty() ) {
            for ( auto it = j_resp_arr.begin(); it != j_resp_arr.end(); it++ ) {
                const QJsonObject j_itm( it->toObject() );
                const quint32 currid = SwiftLib::getAssets()->getCurrencyIdByName(j_itm.value("currency").toString(), getExchangeId() );
                if ( currid > 0 && j_itm.value("type").toString() != "bankToExchange" && j_itm.value("type").toString() != "exchangeToBank" ) {
                    QJsonObject j_item;
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currid );
                    j_item["created_at"] = QString::number( QDateTime::fromString( j_itm.value("createdAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                    j_item["confirmed_at"] = QString::number( QDateTime::fromString( j_itm.value("updatedAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                    j_item["remote_id"] = j_itm.value("id").toString();
                    j_item["amount"] = QString::number( j_itm.value("amount").toString().toDouble(), 'f', 8 );
                    j_item["status"] = j_itm.value("status").toString();
                    j_item["tx"] = j_itm.value("hash").toString("???");
                    j_item["deposit_address"] = j_itm.value("address").toString("???");
                    deposits.push_back( j_item );
                }
            }
        }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["raw"] = j_resp_arr;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["deposits"] = deposits;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawHistory || method == SwiftApiClient::AsyncMethods::WithdrawList ) {
        QJsonArray j_resp_arr( QJsonDocument::fromJson( data ).array() );
        QJsonArray withdraws;
        if ( !j_resp_arr.isEmpty() ) {
            for ( auto it = j_resp_arr.begin(); it != j_resp_arr.end(); it++ ) {
                const QJsonObject j_itm( it->toObject() );
                const quint32 currid = SwiftLib::getAssets()->getCurrencyIdByName(j_itm.value("currency").toString(), getExchangeId() );
                if ( currid > 0 && j_itm.value("type").toString() != "bankToExchange" && j_itm.value("type").toString() != "exchangeToBank" ) {
                    QJsonObject j_item;
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( currid );
                    j_item["created_at"] = QString::number( QDateTime::fromString( j_itm.value("createdAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                    j_item["confirmed_at"] = QString::number( QDateTime::fromString( j_itm.value("updatedAt").toString(), Qt::ISODate ).toSecsSinceEpoch() );
                    j_item["remote_id"] = j_itm.value("id").toString();
                    j_item["amount"] = QString::number( j_itm.value("amount").toString().toDouble(), 'f', 8 );
                    j_item["status"] = j_itm.value("status").toString();
                    j_item["tx"] = j_itm.value("hash").toString("???");
                    j_item["deposit_address"] = j_itm.value("address").toString("???");
                    if ( j_item.value("status").toString() != "failed" ) {
                        withdraws.push_back( j_item );
                    }
                }
            }
        }
        QJsonObject j_ret;
        j_ret["raw"] = j_resp_arr;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["withdraws"] = withdraws;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawCreate ) {
        QJsonObject j_ret;
        j_ret["raw"] = QJsonDocument::fromJson( data ).object();
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["remote_id"] = QJsonDocument::fromJson( data ).object().value("id").toString();
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawInner ) {
        QJsonObject j_ret;
        j_ret["raw"] = QJsonDocument::fromJson( data ).object();
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::GetDepositAddress ) {
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        const QJsonObject ret( QJsonDocument::fromJson( data ).object() );
        j_ret["address"] = ret.value("address").toString();
        if ( ret.contains("paymentId") ) {
            j_ret["tag"] = ret.value("paymentId").toString();
        }
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::TimeSyncMethod ) {
        QJsonArray j_resp_arr( QJsonDocument::fromJson( data ).array() );
        QJsonArray bals;
        _account_balances_storage.clear();
        if ( !j_resp_arr.isEmpty() ) {
            for ( auto it = j_resp_arr.begin(); it != j_resp_arr.end(); it++ ) {
                const QJsonObject j_itm( it->toObject() );
                const quint32 currid = SwiftLib::getAssets()->getCurrencyIdByName(j_itm.value("currency").toString(), getExchangeId() );
                if ( currid > 0 ) {
                    _account_balances_storage[ currid ] =  j_itm.value("available").toString().toDouble() +  j_itm.value("reserved").toString().toDouble();
                }
            }
        }
        return;
    }
    else {
        QJsonObject j_ret;
        j_ret["raw"] = QString( data );
        j_ret["success"] = false;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["error"] = getExchangeName() + " : UNKNOWN METHOD : " + SwiftApiClient::getMethodName( method );
        emit resultParsed( uuid, j_ret );
    }
}
