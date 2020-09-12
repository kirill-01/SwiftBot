#include "swiftapiparserkucoin.h"


QString SwiftApiParserKuCoin::getExchangeName() const
{
    return "kucoin";
}

void SwiftApiParserKuCoin::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
    const quint32 serviceCode( j_resp.value("code").toString().toUInt() );
    if ( serviceCode != 200000 ) {
        QJsonObject j_ret;
        j_ret["success"] = false;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["error"] = QString( data );
        emit resultParsed( uuid, j_ret );
        return;
    }
    if ( method == SwiftApiClient::AsyncMethods::GetOrderbooks ) {

        const QJsonObject j_data( j_resp.value("data").toObject() );
        const QJsonArray j_asks(j_data.value("asks").toArray() );
        const QJsonArray j_bids( j_data.value("bids").toArray() );

        QJsonArray asks;
        QJsonArray bids;

        for( auto _ai = j_asks.begin(); _ai != j_asks.end(); ++_ai ) {
            QJsonArray item;
            item.push_back( QString::number( 18 ) );
            item.push_back( QString::number( _ai->toArray().at(0).toString().toDouble(), 'f', 8 ) );
            item.push_back( QString::number( _ai->toArray().at(1).toString().toDouble(), 'f', 8 ) );
            asks.push_back( item );
        }
        for( auto _ai = j_bids.begin(); _ai != j_bids.end(); ++_ai ) {
            QJsonArray item;
            item.push_back( QString::number( 18 ) );
            item.push_back( QString::number( _ai->toArray().at(0).toString().toDouble(), 'f', 8 ) );
            item.push_back( QString::number( _ai->toArray().at(1).toString().toDouble(), 'f', 8 ) );
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
            const quint32 market_id = 18;
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
        QJsonObject j_order( j_resp.value("data").toObject() );
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["market_id"] = "18";
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["remote_id"] = j_order.value("id").toString();
        j_ret["local_id"] = j_order.value("clientOid").toString();
        j_ret["created_at"] = QString::number( j_order.value("createdAt").toVariant().toULongLong() );
        j_ret["price"] =  QString::number( j_order.value("funds").toString().toDouble() + j_order.value("dealFunds").toString().toDouble(), 'f', 8);
        j_ret["rate"] = QString::number( j_order.value("price").toString().toDouble(), 'f', 8);
        j_ret["amount"] = QString::number( j_order.value("size").toString().toDouble(), 'f', 8);
        j_ret["amount_left"] = QString::number( j_order.value("size").toString().toDouble() - j_order.value("dealSize").toDouble(), 'f', 8);
        j_ret["type"] =  j_order.value("side").toString();
        j_ret["status"] = j_order.value("isActive").toBool(false) == true ?  "1" : "2";
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::OrderPlace ) {
        QJsonObject j_order( j_resp.value("data").toObject() );
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["market_id"] = "18";
        j_ret["created_at"] = QString::number( QDateTime::currentMSecsSinceEpoch() );
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["remote_id"] = j_order.value("orderId").toString();
        j_ret["status"] = "1";
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
    } else if ( method == SwiftApiClient::AsyncMethods::TradeOpenOrders ) {
        const QJsonArray j_a( QJsonDocument::fromJson( data ).object().value("data").toObject().value("items").toArray() );
        QJsonArray items;
        if ( !j_a.isEmpty() ) {
            QJsonArray j_ret;
            for( auto it = j_a.begin(); it != j_a.end(); it++ ) {
                QJsonObject itm( it->toObject( ));
                QJsonObject j_item;
                const QString marketname(  "BTC-USDT" );
                j_item["exchange_id"] = QString::number( getExchangeId() );
                j_item["exchange_name"] = getExchangeName();
                j_item["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( marketname, getExchangeId() ) );
                j_item["created_at"] = QString::number( itm.value("createdAt").toVariant().toULongLong() );
                j_item["closed_at"] =  QString::number( 0 );
                j_item["status"] = "1";
                j_item["remote_id"] =  itm.value("id").toString();
                j_item["local_id"] = itm.value("clientOid").toString();
                j_item["price"] =   QString::number( j_resp.value("funds").toString().toDouble() + j_resp.value("dealFunds").toString().toDouble(), 'f', 8);
                j_item["rate"] = QString::number( itm.value("price").toString().toDouble(), 'f', 8);
                j_item["amount"] = QString::number( itm.value("size").toString().toDouble(), 'f', 8);
                j_item["amount_left"] = QString::number( itm.value("size").toString().toDouble() - itm.value("dealSize").toString().toDouble(), 'f', 8);
                j_item["fee"] = QString::number( 0.0003, 'f', 8 );
                j_item["type"] = itm.value("side").toString();
                items.push_back( j_item );
            }
        }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["orders"] = items;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::TradeHistory  ) {
        const QJsonArray j_a( QJsonDocument::fromJson( data ).object().value("data").toObject().value("items").toArray() );
        QJsonArray items;
        if ( !j_a.isEmpty() ) {
            QJsonArray j_ret;
            for( auto it = j_a.begin(); it != j_a.end(); it++ ) {
                QJsonObject itm( it->toObject( ));
                QJsonObject j_item;
                const QString marketname(  "BTC-USDT" );
                j_item["exchange_id"] = QString::number( getExchangeId() );
                j_item["exchange_name"] = getExchangeName();
                j_item["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( marketname, getExchangeId() ) );
                j_item["created_at"] = QString::number(  itm.value("createdAt").toVariant().toULongLong()  );
                j_item["closed_at"] = QString::number( itm.value("createdAt").toVariant().toULongLong()  );
                j_item["ts"] = QString::number( itm.value("createdAt").toVariant().toULongLong()  );
                j_item["status"] = itm.value("isActive").toBool(false) == true ? "1" : "2";
                j_item["remote_id"] =  itm.value("id").toString();
                j_item["local_id"] = itm.value("clientOid").toString();
                j_item["price"] =   QString::number( j_resp.value("funds").toString().toDouble() + j_resp.value("dealFunds").toString().toDouble(), 'f', 8);
                j_item["rate"] = QString::number( itm.value("price").toString().toDouble(), 'f', 8);
                j_item["amount"] = QString::number( itm.value("size").toString().toDouble(), 'f', 8);
                j_item["amount_left"] = QString::number( itm.value("size").toString().toDouble() - itm.value("dealSize").toDouble(), 'f', 8);
                j_item["fee"] = QString::number( 0.0003, 'f', 8 );
                j_item["type"] = itm.value("side").toString();
                items.push_back( j_item );
            }
        }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["orders"] = items;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::GetCurrencies ) {
        const QJsonArray j_data( j_resp.value("data").toArray() );
        QJsonArray items;
        for ( auto it = j_data.begin(); it != j_data.end(); it++ ) {
            const QJsonObject j_cur( it->toObject( ) );
            const QString curname( j_cur.value("name").toString() );
            const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( curname, getExchangeId() );
            if ( currency_id > 0 ) {
                    QJsonObject itm;
                    itm["exchange_id"] = QString::number( getExchangeId() );
                    itm["name"] = curname;
                    itm["currency_id"] = QString::number( currency_id );
                    itm["precision"] = QString::number( j_cur.value("precision").toInt() );
                    itm["withdraw_enabled"] = true;
                    itm["withdraw_fee"] = QString::number( j_cur.value("withdrawalMinFee").toString().toDouble(), 'f', 8);
                    itm["withdraw_min"] = QString::number( j_cur.value("withdrawalMinSize").toString().toDouble(), 'f', 8);
                    itm["withdraw_limit_daily"] = QString::number( 0 );
                    itm["deposit_address"] = "";
                    items.push_back( itm );
                }
            }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["currencies"] = items;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
        const QJsonArray j_data( j_resp.value("data").toArray() );
        QJsonArray bals;
        if ( !j_data.isEmpty() ) {
            QHash<quint32, QString> _main_accs_ids;
            QHash<quint32, QString> _trading_accs_ids;
            QHash<quint32, double> _balances_main;
            QHash<quint32, double> _balances_main_available;
            QHash<quint32, double> _balances_trading;
            QHash<quint32, double> _balances_trading_available;
            QList<quint32> _processed_bals;
            for( auto it = j_data.begin(); it != j_data.end(); it++ ) {
                const QJsonObject j_curr( it->toObject() );
                const QString acc_type = j_curr.value("type").toString();
                const quint32 cid = SwiftLib::getAssets()->getCurrencyIdByName(j_curr.value("currency").toString(), getExchangeId() );
                if ( !_processed_bals.contains ( cid ) ) { _processed_bals.push_back( cid ); }

                const double balance( j_curr.value("available").toString().toDouble() );
                const double total( j_curr.value("balance").toString().toDouble() );
                if ( cid > 0  ) {
                    if ( acc_type== "main") {
                        _main_accs_ids[ cid ] = j_curr.value("id").toString();
                        _balances_main[ cid ] = total;
                        _balances_main_available[ cid ] = balance;
                    } else if ( acc_type == "trade" ) {
                        _trading_accs_ids[ cid ] = j_curr.value("id").toString();
                        _balances_trading[ cid ] = total;
                        _balances_trading_available[ cid ] = balance;
                    }

                }
            }
            for( auto it = _processed_bals.begin(); it != _processed_bals.end(); it++ ) {
                const quint32 cid = *it;
                    QJsonObject j_item;
                    j_item["exchange_id"] = QString::number( getExchangeId() );
                    j_item["currency_id"] = QString::number( cid );
                    j_item["main_uuid"] = _main_accs_ids.value( cid );
                    j_item["trade_uuid"] = _trading_accs_ids.value( cid );
                    j_item["name"] =  SwiftLib::getAssets()->getCurrencyName( cid );
                    const double totalbalance = _balances_main.value( cid, 0 ) + _balances_trading.value( cid, 0 );
                    j_item["total"] = QString::number( totalbalance, 'f', 8 );
                    j_item["available"] = QString::number( _balances_trading_available.value( cid, 0 ), 'f', 8 );
                    j_item["in_orders"] = QString::number( _balances_trading.value( cid, 0 ) - _balances_trading_available.value( cid, 0 ), 'f', 8 );
                    j_item["reserved"] = QString::number(  _balances_main.value( cid, 0 ) );
                    j_item["pending"] = QString::number( 0, 'f', 8 );
                    j_item["today_in_trades"] = QString::number( 0 );
                    j_item["required"] = QString::number( 0 );
                    bals.push_back( j_item );
            }

            QJsonObject j_ret;
            j_ret["success"] = true;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["balances"] = bals;
            emit resultParsed( uuid, j_ret );
        } else {
            QJsonObject j_ret;
            j_ret["success"] = false;
            j_ret["async_uuid"] = QString::number( uuid );
            j_ret["error"] = QString( "Empty data: " + data );
            emit resultParsed( uuid, j_ret );
        }
    } else if ( method == SwiftApiClient::AsyncMethods::GetDeposits ) {

        const QJsonArray j_data( j_resp.value("data").toObject().value("items").toArray() );
        QJsonArray deposits;
        if ( !j_data.isEmpty() ) {
            for( auto it = j_data.begin(); it != j_data.end(); it++ ) {
                QJsonObject j_itm( it->toObject( ) );
                if ( j_itm.value("isInner").toBool(false) == false ) {
                    QJsonObject j_dep_rec;
                    j_dep_rec["exchange_id"] = QString::number( getExchangeId() );
                    j_dep_rec["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( j_itm.value("currency").toString(), getExchangeId() ) );
                    j_dep_rec["created_at"] = QString::number( j_itm.value("createdAt").toVariant().toULongLong() / 1000 );
                    j_dep_rec["confirmed_at"] = j_itm.value("updatedAt");
                    j_dep_rec["status"] = j_itm.value("status").toString() == "SUCCESS" ? "done" : ( j_itm.value("status").toString() == "FAILURE" ? "error": "pending");
                    j_dep_rec["remote_id"] = j_itm.value("walletTxId");
                    j_dep_rec["local_id"] = "";
                    j_dep_rec["amount"] = j_itm.value("amount");
                    j_dep_rec["fee"] = j_itm.value("fee");
                    j_dep_rec["deposit_address"] = j_itm.value("address").toString();
                    deposits.push_back( j_dep_rec );
                }
            }
        }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["deposits"] = deposits;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawHistory || method == SwiftApiClient::AsyncMethods::WithdrawList ) {

        const QJsonArray j_data( j_resp.value("data").toObject().value("items").toArray() );
        QJsonArray deposits;
        if ( !j_data.isEmpty() ) {
            for( auto it = j_data.begin(); it != j_data.end(); it++ ) {
                QJsonObject j_itm( it->toObject( ) );
                if ( j_itm.value("isInner").toBool(false) == false ) {
                    QJsonObject j_dep_rec;
                    j_dep_rec["exchange_id"] = QString::number( getExchangeId() );
                    j_dep_rec["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( j_itm.value("currency").toString(), getExchangeId() ) );
                    j_dep_rec["created_at"] = QString::number( j_itm.value("createdAt").toVariant().toULongLong() / 1000 );
                    j_dep_rec["confirmed_at"] = j_itm.value("updatedAt");
                    j_dep_rec["status"] = j_itm.value("status").toString() == "SUCCESS" ? "done" : ( j_itm.value("status").toString() == "FAILURE" ? "error": "pending");
                    j_dep_rec["remote_id"] = j_itm.value("id").toString();
                    j_dep_rec["local_id"] = "";
                    j_dep_rec["amount"] = j_itm.value("amount");
                    j_dep_rec["fee"] = j_itm.value("fee");
                    j_dep_rec["deposit_address"] = j_itm.value("address").toString();
                    deposits.push_back( j_dep_rec );
                }
            }
        }
        QJsonObject j_ret;
        j_ret["success"] = true;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["withdraws"] = deposits;
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawInner ) {
        QJsonObject j_ret;
        j_ret["inner"] = true;
        if ( j_resp.contains("msg") && ! j_resp.contains("withdrawalId") ) {
            j_ret["success"] = false;
            j_ret["error"] = j_resp.value("code").toString()+":"+j_resp.value("msg").toString();
        } else {
            j_ret["success"] = true;
            j_ret["uuid"] = j_resp.value("withdrawalId").toString();
        }
        j_ret["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawCreate ) {

        QJsonObject j_ret;
        if ( j_resp.contains("msg") && ! j_resp.contains("withdrawalId") ) {
            j_ret["success"] = false;
            j_ret["error"] = j_resp.value("code").toString()+":"+j_resp.value("msg").toString();
        } else {
            j_ret["success"] = true;
            j_ret["remote_id"] = j_resp.value("withdrawalId").toString();
        }
        j_ret["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid, j_ret );
    } else if ( method == SwiftApiClient::AsyncMethods::TimeSyncMethod ) {

    }
    else {

        QJsonObject j_ret;
        j_ret["success"] = false;
        j_ret["async_uuid"] = QString::number( uuid );
        j_ret["error"] = getExchangeName() + " : UNKNOWN METHOD : " + SwiftApiClient::getMethodName( method );
        emit resultParsed( uuid, j_ret );
    }
}
