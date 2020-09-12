#include "swiftapiparserexmo.h"


QString SwiftApiParserExmo::getExchangeName() const
{
    return "exmo";
}

void SwiftApiParserExmo::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
    if ( j_resp.contains("result") &&  j_resp.value("result").toBool(false) == false ) {
        QJsonObject j_ret;
        j_ret["success"] = false;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["error"] = j_resp.value("error").toString();
        emit resultParsed( uuid, j_ret );
        return;
    }
    if ( method == SwiftApiClient::AsyncMethods::OrderGet ) {
        QJsonObject j_order;
        j_order["async_uuid"] = QString::number( uuid );
        if ( j_resp.value("result").toBool(false) == true )  {
            j_order["success"] = true;
            j_order["type"] = j_resp.value("type").toString();
            j_order["amount"] = j_resp.value("in_amount").toString();
            double amount_left = j_resp.value("in_amount").toString().toDouble();
            if ( !j_resp.value("trades").toArray().isEmpty() ) {
                const QJsonArray trades( j_resp.value("trades").toArray() );
                for( auto it = trades.begin(); it != trades.end(); it++) {
                    amount_left -= it->toObject().value("quantity").toString().toDouble();
                }
            }
            j_order["amount_left"] = QString::number( amount_left, 'f', 8 );
            if ( amount_left == 0 ) {
                j_order["status"] = "2";
            } else {
                j_order["status"] = "1";
            }
        } else {
            j_order["success"] = false;
            j_order["error"] = j_resp.value("error");
        }
        emit resultParsed( uuid, j_order );
    } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
        QJsonObject j_order;
        j_order["success"] = j_resp.value("result").toBool(false);
        j_order["async_uuid"] = QString::number( uuid );
        j_order["created_at"] = QString::number( QDateTime::currentMSecsSinceEpoch() );
        if ( j_resp.value("result").toBool(false) == true ) {
            j_order["status"] = "1";
            j_order["remote_id"] = QString::number( j_resp.value("order_id").toVariant().toULongLong() );
        } else {
            j_order["status"] = "4";
        }
        emit resultParsed( uuid, j_order );
    } else if ( method == SwiftApiClient::AsyncMethods::OrderCancel ) {
        QJsonObject j_order;
        j_order["success"] = j_resp.value("result").toBool(false);
        j_order["async_uuid"] = QString::number( uuid );
        if ( j_resp.value("result").toBool(false) == true ) {
            j_order["status"] = "3";
            j_order["remote_id"] = j_resp.value("order_id");
        } else {
            j_order["status"] = "4";
        }
        emit resultParsed( uuid, j_order );
    } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders ) {
        QJsonArray items;
        QStringList cached;
            for( auto it = j_resp.begin(); it != j_resp.end(); it++ ) {
                const QString market_name = it.key();
                const QJsonArray market_orders( it.value().toArray() );

                for( auto order_it = market_orders.begin(); order_it != market_orders.end(); order_it++ ) {
                    QJsonObject itm( order_it->toObject() );
                    QJsonObject j_item;
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["exchange_name"] = getExchangeName();
                    j_item["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() ) );
                    j_item["created_at"] = QString::number( itm.value("created").toVariant().toULongLong() );
                    j_item["closed_at"] =  QString::number( 0 );
                    j_item["status"] = "1";
                    j_item["remote_id"] =  QString::number( itm.value("order_id").toVariant().toULongLong() );
                    j_item["price"] =   QString::number( itm.value("price").toString().toDouble() * itm.value("quantity").toString().toDouble(), 'f', 8);
                    j_item["rate"] = QString::number( itm.value("price").toString().toDouble(), 'f', 8);
                    j_item["amount"] = QString::number( itm.value("quantity").toString().toDouble(), 'f', 8);
                    j_item["amount_left"] = QString::number( itm.value("quantity").toString().toDouble(), 'f', 8);
                    j_item["price"] =   QString::number( itm.value("price").toString().toDouble() * itm.value("quantity").toString().toDouble(), 'f', 8);
                    j_item["fee"] = QString::number( SwiftLib::getExchangeFee( getExchangeId() ), 'f', 8 );
                    j_item["type"] = itm.value("type").toString();
                    items.push_back( j_item );
                    cached.push_back( j_item.value("remote_id").toString() );
                }
        }
        opn_orders_cache = cached;
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["orders"] = items;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory  ) {
        QJsonArray items;
        QHash<QString, QJsonObject> _ordrs;
        for( auto it = j_resp.begin(); it != j_resp.end(); it++ ) {
            const QString market_name = it.key();
            const QJsonArray market_orders( it.value().toArray() );
            for( auto order_it = market_orders.begin(); order_it != market_orders.end(); order_it++ ) {
                QJsonObject itm( order_it->toObject() );
                const QString orderid( QString::number( itm.value("order_id").toVariant().toULongLong()  ) );
                QJsonObject j_item;
                j_item["exchange_id"] = QString::number( getExchangeId() );
                j_item["exchange_name"] = getExchangeName();
                j_item["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() ) );
                j_item["created_at"] = QString::number( itm.value("date").toVariant().toULongLong() );
                j_item["closed_at"] =  QString::number( itm.value("date").toVariant().toULongLong() );
                j_item["remote_id"] =   QString::number( itm.value("order_id").toVariant().toULongLong()  );
                j_item["status"] = opn_orders_cache.contains(j_item.value("remote_id").toString() ) ? "1" : "2";
                j_item["price"] =   QString::number( itm.value("price").toString().toDouble() * itm.value("quantity").toString().toDouble(), 'f', 8);
                j_item["rate"] = QString::number( itm.value("price").toString().toDouble(), 'f', 8);
                j_item["amount"] = QString::number( itm.value("quantity").toString().toDouble(), 'f', 8);
                j_item["amount_left"] = QString::number( 0, 'f', 8);
                j_item["fee"] = QString::number( SwiftLib::getExchangeFee( getExchangeId() ), 'f', 8 );
                j_item["type"] = itm.value("type").toString();
                if ( !_ordrs.contains( orderid ) ) {
                    _ordrs.insert( orderid, j_item );
                } else {
                    _ordrs[orderid]["amount"] = QString::number( _ordrs[orderid].value("amount").toString().toDouble() +  j_item.value("amount").toString().toDouble(), 'f', 8 );
                    _ordrs[orderid]["price"] = QString::number( _ordrs[orderid].value("price").toString().toDouble() +  j_item.value("price").toString().toDouble(), 'f', 8 );
                }
            }
        }
        for ( auto it = _ordrs.begin(); it != _ordrs.end(); it++ ) {
            if ( !opn_orders_cache.contains( it.key() ) ) {
                items.push_back( it.value() );
            }
        }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["orders"] = items;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
        const QJsonObject j_balances( j_resp.value("balances").toObject() );
        const QJsonObject j_orders( j_resp.value("reserved").toObject() );
        QJsonArray bals;
        for ( auto it = j_balances.begin(); it != j_balances.end(); it++ ) {
            if ( SwiftLib::getAssets()->getCurrencyIdByName( it.key(), getExchangeId() ) > 0 ) {
                QJsonObject j_item;
                j_item["exchange_id"] = QString::number( getExchangeId() );
                j_item["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( it.key(), getExchangeId() ) );
                j_item["name"] =   it.key();
                j_item["total"] = QString::number(j_balances.value(it.key()).toString("0.0").toDouble() + j_orders.value(it.key()).toString("0.0").toDouble(), 'f', 8);
                j_item["available"] = QString::number( it.value().toString().toDouble() , 'f', 8 );
                j_item["in_orders"] = QString::number( j_orders.value(it.key()).toString("0.0").toDouble(), 'f', 8 );
                j_item["reserved"] = QString::number( 0 );
                j_item["pending"] = QString::number( 0 );
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
    } else if ( method == SwiftApiClient::AsyncMethods::GetDeposits || method == SwiftApiClient::AsyncMethods::WithdrawHistory || method == SwiftApiClient::AsyncMethods::WithdrawList ) {
        QJsonArray j_withdraws;
        QJsonArray j_deposits;
        QJsonObject j_ret;
        if ( j_resp.value("result").toBool(false) == true ) {
            j_ret["success"] = true;
            if ( !j_resp.value("history").toArray().isEmpty() ) {
                QJsonArray items( j_resp.value("history").toArray() );
                for( auto it = items.begin(); it != items.end(); it++ ) {
                    QJsonObject j_itm( it->toObject() );
                    QJsonObject itm;
                    itm["amount"] = j_itm.value("amount").toString().replace("-","");
                    itm["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( j_itm.value("curr").toString(), getExchangeId() ) );
                    itm["exchange_id"] = QString::number( getExchangeId() );
                    itm["exchange_name"] = getExchangeName();
                    itm["currency_name"] = j_itm.value("cur").toString();
                    itm["created_at"] = QString::number( j_itm.value("dt").toVariant().toULongLong() );
                    itm["status"] = j_itm.value("status").toString() == "processing" ? "pending" : "done";
                    itm["tx_hash"] = j_itm.value("txid").toString();
                    QStringList addressparts( j_itm.value("account").toString().split(": ") );
                    itm["deposit_address"] = addressparts.count() == 2 ? addressparts.at(1) : j_itm.value("account").toString();
                    itm["remote_id"] = QString(addressparts.count() == 2 ? addressparts.at(1) : j_itm.value("account").toString()) + j_itm.value("dt").toString()+j_itm.value("amount").toString();
                    if ( j_itm.value("type").toString() == "deposit" ) {
                        j_deposits.push_back( itm );
                    } else if ( j_itm.value("type").toString() == "withdrawal" ) {
                        j_withdraws.push_back( itm );
                    }
                }
            }
        } else {
            j_ret["success"] = false;
        }
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["deposits"] = j_deposits;
        j_ret["withdraws"] = j_withdraws;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawCreate ) {
        QJsonObject j_ret;
        if ( j_resp.value("result").toBool() == true ) {
            j_ret["success"] = true;
            j_ret["remote_id"] = j_resp.value("task_id").toString();
        } else {
            j_ret["success"] = false;
            j_ret["error"] = j_resp.value("error").toString();
        }
        j_ret["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::GetDepositAddress ) {
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["addresses"] = QString( data );
        emit resultParsed( uuid, j_ret );
    }
    else {
        QJsonObject j_ret;
        j_ret["success"] = false;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["error"] = getExchangeName() + " : UNKNOWN METHOD : " + SwiftApiClient::getMethodName( method );
        emit resultParsed( uuid, j_ret );
    }
}
