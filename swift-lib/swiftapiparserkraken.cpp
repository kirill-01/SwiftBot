#include "swiftapiparserkraken.h"

SwiftApiParserKraken::SwiftApiParserKraken(QObject *parent) : SwiftApiParser(parent)
{

}

QString SwiftApiParserKraken::getExchangeName() const
{
    return "kraken";
}

void SwiftApiParserKraken::parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonDocument json_data( QJsonDocument::fromJson( data ) );
    QJsonObject result;
    if(json_data.isObject()){
        QJsonObject j_resp = json_data.object();
        if(j_resp.contains("error") && j_resp.value("error").toArray().isEmpty()){
            result = j_resp.value("result").toObject();
        }
        else {
            // Error request
            QJsonObject ret;
            ret["success"] = false;
            ret["async_uuid"] = QString::number( uuid );
            ret["method"] = QString::number( method );
            ret["error"] = [](const QJsonArray& arr) -> QString {
                QString out;
                for(auto it = arr.cbegin(); it != arr.cend(); ++it)
                    out.append(it->toString());
                return out;
            }(j_resp.value("error").toArray());
            emit resultParsed( uuid,  ret );
            return;
        }
    }
    else {
        // Error expect JsonObject
        QJsonObject ret;
        ret["success"] = false;
        ret["method"] = QString::number( method );
        ret["async_uuid"] = QString::number( uuid );
        ret["error"] = "Response format is not JsonObject (maybe it JsonArray)";
        emit resultParsed( uuid,  ret );
        return;
    }
    QJsonObject j_ret;

    if(method == SwiftApiClient::AsyncMethods::GetCurrencies){
        QJsonArray currencies_list;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            QJsonObject item = it.value().toObject();
            QJsonObject currency_obj;
            currency_obj["exchange_id"] = QString::number(getExchangeId());
            currency_obj["name"] = it.key();
            currency_obj["precision"] = QString::number(item.value("decimals").toInt());
            currencies_list.push_back(currency_obj);
        }
        j_ret["currencies"] = currencies_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::GetMarkets){
        QJsonArray pair_list;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            QJsonObject item = it.value().toObject();
            QJsonObject pair_obj;
            pair_obj["exchange_id"] = QString::number(getExchangeId());
            pair_obj["name"] = it.key();
            pair_obj["market_id"] = QString::number(GetMarketId(it.key()));
            pair_obj["base_currency_id"] = QString::number(item.value("base").toInt());
            pair_obj["market_currency_id"] = QString::number(item.value("quote").toInt());
            // fee имеет формат не числа в ответе
            //obj["trade_fee"] = ;
            // неясно
            //obj["min_trade_size"] = ;
            pair_list.push_back(pair_obj);
        }
        j_ret["markets"] = pair_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::GetOrderbooks){
        QJsonArray all_asks, all_bids;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            auto assetInfo = it.value().toObject();
            auto pairId = GetMarketId(it.key());
            QJsonArray asks(assetInfo.value("asks").toArray()), bids(assetInfo.value("bids").toArray());
            for(auto it2 = asks.constBegin(); it2 != asks.constEnd(); ++it2) {
                auto _ask_item = it2->toObject();
                QJsonArray _item;
                _item.append(QString::number(pairId));
                _item.append(QString::number(_ask_item.value("price").toDouble(), 'f', 8));
                _item.append(QString::number(_ask_item.value("volume").toDouble(), 'f', 8));
                all_asks.push_back(_item);
            }
            for(auto it2 = bids.constBegin(); it2 != bids.constEnd(); ++it2) {
                auto _bid_item = it2->toObject();
                QJsonArray _item;
                _item.append(QString::number(pairId));
                _item.append(QString::number(_bid_item.value("price").toDouble(), 'f', 8));
                _item.append(QString::number(_bid_item.value("volume").toDouble(), 'f', 8));
                all_bids.push_back(_item);
            }
        }
        j_ret["a"] = all_asks;
        j_ret["b"] = all_bids;
    }
    else if(method == SwiftApiClient::AsyncMethods::OrderPlace){
        auto id_arr = result.value("txid").toArray();
        if(!id_arr.empty()) {
            if (result.value("descr").toObject().contains("close"))
                j_ret["status"] = "2";
            else
                j_ret["status"] = "1";

            const QString descr = result.value("descr").toObject().value("order").toString();
            auto descr_data = descr.split(" ", QString::SkipEmptyParts);
            j_ret["remote_id"] = id_arr.at(0);
            //j_ret["local_id"] = ;
            j_ret["amount"] = descr_data.at(1);
            //j_ret["amount_left"] = ;
            j_ret["type"] = descr_data.at(0);
            j_ret["rate"] = descr_data.at(5);
            j_ret["price"] = QString::number(descr_data.at(5).toDouble() * descr_data.at(1).toDouble(), 'f', 8);
            //j_ret["fee"] = ;
            //j_ret["base_currency_id"] = ;
            //j_ret["market_currency_id"] = ;

            //j_ret["recieved_at"] = QString::number(item.value("opentm").toInt());
            //j_ret["confirmed_at"] = ;
        }
        else {
            j_ret["status"] = "3";
        }
    }
    else if(method == SwiftApiClient::AsyncMethods::OrderCancel){
        if(result.value("count").toString().toInt() == 1)
            j_ret["status"] = "3";
        else j_ret["status"] = "1";
    }
    else if(method == SwiftApiClient::AsyncMethods::OrderGet){
        QJsonArray order_list;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            QJsonObject item = it.value().toObject();
            QJsonObject order_item, item_desc(item.value("descr").toObject());
            order_item["market_id"] = QString::number(GetMarketId(item_desc.value("pair").toString()));
            order_item["remote_id"] = it.key();
            order_item["status"] = orderStatus(item.value("status").toString());
            order_item["type"] = item_desc.value("type").toString();
            order_item["rate"] = item_desc.value("price").toString();
            //order_item["local_id"] = ;
            order_item["amount"] = item.value("vol").toString();
            order_item["amount_left"] = QString::number(
                        item.value("vol").toString().toDouble() - item.value("vol_exec").toString().toDouble(),
                        'f', 8);
            //order_item["price"] = QString::number(item.value("price").toDouble() * item.value("").toDouble(), 'f', 8);
            //order_item["fee"] = ;
            //order_item["base_currency_id"] = ;
            //order_item["market_currency_id"] = ;
            order_item["recieved_at"] = item.value("opentm").toString();
            if(order_item["status"] == "2")
                order_item["confirmed_at"] = item.value("closetm").toString();
            order_list.push_back(order_item);
        }
        j_ret["orders"] = order_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::OrderReplace){

    }
    else if(method == SwiftApiClient::AsyncMethods::WithdrawGetLimits){

    }
    else if(method == SwiftApiClient::AsyncMethods::WithdrawList){
        QJsonArray withdraw_list;
        const QJsonArray a_result( json_data.object().value("result").toArray() );
        for(auto it = a_result.constBegin(); it != a_result.constEnd(); it++){
            QJsonObject item = it->toObject();
            QJsonObject withdraw_item;
            withdraw_item["exchange_id"] = QString::number(getExchangeId());
            withdraw_item["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item.value("asset").toString(), getExchangeId()) );
            withdraw_item["created_at"] = QString::number(item.value("time").toInt());
            withdraw_item["status"] = ioStatus(item.value("status").toString());
            withdraw_item["tx"] = item.value("txid").toString();
            withdraw_item["remote_id"] = item.value("refid").toString();
            withdraw_item["amount"] = QString::number(item.value("amount").toString().toDouble(), 'f', 8);
            withdraw_item["fee"] = QString::number( item.value("fee").toString().toDouble(), 'f', 8);
            withdraw_item["deposit_address"] = item.value("info").toString();
            withdraw_list.push_back(withdraw_item);
        }
        j_ret["withdraws"] = withdraw_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::WithdrawHistory){
        QJsonArray withdraw_list;
        const QJsonArray a_result( json_data.object().value("result").toArray() );
        for(auto it = a_result.constBegin(); it != a_result.constEnd(); it++){
            QJsonObject item = it->toObject();
            QJsonObject withdraw_item;
            withdraw_item["exchange_id"] = QString::number(getExchangeId());
            withdraw_item["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item.value("asset").toString(), getExchangeId()) );
            withdraw_item["created_at"] = QString::number(item.value("time").toInt());
            withdraw_item["status"] = ioStatus(item.value("status").toString());
            withdraw_item["tx"] = item.value("txid").toString();
            withdraw_item["remote_id"] = item.value("refid").toString();
            withdraw_item["amount"] = QString::number(item.value("amount").toString().toDouble(), 'f', 8);
            withdraw_item["fee"] = QString::number( item.value("fee").toString().toDouble(), 'f', 8);
            withdraw_item["deposit_address"] = item.value("info").toString();
            withdraw_list.push_back(withdraw_item);
        }
        j_ret["withdraws"] = withdraw_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::WithdrawCancel){
        // ??? просто возвращает bool
        j_ret["exchange_id"] = QString::number(getExchangeId());
        //withdraw_item["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item.value("asset").toString(), getExchangeId()) );
        //withdraw_item["created_at"] = QString::number(item.value("time").toInt());
        //withdraw_item["confirmed_at"] = ;
        //withdraw_item["status"] = item.value("status").toString();
        j_ret["type"] = "external";
        //withdraw_item["remote_id"] = QString::number(item.value("refid").toInt());
        //withdraw_item["local_id"] = ;
        //withdraw_item["amount"] = QString::number(item.value("amount").toInt());
        //withdraw_item["fee"] = QString::number(item.value("fee").toInt());
        // ??? не нашёл либо refid
        //withdraw_item["deposit_address"] = QString::number(item.value("refid").toInt());
    }
    else if(method == SwiftApiClient::AsyncMethods::WithdrawCreate){
        QJsonObject withdraw_item;

        // ??? Возвращает лишь refid
        j_ret["exchange_id"] = QString::number(getExchangeId());
        //withdraw_item["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item.value("asset").toString(), getExchangeId()) );
        //withdraw_item["created_at"] = QString::number(item.value("time").toInt());
        //withdraw_item["confirmed_at"] = ;
        //withdraw_item["status"] = item.value("status").toString();
        j_ret["type"] = "external";
        j_ret["remote_id"] = QString::number(result.value("refid").toInt());
        //withdraw_item["local_id"] = ;
        //withdraw_item["amount"] = QString::number(item.value("amount").toInt());
        //withdraw_item["fee"] = QString::number(item.value("fee").toInt());
        // ??? не нашёл либо refid
        //withdraw_item["deposit_address"] = QString::number(item.value("refid").toInt());
    }
    else if(method == SwiftApiClient::AsyncMethods::WithdrawInner){
        QJsonObject withdraw_item;
        // ??? Возвращает лишь refid
        j_ret["exchange_id"] = QString::number(getExchangeId());
        //j_ret["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item.value("asset").toString(), getExchangeId()) );
        //j_ret["from"] = ;
        //j_ret["to"] = ;
        //j_ret["type"] = "internal";
        //j_ret["amount"] = QString::number(item.value("amount").toInt());

    }
    else if(method == SwiftApiClient::AsyncMethods::WithdrawGetFee){

    }
    else if(method == SwiftApiClient::AsyncMethods::GetBalances){
        QJsonArray balance_list;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            QJsonObject balance_item;
            QJsonObject item = it.value().toObject();
            balance_item["currency_id"] = QString::number(SwiftLib::getAssets()->getCurrencyIdByName(it.key(), getExchangeId()));
            balance_item["total"] = QString::number(it.value().toString().toDouble(), 'f', 8);
            balance_item["exchange_id"] = QString::number(getExchangeId());
            balance_item["available"] = QString::number(it.value().toString().toDouble(), 'f', 8);
            balance_item["in_orders"] = "0";
            balance_item["reserved"] = "0";
            balance_item["pending"] = "0";
            balance_list.push_back(balance_item);
        }
        j_ret["balances"] = balance_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::GetDeposits){
        const QJsonArray a_result( json_data.object().value("result").toArray() );
        QJsonArray deposit_list;
        for(auto it = a_result.constBegin(); it != a_result.constEnd(); it++){
            QJsonObject item = it->toObject();
            QJsonObject deposit_item;
            deposit_item["created_at"] = item.value("time").toString();
            deposit_item["remote_id"] = item.value("refid").toString();
            deposit_item["amount"] = QString::number(item.value("amount").toString().toDouble(), 'f', 8);
            deposit_item["exchange_id"] = QString::number(getExchangeId());
            deposit_item["status"] = ioStatus(item.value("status").toString());
            deposit_item["tx"] = item.value("txid").toString();
            deposit_item["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item.value("asset").toString(), getExchangeId()) );
            deposit_item["deposit_address"] = item.value("info").toString();
            deposit_list.push_back(deposit_item);
        }
        j_ret["deposits"] = deposit_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::GetDepositAddress){
        QJsonArray adress_list;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            QJsonObject adress_item;
            QJsonObject item = it.value().toObject();
            adress_item["exchange_id"] = QString::number(getExchangeId());
            //deposit_item["currency_id"] = QString::number();
            //deposit_item["received_at"] = ;
            //deposit_item["confirmed_at"] = ;
            //deposit_item["status"] = ;
            //deposit_item["remote_id"] = ;
            //deposit_item["local_id"] = ;
            adress_item["amount"] = QString::number(item.value("limit").toDouble(), 'f', 8);
            adress_item["fee"] = QString::number(item.value("fee").toDouble(), 'f', 8);
            // ??? не очевидно что address-setup-fee это адресс
            //deposit_item["deposit_address"] = item.value("address-setup-fee").toString();
            adress_list.push_back(adress_item);
        }
        j_ret["depositAddress"] = adress_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::TradeHistory){
        QJsonArray orders_list;
        QJsonObject trades = result.value("closed").toObject();
        for(auto it = trades.constBegin(); it != trades.constEnd(); ++it) {
            QJsonObject order_item;
            QJsonObject item = it.value().toObject();
            QJsonObject item_desc(item.value("descr").toObject() );
            order_item["created_at"] = QString::number(item.value("opentm").toVariant().toULongLong() );
            order_item["updated_at"] = QString::number(item.value("closetm").toVariant().toULongLong() );
            order_item["exchange_id"] = QString::number( getExchangeId() );
            order_item["exchange_name"] = getExchangeName();
            order_item["remote_id"] = it.key();
            if ( item.value("userref").toVariant().toULongLong() > 0 ) {
                order_item["local_id"] = item.value("userref").toString();
            }
            order_item["market_id"] = QString::number( GetMarketId( item_desc.value("pair").toString() ) );
            order_item["amount"] = item.value("vol").toString();
            if ( item.value("limitprice").toString().toDouble() > 0 ) {
                order_item["rate"] = item.value("limitprice").toString();
            }  else {
                order_item["rate"] = item.value("price").toString();
            }
            order_item["status"] = orderStatus(item.value("status").toString());
            order_item["amount_left"] = QString::number(
                        item.value("vol").toString().toDouble() - item.value("vol_exec").toString().toDouble(),
                        'f', 8);
            order_item["type"] = item_desc.value("type").toString();
            orders_list.push_back(order_item);
        }
        j_ret["orders"] = orders_list;
    }
    else if(method == SwiftApiClient::AsyncMethods::TradeOpenOrders){
        QJsonArray orders_list;
        QJsonObject trades = result.value("open").toObject();
        for(auto it = trades.constBegin(); it != trades.constEnd(); ++it){
            QJsonObject order_item;
            QJsonObject item = it.value().toObject();
            QJsonObject item_desc(item.value("descr").toObject() );
            order_item["created_at"] = QString::number(item.value("opentm").toVariant().toULongLong() );
            order_item["updated_at"] = QString::number(item.value("closetm").toVariant().toULongLong() );
            order_item["exchange_id"] = QString::number( getExchangeId() );
            order_item["exchange_name"] = getExchangeName();
            order_item["remote_id"] = it.key();
            if ( item.value("userref").toVariant().toULongLong() > 0 ) {
                order_item["local_id"] = QString::number( item.value( "userref" ).toVariant().toULongLong() );
            }
            order_item["market_id"] = QString::number( GetMarketId( item_desc.value("pair").toString() ) );
            order_item["amount"] = item.value("vol").toString();
            order_item["rate"] = item_desc.value("price").toString();
            order_item["status"] = orderStatus(item.value("status").toString());
            order_item["amount_left"] = QString::number(
                        item.value("vol").toString().toDouble() - item.value("vol_exec").toString().toDouble(),
                        'f', 8);
            order_item["type"] = item_desc.value("type").toString();
            orders_list.push_back(order_item);
        }
        j_ret["orders"] = orders_list;
    } else if(method == SwiftApiClient::AsyncMethods::TradeGetFee){
        QJsonArray pair_list;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            QJsonObject item = it.value().toObject();
            QJsonObject pair_obj;
            pair_obj["market_id"] = QString::number(GetMarketId(it.key()));
            pair_obj["fee"] = QString::number(item.value("fees").toArray().at(1).toDouble(), 'f', 8);
            pair_list.push_back(pair_obj);
        }
        j_ret["markets"] = pair_list;
    } else if(method == SwiftApiClient::AsyncMethods::TradeGetLimits){
        QJsonArray balance_list;
        for(auto it = result.constBegin(); it != result.constEnd(); ++it){
            QJsonObject balance_item;
            QJsonObject item = it.value().toObject();
            QString asset = it.key();
            balance_item["currency_id"] = QString::number(SwiftLib::getAssets()->getCurrencyIdByName(asset, getExchangeId()));
            balance_item["name"] = asset;
            balance_item["total"] = QString::number(it.value().toDouble(), 'f', 8);
            //balance_item["available"] = ;
            //balance_item["in_orders"] = ;
            //balance_item["reserved"] = ;
            //balance_item["pending"] = ;
            //balance_item["today_in_trades"] = ;
            //balance_item["required"] = ;
            balance_list.push_back(balance_item);
        }
        j_ret["balances"] = balance_list;
    } else {
        j_ret["error"] = "UNKNOWN METHOD PARSING"+QString::number( method );
    }
    j_ret["success"] = true;
    j_ret["async_uuid"] = QString::number( uuid );
    emit resultParsed( uuid, j_ret );
}

int SwiftApiParserKraken::GetMarketId(const QString& market) const {
    int result = SwiftLib::getAssets()->getMarketIdByName(market, getExchangeId());
    return result;
}

QString SwiftApiParserKraken::orderStatus(const QString& status) const {
    if ( status == "open" || status == "pending" ) {
        return "1";
    } else if ( status == "closed" || status == "partial" ) {
        return "2";
    } else if ( status == "canceled" || status == "expired") {
        return "3";
    } else {
        return "4";
    }
}

QString SwiftApiParserKraken::ioStatus(const QString &status) const {
    if(status.contains("send", Qt::CaseInsensitive))
        return "sending";
    if(status.contains("pend", Qt::CaseInsensitive))
        return "pending";
    if(status.contains("complet", Qt::CaseInsensitive))
        return "completed";
    if(status.contains("success", Qt::CaseInsensitive))
        return "completed";
    return "pending";
}
