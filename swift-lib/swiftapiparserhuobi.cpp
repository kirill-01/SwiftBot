#include "swiftapiparserhuobi.h"

SwiftApiParserHuobi::SwiftApiParserHuobi(QObject *parent) : SwiftApiParser(parent)
{
}


QString SwiftApiParserHuobi::getExchangeName() const
{
    return "huobi";
}

void SwiftApiParserHuobi::parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonDocument j_doc = QJsonDocument::fromJson( data );
    QJsonObject parsed_result;
    parsed_result["async_uuid"] = QString::number( uuid );

    if(j_doc.isObject()){
        // ok
        const QJsonObject j_obj = j_doc.object();
        parsed_result["success"] = false;
        if(j_obj.value("status").toString() != "ok"){
            if(j_obj.value("status").toString() == "error") {
                parsed_result["error"] = "Error! Code:" + j_obj.value("err-code").toString() +" - Message:" + j_obj.value("err-msg").toString();
            }
            else { parsed_result["error"] = "Error! Response is not expected!"; }
        }
        else {
            parsed_result["success"] = true;
            switch(method){
                //case SwiftApiClient::AsyncMethods::GetCurrencies: break;
            case SwiftApiClient::AsyncMethods::TimeSyncMethod: {
                if ( j_obj.value("data").isArray() ) {
                    const QJsonArray j_result_a( j_obj.value("data").toArray() );
                    for( auto it = j_result_a.begin(); it != j_result_a.end(); it++ ) {
                        if ( it->toObject().value("type").toString() == "spot" ) {
                            SwiftLib::getSettings()->setValue( "api_nodes/huobi/username", QString::number( it->toObject().value("id").toInt() ));
                        }
                    }
                }
            } break;
            case SwiftApiClient::AsyncMethods::GetOrderbooks: {
                auto _func = [&](const QJsonArray& arr) -> QJsonArray {
                    QJsonArray out;
                    for(auto it = arr.cbegin(); it != arr.cend(); ++it){
                        auto item = it->toArray();
                        out.push_back(QJsonArray({"", QString::number(item.at(0).toDouble(), 'f', 8), QString::number(item.at(1).toDouble(), 'f', 8)}));
                    }
                    return out;
                };
                auto tick = j_obj.value("tick").toObject();
                parsed_result["a"] = std::move(_func(tick.value("asks").toArray()));
                parsed_result["b"] = std::move(_func(tick.value("bids").toArray()));
            } break;
            case SwiftApiClient::AsyncMethods::OrderPlace: {
                parsed_result["remote_id"] = j_obj.value("data").toString();
            } break;
            case SwiftApiClient::AsyncMethods::OrderCancel: {
                parsed_result["remote_id"] = j_obj.value("data").toString();
                parsed_result["status"] = "3";
            } break;
            case SwiftApiClient::AsyncMethods::OrderGet: {
                QJsonObject data(j_obj.value("data").toObject());
                double filled = data.value("filled-amount").toString().toDouble();
                parsed_result["created_at"] = QString::number(quint64(data.value("created-at").toDouble() / 1000));
                parsed_result["updated_at"] = QString::number(quint64(data.value("finished-at").toDouble() / 1000));
                parsed_result["remote_id"] = QString::number(quint64(data.value("id").toDouble()));
                parsed_result["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName(data.value("symbol").toString(), getExchangeId()));
                parsed_result["amount"] = data.value("amount").toString();
                parsed_result["rate"] = data.value("price").toString();
                parsed_result["status"] = orderStatus(data.value("state").toString());
                parsed_result["amount_left"] = QString::number(data.value("amount").toString().toDouble() - filled, 'f', 8);
                parsed_result["type"] = sideValue(data.value("type").toString());

            } break;
            //case SwiftApiClient::AsyncMethods::OrderReplace: break;
            //case SwiftApiClient::AsyncMethods::WithdrawGetLimits: break;
            case SwiftApiClient::AsyncMethods::WithdrawList:
            case SwiftApiClient::AsyncMethods::WithdrawHistory: {
                QJsonArray withdraws, data(j_obj.value("data").toArray());
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item(it->toObject());
                    withdraws.push_back(QJsonObject({
                            {"created_at", QString::number(quint64(item.value("created-at").toDouble() / 1000)) },
                            {"confirmed_at", QString::number(quint64(item.value("updated-at").toDouble() / 1000)) },
                            {"remote_id", QString::number(quint64(item.value("id").toDouble())) },
                            {"amount", QString::number(item.value("amount").toDouble(), 'f', 8) },
                            {"exchange_id", QString::number(getExchangeId()) },
                            {"status", withdrawStatus(item.value("state").toString()) },
                            {"tx", item.value("tx-hash").toString() },
                            {"deposit_address", item.value("address").toString() },
                            {"currency_id", QString::number(SwiftLib::getAssets()->getCurrencyIdByName(item.value("currency").toString(), getExchangeId())) }
                    }));
                }
                parsed_result["withdraws"] = withdraws;
            }break;
            /*case SwiftApiClient::AsyncMethods::WithdrawCancel: {

            }break;*/
            case SwiftApiClient::AsyncMethods::WithdrawCreate: {
                parsed_result["remote_id"] = QString::number(j_obj.value("data").toDouble());
            } break;
            /*case SwiftApiClient::AsyncMethods::WithdrawInner: break;
            case SwiftApiClient::AsyncMethods::WithdrawGetFee: break;*/
            case SwiftApiClient::AsyncMethods::GetBalances: {
                QJsonArray balances, data(j_obj.value("data").toObject().value("list").toArray());
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item1(it->toObject());
                    it++;
                    QJsonObject item2(it->toObject());
                    auto currency_id = SwiftLib::getAssets()->getCurrencyIdByName(item1.value("currency").toString(), getExchangeId());
                    if(currency_id == 0)
                        continue;
                    balances.push_back(QJsonObject({
                            {"currency_id", QString::number(currency_id) },
                            {"exchange_id", QString::number(getExchangeId()) },
                            {"available", QString::number( item1.value("balance").toString().toDouble() - item2.value("balance").toString().toDouble(), 'f', 8 )},
                            {"total", item1.value("balance").toString() },
                            {"in_orders", item2.value("balance").toString() }
                    }));
                }
                parsed_result["balances"] = balances;
            }break;
            case SwiftApiClient::AsyncMethods::GetDeposits: {
                QJsonArray deposits, data(j_obj.value("data").toArray());
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item(it->toObject());
                    auto currency_id = SwiftLib::getAssets()->getCurrencyIdByName(item.value("currency").toString(), getExchangeId());
                    if(currency_id == 0)
                        continue;
                    deposits.push_back(QJsonObject({
                            {"created_at", QString::number(quint64(item.value("created-at").toDouble() / 1000)) },
                            {"confirmed_at", QString::number(quint64(item.value("updated-at").toDouble() / 1000)) },
                            {"remote_id", QString::number(quint64(item.value("id").toDouble())) },
                            {"amount", QString::number(item.value("amount").toDouble(), 'f', 8) },
                            {"exchange_id", QString::number(getExchangeId()) },
                            {"status", depositStatus(item.value("state").toString()) },
                            {"tx", item.value("tx-hash").toString() },
                            {"deposit_address", item.value("address").toString() },
                            {"currency_id", QString::number(currency_id) }
                    }));
                }
                parsed_result["deposits"] = deposits;
            }break;
            case SwiftApiClient::AsyncMethods::GetDepositAddress: {
                auto j_data_arr = j_obj.value("data").toArray();
                for(auto it = j_data_arr.cbegin(); it != j_data_arr.cend(); ++it) {
                    auto item = it->toObject();
                    parsed_result["address"] = item.value("address").toString();
                    parsed_result["tag"] = item.value("addressTag ").toString();
                    break;
                }
            }break;
            case SwiftApiClient::AsyncMethods::TradeHistory: {
                QJsonArray orders, data(j_obj.value("data").toArray());
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item(it->toObject());
                    double filled = cropDoubleStr(item.value("filled-amount").toString(), true ).toDouble();
                    orders.push_back(QJsonObject({
                            {"created_at", QString::number(quint64(item.value("created-at").toDouble() / 1000)) },
                            {"updated_at", QString::number(quint64(item.value("finished-at").toDouble() / 1000)) },
                            {"remote_id", QString::number(quint64(item.value("id").toDouble())) },
                            {"market_id", QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId())) },
                            {"amount", cropDoubleStr(item.value("amount").toString(), true) },
                            {"rate", cropDoubleStr(item.value("price").toString(), true) },
                            {"status", orderStatus(item.value("state").toString()) },
                            {"amount_left", orderStatus(item.value("state").toString()) == "2" ? "0.0" : QString::number(item.value("amount").toString().toDouble() - filled, 'f', 8) },
                            {"type", sideValue(item.value("type").toString()) },

                    }));
                }
                parsed_result["orders"] = orders;
            }break;
            case SwiftApiClient::AsyncMethods::TradeOpenOrders: {
                QJsonArray orders, data(j_obj.value("data").toArray());
                for(auto it = data.cbegin(); it != data.cend(); ++it){
                    QJsonObject item(it->toObject());
                    double filled = cropDoubleStr(item.value("filled-amount").toString(), true ).toDouble();
                    orders.push_back(QJsonObject({
                            {"created_at", QString::number(quint64(item.value("created-at").toDouble() / 1000)) },
                            {"updated_at", QString::number(quint64(item.value("finished-at").toDouble() / 1000)) },
                            {"remote_id", QString::number(quint64(item.value("id").toDouble())) },
                            {"market_id", QString::number(SwiftLib::getAssets()->getMarketIdByName(item.value("symbol").toString(), getExchangeId())) },
                            {"amount", cropDoubleStr(item.value("amount").toString(), true) },
                            {"rate", cropDoubleStr(item.value("price").toString(), true) },
                            {"status", orderStatus(item.value("state").toString()) },
                            {"amount_left", orderStatus(item.value("state").toString()) == "2" ? "0.0" :  QString::number( cropDoubleStr( item.value("amount").toString(), true).toDouble() - filled, 'f', 8) },
                            {"type", sideValue(item.value("type").toString()) },

                    }));
                }
                parsed_result["orders"] = orders;
            }break;
            case SwiftApiClient::AsyncMethods::TradeGetFee: break;
            case SwiftApiClient::AsyncMethods::TradeGetLimits: break;
            default: {
                parsed_result["success"] = false;
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

QString SwiftApiParserHuobi::orderStatus(const QString &status) const
{
    if(status == "created" || status == "submitted")
        return "1";
    else if(status == "partial-filled" || status == "filled" || status == "partial-canceled")
        return "2";
    else if(status == "cancelling" || status == "canceled")
        return "3";
    else
        return "4";
}

QString SwiftApiParserHuobi::withdrawStatus(const QString &status) const
{
    if(status == "confirmed")
        return "completed";
    else if(status == "verifying" || status == "reexamine"
            || status == "pass" || status == "pre-transfer"
            || status == "wallet-transfer" || status == "submitted")
        return "pending";
    else
        return "";
}

QString SwiftApiParserHuobi::depositStatus(const QString &status) const
{
    if(status == "confirmed" || status == "safe")
        return "completed";
    else if(status == "confirming")
        return "pending";
    else
        return "";
}

QString SwiftApiParserHuobi::sideValue(const QString &side) const
{
    if(side.contains("sell", Qt::CaseInsensitive))
        return "sell";
    else if(side.contains("buy", Qt::CaseInsensitive))
        return "buy";
    else
        return "";
}

QString SwiftApiParserHuobi::cropDoubleStr(const QString &s, bool onlyPos) const
{
    if(onlyPos)
        return QString::number(abs(s.toDouble()), 'f', 8);
    else
        return QString::number(s.toDouble(), 'f', 8);
}
