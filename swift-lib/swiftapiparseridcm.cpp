#include "swiftapiparseridcm.h"

SwiftApiParserIDCM::SwiftApiParserIDCM(QObject *parent) : SwiftApiParser(parent)
{
}


QString SwiftApiParserIDCM::getExchangeName() const
{
    return "idcm";
}

void SwiftApiParserIDCM::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonDocument j_doc = QJsonDocument::fromJson( data );
    QJsonObject parsed_result;
    parsed_result["async_uuid"] = QString::number( uuid );
    if(j_doc.isObject()) {
        // ok
        const QJsonObject j_obj = j_doc.object();
        parsed_result["success"] = false;
        if(j_obj.value("result").toInt() == 0){
            parsed_result["error"] = "Error! Code:" + j_obj.value("code").toString() + " - Message:";
        }
        else {
            parsed_result["success"] = true;
            switch(method) {
            case SwiftApiClient::AsyncMethods::GetOrderbooks: {
                QJsonObject data = j_obj.value("data").toObject();

                auto _func = [&](const QJsonArray& arr) -> QJsonArray {
                    QJsonArray out;
                    for(auto it = arr.cbegin(); it != arr.cend(); ++it){
                        auto item = it->toObject();
                        out.push_back(QJsonArray({
                             QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId())),
                             QString::number(item.value("price").toDouble(), 'f', 8),
                             QString::number(item.value("amount").toDouble(), 'f', 8)
                       }));
                    }
                    return out;
                };
                parsed_result["a"] = std::move(_func(data.value("asks").toArray()));
                parsed_result["b"] = std::move(_func(data.value("bids").toArray()));
            } break;
            case SwiftApiClient::AsyncMethods::OrderPlace: {
                parsed_result["remote_id"] = j_obj.value("data").toObject().value("orderid").toString();
            } break;
            case SwiftApiClient::AsyncMethods::OrderCancel: {
                if(j_obj.value("data").toBool())
                    parsed_result["status"] = "3";
            } break;
            case SwiftApiClient::AsyncMethods::OrderGet: {
                QJsonObject data(j_obj.value("data").toObject()); // !!!  obj or arr
                double filled = data.value("executedamount").toDouble();
                parsed_result["created_at"] = data.value("timestamp").toString(); // !!! /1000
                parsed_result["updated_at"] = data.value("timestamp").toString();
                parsed_result["remote_id"] = data.value("orderid").toString();
                parsed_result["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName(data.value("symbol").toString(), getExchangeId()));
                parsed_result["amount"] = QString::number(data.value("amount").toDouble(), 'f', 8);
                parsed_result["rate"] = QString::number(data.value("price").toDouble(), 'f', 8);  // !!! price or avgprice
                parsed_result["status"] = orderStatus(data.value("status").toInt());
                parsed_result["amount_left"] = QString::number(data.value("amount").toDouble() - filled, 'f', 8);
                parsed_result["type"] = data.value("side").toInt() == 0 ? "buy" : "sell";

            } break;
            case SwiftApiClient::AsyncMethods::WithdrawList:
            case SwiftApiClient::AsyncMethods::WithdrawHistory: {
                QJsonArray withdraws;
                QJsonObject data(j_obj.value("data").toObject());
                    //QJsonObject item(it->toObject());
                    withdraws.push_back(QJsonObject({
                            {"created_at", QString::number(quint64(data.value("createtime").toDouble() / 1000)) },
                            {"confirmed_at", QJsonValue(0) },
                            {"remote_id", data.value("withdrawid").toString() },
                            {"amount", QString::number(data.value("amount").toDouble(), 'f', 8) },
                            {"exchange_id", QString::number(getExchangeId()) },
                            {"status", withdrawStatus(data.value("status").toInt()) },
                            {"tx", QJsonValue(0) },
                            {"deposit_address", data.value("address").toString() },
                            {"currency_id", QJsonValue(0) }
                    }));
                parsed_result["withdraws"] = withdraws;
            }break;
            case SwiftApiClient::AsyncMethods::WithdrawCancel: {

            }break;
            case SwiftApiClient::AsyncMethods::WithdrawCreate: {
                parsed_result["remote_id"] = j_obj.value("data").toString(); // !!! Check
            } break;
            case SwiftApiClient::AsyncMethods::GetBalances: {
                QJsonArray balances;
                QJsonArray data = j_obj.value("data").toArray();
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item(it->toObject());
                    auto currency_id = SwiftLib::getAssets()->getCurrencyIdByName(item.value("code").toString(), getExchangeId());
                    if(currency_id == 0)
                        continue;
                    auto free = item.value("free").toDouble();
                    auto freezed = item.value("freezed").toDouble();
                    balances.push_back(QJsonObject({
                            {"currency_id", QString::number(currency_id) },
                            {"exchange_id", QString::number(getExchangeId()) },
                            {"available", QString::number(free, 'f', 8 )},
                            {"total", QString::number(free + freezed, 'f', 8) },
                            {"in_orders", QString::number(freezed, 'f', 8) }
                    }));
                }
                parsed_result["balances"] = balances;
            }break;
            case SwiftApiClient::AsyncMethods::TradeHistory: {
                QJsonArray orders, data(j_obj.value("data").toArray());
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item(it->toObject());
                    double filled = item.value("executedamount").toDouble();
                    orders.push_back(QJsonObject({
                         {"created_at", item.value("timestamp").toString()}, // !!! /1000
                         {"updated_at", item.value("timestamp").toString()},
                         {"remote_id", item.value("orderid").toString()},
                         {"market_id", QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId()))},
                         {"amount", QString::number(item.value("amount").toDouble(), 'f', 8)},
                         {"rate", QString::number(item.value("price").toDouble(), 'f', 8)},  // !!! price or avgprice
                         {"status", orderStatus(item.value("status").toInt())},
                         {"amount_left", QString::number(item.value("amount").toDouble() - filled, 'f', 8)},
                         {"type", item.value("side").toInt() == 0 ? "buy" : "sell"}
                    }));
                }
                parsed_result["orders"] = orders;
            }break;
            case SwiftApiClient::AsyncMethods::TradeOpenOrders: {
                QJsonArray orders, data(j_obj.value("data").toArray());
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item(it->toObject());
                    double filled = item.value("executedamount").toDouble();
                    orders.push_back(QJsonObject({
                        {"created_at", item.value("timestamp").toString()}, // !!! /1000
                        {"updated_at", item.value("timestamp").toString()},
                        {"remote_id", item.value("orderid").toString()},
                        {"market_id", QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId()))},
                        {"amount", QString::number(item.value("amount").toDouble(), 'f', 8)},
                        {"rate", QString::number(item.value("price").toDouble(), 'f', 8)},  // !!! price or avgprice
                        {"status", orderStatus(item.value("status").toInt())},
                        {"amount_left", QString::number(item.value("amount").toDouble() - filled, 'f', 8)},
                        {"type", item.value("side").toInt() == 0 ? "buy" : "sell"}
                    }));
                }
                parsed_result["orders"] = orders;
            }break;
            default: {
                parsed_result["success"] = QJsonDocument::fromJson( data ).object().value("result").toString().toUInt() == 1;
                parsed_result["raw"] = QString( data );

                parsed_result["error"] = "Error! Parsing is not defined for method "+QString::number( method );
                break;
            }
            }
        }
    }
    else {
        // expected json object in response
        parsed_result["success"] = false;
        parsed_result["error"] = "Error. Unknown response json type!";
    }
    emit resultParsed( uuid,  parsed_result );
}

QString SwiftApiParserIDCM::orderStatus(int status) const
{
    switch (status) {
    case -2: return "3";
    case -1: return "4";
    case 0: return "1";
    case 1:
    case 2: return "2"; // !!! if case 3 ?
    default: return "4";
    }
}

QString SwiftApiParserIDCM::withdrawStatus(int status) const
{
    switch (status) {
    case 12: return "completed";
    case 1:
    case 2:
    case 3:
    case 8:
    case 9:
    case 10:
    case 11: return "pending";
    default: return "";
    }
}
