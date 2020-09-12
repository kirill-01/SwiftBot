#include "swiftapiparserlakebtc.h"

SwiftApiParserLakebtc::SwiftApiParserLakebtc(QObject* parent) : SwiftApiParser(parent) {

}


QString SwiftApiParserLakebtc::getExchangeName() const
{
    return "lakebtc";
}

void SwiftApiParserLakebtc::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonDocument json_data( QJsonDocument::fromJson( data ) );
    QJsonObject ret;
    if(json_data.isObject()){
        QJsonObject j_resp = json_data.object();
        // ??? В доке не описан формат ошибки
        if(j_resp.empty() || j_resp.contains("error")){
            ret["success"] = false;
            ret["async_uuid"] = QString::number( uuid );
            ret["error"] = "Request error";
            emit resultParsed( uuid,  ret );
            return;
        }
        else {
            switch(method){
                case SwiftApiClient::AsyncMethods::GetCurrencies: break;
                case SwiftApiClient::AsyncMethods::GetMarkets: break;
                case SwiftApiClient::AsyncMethods::GetOrderbooks:{
                    auto asks = j_resp["asks"].toArray();
                    auto bids = j_resp["bids"].toArray();
                    QJsonArray order_list_ask, order_list_bid;
                    for(auto it = asks.cbegin(); it != asks.end(); ++it){
                        QJsonArray order_item, item = it->toArray();
                        order_item.push_back(0);
                        order_item.push_back(item.at(0).toString());
                        order_item.push_back(item.at(1).toString());
                        order_list_ask.push_back(order_item);
                    }
                    ret["a"] = order_list_ask;
                    for(auto it = bids.cbegin(); it != bids.end(); ++it){
                        QJsonArray order_item, item = it->toArray();
                        order_item.push_back(0);
                        order_item.push_back(item.at(0).toString());
                        order_item.push_back(item.at(1).toString());
                        order_list_bid.push_back(order_item);
                    }
                    ret["b"] = order_list_bid;
                }
                    break;
                case SwiftApiClient::AsyncMethods::OrderPlace:{
                    ret["remote_id"] = j_resp.value("id").toString();
                    // ??? при размещении ордера какие значения в поле статус?
                    ret["status"] = j_resp["result"].toString() == "order received" ? 1 : 4;
                }
                    break;
                case SwiftApiClient::AsyncMethods::OrderCancel:{
                    ret["status"] = j_resp["result"].toBool() ? 3 : 4;
                }
                    break;
                case SwiftApiClient::AsyncMethods::OrderReplace: break;
                case SwiftApiClient::AsyncMethods::WithdrawGetLimits: break;
                case SwiftApiClient::AsyncMethods::WithdrawList: break;
                case SwiftApiClient::AsyncMethods::WithdrawHistory: break;
                case SwiftApiClient::AsyncMethods::WithdrawCancel: break;
                case SwiftApiClient::AsyncMethods::WithdrawCreate:{
                    ret["exchange_id"] = QString::number(getExchangeId());
                    ret["currency_id"] = QString::number(SwiftLib::getAssets()->getCurrencyIdByName(j_resp.value("currency").toString(), getExchangeId()));
                    ret["remote_id"] = j_resp.value("id").toString();
                    ret["amount"] = j_resp.value("amount").toString();
                    ret["fee"] = j_resp.value("fee").toString();
                    ret["status"] = j_resp.value("state").toString();
                    // ??? пример 3322070951
                    //ret[""] = j_resp.value("external_account_id").toString();
                    ret["created_at"] = j_resp.value("at").toString();
                } break;
                case SwiftApiClient::AsyncMethods::WithdrawInner: break;
                case SwiftApiClient::AsyncMethods::WithdrawGetFee: break;
                case SwiftApiClient::AsyncMethods::GetBalances:{
                    QJsonObject balances = j_resp.value("balance").toObject();
                    QJsonObject locked = j_resp.value("locked").toObject();
                    QMap<QString, double> balance_map;
                    QJsonArray balance_arr;
                    for(auto it = locked.constBegin(); it != locked.constEnd(); ++it)
                        balance_map[it.key()] = it.value().toString().toDouble();
                    for(auto it = balances.constBegin(); it != balances.constEnd(); ++it){
                        // ??? total or available
                        QJsonObject item;
                        item["exchange_id"] = QString::number(getExchangeId());
                        item["currency_id"] = QString::number(SwiftLib::getAssets()->getCurrencyIdByName(it.key(), getExchangeId()));
                        double total = it.value().toString().toDouble();
                        item["total"] = it.value().toString();
                        item["available"] = QString::number(abs(total - balance_map[it.key()]), 'f', 8);
                        item["in_orders"] = QString::number(balance_map[it.key()], 'f', 8);
                        balance_arr.push_back(item);
                    }
                    ret["balances"] = balance_arr;
                } break;
                case SwiftApiClient::AsyncMethods::GetDeposits: break;
                case SwiftApiClient::AsyncMethods::GetDepositAddress:{
                    QJsonObject info = j_resp.value("profile").toObject();
                    QJsonArray addresses_arr;
                    for(auto it = info.constBegin(); it != info.constEnd(); ++it){
                        if(it.key().contains("deposit_addres", Qt::CaseInsensitive)){
                            auto deposit_str = it.key().split("_", QString::SkipEmptyParts).at(0);
                            addresses_arr.push_back(QJsonObject({
                                                                    {"currency_id", QString::number(SwiftLib::getAssets()->getCurrencyIdByName(deposit_str, getExchangeId()))},
                                                                    {"deposit_address", it.value().toString()}
                                                                }));
                        }
                    }
                    ret["deposit_addresses"] = addresses_arr;
                } break;
                case SwiftApiClient::AsyncMethods::TradeGetFee: break;
                case SwiftApiClient::AsyncMethods::TradeGetLimits: break;
                default: {
                    ret["error"] = "UNKNOWN METHOD PARSING"+QString::number( method );
                    break;
                }
            }
            ret["success"] = true;
            ret["async_uuid"] = QString::number( uuid );
            emit resultParsed( uuid,  ret );
            return;
        }
    }
    else if(json_data.isArray()){
        QJsonArray j_resp = json_data.array();

        switch(method){
            case SwiftApiClient::AsyncMethods::OrderGet:{
                for(auto it = j_resp.cbegin(); it != j_resp.cend(); ++it){
                    QJsonObject order_item, item = it->toObject();
                    double amount = item.value("original_amount").toString().toDouble();
                    double amount_fill = item.value("amount").toString().toDouble();
                    ret["created_at"] = item.value("at").toString();
                    ret["remote_id"] = item.value("id").toString();
                    ret["amount"] = item.value("original_amount").toString();
                    ret["rate"] = item.value("price").toString();
                    // ??? нет списка значений, по доке известно только - active
                    auto status = item.value("state").toString();
                    if(status.contains("active", Qt::CaseInsensitive))
                        ret["status"] = 1;
                    else if(status.contains("execute", Qt::CaseInsensitive))
                        ret["status"] = 2;
                    else if(status.contains("cancel", Qt::CaseInsensitive))
                        ret["status"] = 3;
                    else order_item["status"] = 4;
                    ret["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId()));
                    ret["amount_left"] = QString::number(abs(amount - amount_fill), 'f', 8);
                    ret["type"] = item.value("type").toString();
                    break;
                }
            } break;
            case SwiftApiClient::AsyncMethods::TradeHistory:{
                QJsonArray orders_list;
                for(auto it = j_resp.cbegin(); it != j_resp.cend(); ++it){
                    QJsonObject order_item, item = it->toObject();
                    double amount = item.value("total").toString().toDouble();
                    double amount_fill = item.value("amount").toString().toDouble();
                    order_item["created_at"] = item.value("at").toString();
                    order_item["amount"] = item.value("total").toString();
                    order_item["amount_left"] = QString::number(abs(amount - amount_fill), 'f', 8);
                    //order_item["rate"] = item.value("price").toString(); Отсутствует вообще
                    if(amount != amount_fill)
                        order_item["status"] = 2; // or 1 ???
                    else
                        order_item["status"] = 2;
                    order_item["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId()));
                    if(item.value("type").toString() == "ask")
                        order_item["type"] = "sell";
                    else
                        order_item["type"] = "buy";
                    orders_list.push_back(order_item);
                }
                ret["orders"] = orders_list;
            } break;
            case SwiftApiClient::AsyncMethods::TradeOpenOrders:{
                QJsonArray orders_list;
                for(auto it = j_resp.cbegin(); it != j_resp.cend(); ++it){
                    QJsonObject order_item, item = it->toObject();
                    order_item["created_at"] = item.value("at").toString();
                    order_item["remote_id"] = item.value("id").toString();
                    order_item["amount"] = item.value("amount").toString();
                    order_item["rate"] = item.value("price").toString();
                    order_item["status"] = 1;
                    order_item["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId()));
                    order_item["type"] = item.value("type").toString();
                    orders_list.push_back(order_item);
                }
                ret["orders"] = orders_list;
            } break;
            default: {
                ret["error"] = "UNKNOWN METHOD PARSING"+QString::number( method );
                break;
            }
        }

        ret["success"] = true;
        ret["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid,  ret );
        return;
    }
    else{
        // Error unexpect format
        ret["success"] = false;
        ret["async_uuid"] = QString::number( uuid );
        ret["error"] = "Unexpectable response format!";
        emit resultParsed( uuid,  ret );
        return;
    }
}
