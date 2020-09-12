#include "swiftapiparserzb.h"

SwiftApiParserZB::SwiftApiParserZB(QObject *parent) : SwiftApiParser(parent)
{

}


QString SwiftApiParserZB::getExchangeName() const
{
    return "zb";
}

void SwiftApiParserZB::parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    if ( method == SwiftApiClient::AsyncMethods::TimeSyncMethod ) {
        return;
    } else if ( method == SwiftApiClient::AsyncMethods::WithdrawGetLimits ) {
        qWarning() << data;
    }
    const QJsonDocument j_doc = QJsonDocument::fromJson( data );
    QJsonObject parsed_result;
    parsed_result["async_uuid"] = QString::number( uuid );
    if(j_doc.isObject()){
        const QJsonObject j_obj = j_doc.object();
        if(j_obj.contains("code") && ( j_obj.value("code").toInt() != 1000 && j_obj.value("code").toInt() != 3001 && j_obj.value("code").toInt() != 3005 ) ){
            parsed_result["success"] = false;
            parsed_result["error"] = "Error!["+ QString::number( j_obj.value("code").toInt() ) +"] Message:" + j_obj.value("message").toString();
            parsed_result["method"] = QString::number( method );
        }
        else if(j_obj.contains("error") && ( j_obj.value("code").toInt() != 1000 &&  j_obj.value("code").toInt() != 3001 && j_obj.value("code").toInt() != 3005 ) ) {
            parsed_result["success"] = false;
            parsed_result["error"] = "Error! Message:" + j_obj.value("error").toString();
        }
        else if(j_obj.empty()){
            parsed_result["success"] = false;
            parsed_result["error"] = "Error! empty response";
        }
        else {
            parsed_result["success"] = true;
            switch(method){
                case SwiftApiClient::AsyncMethods::GetOrderbooks: break;
            case SwiftApiClient::AsyncMethods::OrderPlace: {
                parsed_result["remote_id"] = j_obj.value("id").toString();
                parsed_result["status"] = "1";
            } break;
            case SwiftApiClient::AsyncMethods::OrderCancel: {
                parsed_result["remote_id"] = j_obj.value("id").toString();
                parsed_result["status"] = "3";
            }break;
            case SwiftApiClient::AsyncMethods::OrderGet:{
                {
                    double totalAmount = j_obj.value("total_amount").toString().toDouble();
                    double tradeAmount = j_obj.value("trade_amount").toString().toDouble();
                    parsed_result["created_at"] = j_obj.value("trade_date").toString();
                    parsed_result["updated_at"] = j_obj.value("trade_date").toString();
                    parsed_result["remote_id"] = j_obj.value("id").toString();
                    parsed_result["amount"] = j_obj.value("total_amount").toString();
                    parsed_result["rate"] = j_obj.value("price").toString();
                    parsed_result["status"] = orderStatus(j_obj.value("status").toString().toInt());
                    parsed_result["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName(j_obj.value("currency").toString(), getExchangeId()));
                    parsed_result["amount_left"] = QString::number(totalAmount - tradeAmount, 'f', 8);
                    parsed_result["type"] = j_obj.value("type").toString().toInt() == 0 ? "sell" : "buy";
                }
            }break;
            case SwiftApiClient::AsyncMethods::WithdrawList:
            case SwiftApiClient::AsyncMethods::WithdrawHistory: {
                QJsonArray withdraws;
                QJsonArray data = j_obj.value("message").toObject().value("datas").toObject().value("list").toArray();
                for(auto it = data.cbegin(); it != data.cend(); ++it) {
                    auto item = it->toObject();
                    withdraws.push_back(QJsonObject({
                                {"created_at", QString::number( item.value("manageTime").toVariant().toULongLong() )},
                                {"updated_at", QString::number( item.value("manageTime").toVariant().toULongLong() )},
                                {"remote_id", QString::number( item.value("id").toVariant().toULongLong() )},
                                {"amount", QString::number( item.value("amount").toString().toDouble(), 'f', 8) },
                                {"exchange_id", QString::number(getExchangeId())},
                                {"status", withdrawStatus(item.value("status").toString().toInt())},
                                {"deposit_address", item.value("toAddress").toString()}
                            }));
                }
                parsed_result["withdraws"] = withdraws;
            }break;
            case SwiftApiClient::AsyncMethods::WithdrawCreate: {
                parsed_result["remote_id"] = j_obj.value("id").toString();
                parsed_result["message"] = j_obj.value("message").toString();
                // qWarning() << parsed_result;
            } break;
            case SwiftApiClient::AsyncMethods::GetBalances: {
                QJsonArray balances;
                QJsonArray data = j_obj.value("result").toObject().value("coins").toArray();
                for(auto it = data.cbegin(); it != data.cend(); it++) {

                    auto item = it->toObject();
                    const quint32 cid = SwiftLib::getAssets()->getCurrencyIdByName(item.value("key").toString(), getExchangeId());
                    if ( cid > 0 ) {
                        double available = item.value("available").toString().toDouble();
                        double freez = item.value("freez").toString().toDouble();

                        QJsonObject j_item;
                        j_item["exchange_id"] = QString::number( getExchangeId() );
                        j_item["currency_id"] = QString::number( cid );
                        j_item["name"] = item.value("key").toString();
                        j_item["total"] =  QString::number(freez+available, 'f', 8);
                        j_item["available"] = QString::number( available, 'f', 8 );
                        j_item["in_orders"] = QString::number( freez, 'f', 8 );
                        j_item["reserved"] = QString::number( 0 );
                        j_item["pending"] = QString::number( 0 );
                        j_item["today_in_trades"] = QString::number( 0 );
                        j_item["required"] = QString::number( 0 );
                        balances.push_back(j_item);
                    }
                }
                parsed_result["balances"] = balances;
            }break;
            case SwiftApiClient::AsyncMethods::GetDeposits: {
                QJsonArray deposits;
                QJsonArray data = j_obj.value("message").toObject().value("datas").toObject().value("list").toArray();
                for(auto it = data.cbegin(); it != data.cend(); it++) {
                    auto item = it->toObject();
                    deposits.push_back(QJsonObject({
                                {"created_at", QString::number( QDateTime::fromString( item.value("submit_time").toString(), "yyyy-MM-dd HH:mm:ss").toSecsSinceEpoch() ) },
                                {"remote_id", QString::number( item.value("id").toVariant().toULongLong() )},
                                {"amount", item.value("amount").toString()},
                                {"exchange_id", QString::number(getExchangeId())},
                                {"status", withdrawStatus( item.value("status").toInt() ) },
                                {"tx", item.value("hash").toString()},
                                {"deposit_address", item.value("address").toString()},
                                {"currency_id", QString::number(SwiftLib::getAssets()->getCurrencyIdByName(item.value("currency").toString(), getExchangeId()))}
                            }));
                }
                parsed_result["deposits"] = deposits;
            }break;
            case SwiftApiClient::AsyncMethods::GetDepositAddress: {
                const QJsonArray j_datas( j_obj.value("message").toObject().value("datas").toArray() );
                QJsonArray addresses;
                for( auto it = j_datas.begin(); it != j_datas.end(); it++ ) {
                    addresses.push_back( it->toObject().value("address").toString() + ":" + it->toObject().value("blockChain").toString() );
                }
                parsed_result["address"] = addresses;
            }break;
            case SwiftApiClient::AsyncMethods::TradeOpenOrders: {
                parsed_result["orders"] = QJsonArray();
            }break;
            case SwiftApiClient::AsyncMethods::TradeHistory: {
                parsed_result["orders"] = QJsonArray();
            }break;
                default: {                   
                    parsed_result["success"] = false;
                    parsed_result["error"] = "Error! Parsing is not defined for method "+QString::number( method );
                    break;
                }
            }
        }
    }
    else if(j_doc.isArray()){
        const QJsonArray j_arr = j_doc.array();
        parsed_result["success"] = true;
        switch(method){
        case SwiftApiClient::AsyncMethods::GetOrderbooks: {
            QJsonArray asks, bids;
            for(auto it = j_arr.cbegin(); it != j_arr.cend(); ++it) {
                auto item = it->toObject();
                QJsonArray item_arr = QJsonArray({
                            "", item.value("price").toString(),
                                item.value("amount").toString()
                        });
                if(item.value("type").toString() == "sell")
                    asks.push_back(item_arr);
                else bids.push_back(item_arr);
            }
            parsed_result["a"] = asks;
            parsed_result["b"] = bids;
        } break;
        case SwiftApiClient::AsyncMethods::TradeHistory: {
            QJsonArray orders;
            qWarning() << orders;
            for(auto it = j_arr.cbegin(); it != j_arr.cend(); ++it) {
                auto item = it->toObject();
                if( orderStatus( item.value("status").toInt() ).toUInt() > 1 ) {
                    QJsonObject item_order;
                    auto amount = item.value("total_amount").toDouble();
                    auto filled_amount = item.value("trade_amount").toDouble();
                    item_order["created_at"] = QString::number( item.value("trade_date").toVariant().toULongLong() );
                    item_order["remote_id"] = item.value("id").toString();
                    item_order["exchange_id"] = QString::number( getExchangeId() );
                    item_order["exchange_name"] = getExchangeName();
                    item_order["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( item.value("currency").toString(), getExchangeId() ) );
                    item_order["amount"] = QString::number(amount, 'f', 8);
                    item_order["rate"] = QString::number(item.value("price").toDouble(), 'f', 8 );
                    item_order["status"] = orderStatus( item.value("status").toInt() );
                    item_order["amount_left"] = QString::number(amount - filled_amount, 'f', 8);
                    item_order["type"] = item.value("type").toInt() == 0 || item.value("type").toInt() == 4 ? "sell" : "buy";
                    orders.push_back(item_order);
                }
            }

            parsed_result["orders"] = orders;
        }break;
        case SwiftApiClient::AsyncMethods::TradeOpenOrders: {
            QJsonArray orders;
            for(auto it = j_arr.cbegin(); it != j_arr.cend(); ++it) {
                auto item = it->toObject();
                if( orderStatus( item.value("status").toInt() ).toUInt() < 2 ) {
                    QJsonObject item_order;
                    auto amount = item.value("total_amount").toDouble();
                    auto filled_amount = item.value("trade_amount").toDouble();
                    item_order["created_at"] = QString::number( item.value("trade_date").toVariant().toULongLong() );
                    item_order["remote_id"] = item.value("id").toString();
                    item_order["market_id"] = QString::number( SwiftLib::getAssets()->getMarketIdByName( item.value("currency").toString(), getExchangeId() ) );
                    item_order["amount"] = QString::number(amount, 'f', 8);
                    item_order["rate"] = QString::number(item.value("price").toDouble(), 'f', 8 );
                    item_order["status"] = orderStatus( item.value("status").toInt() );
                    item_order["amount_left"] = QString::number(amount - filled_amount, 'f', 8);
                    item_order["type"] = item.value("type").toInt() == 0 || item.value("type").toInt() == 4 ? "sell" : "buy";
                    orders.push_back(item_order);
                }
            }
            parsed_result["orders"] = orders;
        }break;
            default: {
                parsed_result["success"] = false;
                parsed_result["error"] = "Error! Parsing is not defined for method "+QString::number( method );
                break;
            }
        }
    }
    else {
        parsed_result["success"] = false;
        parsed_result["error"] = "Error. Unknown response json type!";
    }
    parsed_result["raw"] = QString( data );
    emit resultParsed( uuid,  parsed_result );
}

QString SwiftApiParserZB::orderStatus(int status) const
{
    switch (status) {
    case 1: return "3";
    case 2: return "2";
    case 3: return "1";
    default: return "4";
    }
}

QString SwiftApiParserZB::withdrawStatus(int status) const
{
    if(status == 2 || status == 5) {
        return "completed";
    } else if(status == 0) {
        return "pending";
    } else {
        return QString::number( status );
    }
}

QString SwiftApiParserZB::depositStatus(int status) const
{
    switch (status) {
    case 0: return "pending";
    case 2: return "completed";
    default: return "";
    }
}

