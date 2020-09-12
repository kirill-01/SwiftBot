#include "swiftapiparserokcoin.h"

SwiftApiParserOkcoin::SwiftApiParserOkcoin(QObject *parent) : SwiftApiParser(parent)
{

}


QString SwiftApiParserOkcoin::getExchangeName() const
{
    return "okcoin";
}

void SwiftApiParserOkcoin::parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonDocument json_data( QJsonDocument::fromJson( data ) );
    QJsonObject result;

    if(json_data.isObject()){
        //for object as response
        QJsonObject response_obj = json_data.object();
        if(response_obj.contains("error_code") && response_obj.value("error_code").toString().toInt() > 4000){
            result["success"] = false;
            result["async_uuid"] = QString::number( uuid );
            result["error"] = response_obj.value("error_code").toString() +
                    " - " + response_obj.value("error_message").toString();
            resultParsed( uuid, result );
            return;
        }

        result["success"] = true;
        result["async_uuid"] = QString::number( uuid );

        switch(method){
        case SwiftApiClient::AsyncMethods::GetOrderbooks:
        {
            auto bookParsing = [&](const QJsonArray& arr) -> QJsonArray {
                QJsonArray res;
                for(auto it = arr.cbegin(); it != arr.cend(); ++it)
                    res.push_back(QJsonArray({
                                        "0",
                                        it->toArray().at(0).toString(),
                                        it->toArray().at(1).toString()
                                    }));
                return res;
            };

            result["a"] = bookParsing(response_obj.value("asks").toArray());
            result["b"] = bookParsing(response_obj.value("bids").toArray());
            break;
        }
        case SwiftApiClient::AsyncMethods::OrderPlace:
        {
            if(!response_obj.value("result").toBool()) {
                result["remote_id"] = response_obj.value("order_id").toString();
                result["status"] = "1";
            }
            else
                result["status"] = "4";
            break;
        }
        case SwiftApiClient::AsyncMethods::OrderCancel:
        {
            if(!response_obj.value("result").toBool()) {
                result["remote_id"] = response_obj.value("order_id").toString();
                result["status"] = "3";
            }
            else
                result["status"] = "4";
            break;
        }
        case SwiftApiClient::AsyncMethods::OrderGet:
        {
            double amount(response_obj.value("size").toString().toDouble());
            double filled_amount(response_obj.value("filled_size").toString().toDouble());
            QString time_create = fromISOToStr(response_obj.value("timestamp").toString());
            result["created_at"] = time_create;
            //result["updated_at"] = time_create;
            result["remote_id"] = response_obj.value("order_id").toString();
            result["amount"] = response_obj.value("size").toString();
            result["rate"] = response_obj.value("price").toString();
            result["status"] = orderStatusParse(response_obj.value("state").toString().toInt());
            result["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName(response_obj.value("instrument_id").toString(), getExchangeId()));
            result["amount_left"] = QString::number(amount - filled_amount, 'f', 8);
            result["type"] = response_obj.value("side").toString();
            break;
        }
        case SwiftApiClient::AsyncMethods::WithdrawCreate:
        {
            result["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(response_obj.value("currency").toString(), getExchangeId()));
            result["remote_id"] = response_obj.value("withdraw_id").toString();
            result["amount"] = response_obj.value("amount").toString();
            result["status"] = response_obj.value("result").toBool() ? "success" : "failed";
            break;
        }
        case SwiftApiClient::AsyncMethods::WithdrawInner:
        {
            result["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName(response_obj.value("currency").toString(), getExchangeId()));
            result["remote_id"] = response_obj.value("transfer_id").toString();
            result["amount"] = response_obj.value("amount").toString();
            result["status"] = response_obj.value("result").toBool() ? "success" : "failed";
            break;
        }
        case SwiftApiClient::AsyncMethods::TradeGetFee:
        {
            result["maker"] = response_obj.value("maker").toString();
            result["taker"] = response_obj.value("taker").toString();
            break;
        }
        case SwiftApiClient::AsyncMethods::TradeGetLimits: break;
        default: {
            result["error"] = "UNKNOWN METHOD PARSING"+QString::number( method );
            break;
        }
        }
        emit resultParsed( uuid, result );
    }
    else if(json_data.isArray()){
        //for array as response
        QJsonArray response_arr = json_data.array();
        result["success"] = true;
        result["async_uuid"] = QString::number( uuid );

        switch(method){
        case SwiftApiClient::AsyncMethods::WithdrawList:
        case SwiftApiClient::AsyncMethods::WithdrawHistory:
        {
            QJsonArray withdraw;
            for(auto it = response_arr.cbegin(); it != response_arr.cend(); ++it){
                auto item_obj = it->toObject();
                QString time_create = fromISOToStr(item_obj.value("timestamp").toString());
                withdraw.push_back(QJsonObject({
                   //{"created_at", time_create},
                   {"confirmed_at", time_create},
                   {"remote_id", item_obj.value("withdrawal_id").toString()},
                   {"amount", item_obj.value("amount").toString()},
                   {"exchange_id", QString::number(getExchangeId())},
                   {"status", withdrawStatusParse(item_obj.value("status").toString().toInt())},
                   {"tx", item_obj.value("txid").toString()},
                   {"deposit_address", item_obj.value("to").toString()},
                   {"currency_id", QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item_obj.value("currency").toString(), getExchangeId()))}
                }));
            }
            result["withdraws"] = withdraw;
            break;
        }
        case SwiftApiClient::AsyncMethods::WithdrawGetFee:
        {
            QJsonArray fees;
            for(auto it = response_arr.cbegin(); it != response_arr.cend(); ++it){
                auto item_obj = it->toObject();
                fees.push_back(QJsonObject({
                    {"min_fee", item_obj.value("min_fee").toString()},
                    {"max_fee", item_obj.value("max_fee").toString()},
                    {"currency_id", QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item_obj.value("currency").toString(), getExchangeId()))}
                }));
            }
            result["fees"] = fees;
            break;
        }
        case SwiftApiClient::AsyncMethods::GetBalances:
        {
            QJsonArray balances;
            for(auto it = response_arr.cbegin(); it != response_arr.cend(); ++it){
                auto item_obj = it->toObject();
                balances.push_back(QJsonObject({
                    {"currency_id", QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item_obj.value("currency").toString(), getExchangeId()))},
                    {"exchange_id", QString::number(getExchangeId())},
                    {"total", item_obj.value("balance").toString()},
                    {"available", item_obj.value("available").toString()},
                }));
            }
            result["balances"] = balances;
            break;
        }
        case SwiftApiClient::AsyncMethods::GetDeposits:
        {
            QJsonArray deposits;
            for(auto it = response_arr.cbegin(); it != response_arr.cend(); ++it){
                auto item_obj = it->toObject();
                QString time_create = fromISOToStr(item_obj.value("timestamp").toString());
                deposits.push_back(QJsonObject({
                   //{"created_at", time_create},
                   {"confirmed_at", time_create},
                   {"remote_id", item_obj.value("deposit_id").toString()},
                   {"amount", item_obj.value("amount").toString()},
                   {"exchange_id", QString::number(getExchangeId())},
                   {"status", depositStatusParse(item_obj.value("status").toString().toInt())},
                   {"tx", item_obj.value("txid").toString()},
                   {"deposit_address", item_obj.value("to").toString()},
                   {"currency_id", QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item_obj.value("currency").toString(), getExchangeId()))}
                }));
            }
            result["deposits"] = deposits;
            break;
        }
        case SwiftApiClient::AsyncMethods::GetDepositAddress:
        {
            QJsonArray addr;
            for(auto it = response_arr.cbegin(); it != response_arr.cend(); ++it){
                auto item_obj = it->toObject();
                addr.push_back(QJsonObject({
                   {"deposit_address", item_obj.value("address").toString()},
                   {"currency_id", QString::number( SwiftLib::getAssets()->getCurrencyIdByName(item_obj.value("currency").toString(), getExchangeId()))},
                   {"account_type", depositTypeParse(item_obj.value("to").toString().toInt())}
                }));
            }
            result["addresses"] = addr;
            break;
        }
        case SwiftApiClient::AsyncMethods::TradeHistory:
        {
            QJsonArray orders;
            for(auto it = response_arr.cbegin(); it != response_arr.cend(); ++it){
                auto item_obj = it->toObject();
                double amount(item_obj.value("size").toString().toDouble());
                double filled_amount(item_obj.value("filled_size").toString().toDouble());
                QString time_create = fromISOToStr(item_obj.value("timestamp").toString());
                orders.push_back(QJsonObject({
                   {"created_at", time_create},
                   //{"updated_at", time_create},
                   {"remote_id", item_obj.value("order_id").toString()},
                   {"market_id", QString::number( SwiftLib::getAssets()->getMarketIdByName(item_obj.value("instrument_id").toString(), getExchangeId()))},
                   {"amount", item_obj.value("size").toString()},
                   {"rate", item_obj.value("price").toString()},
                   {"status", orderStatusParse(item_obj.value("state").toString().toInt())},
                   {"amount_left", QString::number(amount - filled_amount, 'f', 8)},
                   {"type", item_obj.value("side").toString()}
                }));
            }
            result["orders"] = orders;
            break;
        }
        case SwiftApiClient::AsyncMethods::TradeOpenOrders:
        {
            QJsonArray orders;
            for(auto it = response_arr.cbegin(); it != response_arr.cend(); ++it){
                auto item_obj = it->toObject();
                double amount(item_obj.value("size").toString().toDouble());
                double filled_amount(item_obj.value("filled_size").toString().toDouble());
                QString time_create = fromISOToStr(item_obj.value("timestamp").toString());
                orders.push_back(QJsonObject({
                   {"created_at", time_create},
                   //{"updated_at", time_create},
                   {"remote_id", item_obj.value("order_id").toString()},
                   {"market_id", QString::number( SwiftLib::getAssets()->getMarketIdByName(item_obj.value("instrument_id").toString(), getExchangeId()))},
                   {"amount", item_obj.value("size").toString()},
                   {"rate", item_obj.value("price").toString()},
                   {"status", "1"},
                   {"amount_left", QString::number(amount - filled_amount, 'f', 8)},
                   {"type", item_obj.value("side").toString()}
                }));
            }
            result["orders"] = orders;
            break;
        }
        case SwiftApiClient::AsyncMethods::TradeGetLimits: break;
        default: {
            result["error"] = "UNKNOWN METHOD PARSING"+QString::number( method );
            break;
        }
        }
        emit resultParsed( uuid, result );
    }
    else {
        // Error expect JsonObject
        result["success"] = false;
        result["async_uuid"] = QString::number( uuid );
        result["error"] = "Response format is not JsonObject or JsonArray";
        emit resultParsed( uuid,  result );
        return;
    }
}

QString SwiftApiParserOkcoin::orderStatusParse(int status) const {
    switch (status) {
        case -2: return "4";    // Failed
        case -1: return "3";    // Canceled
        case 0: return "1";     // Open
        case 1: return "1";     // Partially Filled
        case 2: return "2";     // Fully Filled
        case 3: return "2";     // Submitting
        case 4: return "3";     // Canceling
    }
    return "4";
}

QString SwiftApiParserOkcoin::withdrawStatusParse(int status) const {
    switch (status) {
        //case -3: return "pending"; // pending cancel;
        //case -2: return "cancel"; // cancelled;
        //case -1: return "failed"; // failed;
        case 0: return "pending"; // pending;
        case 1: return "pending"; // sending;
        case 2: return "completed"; // sent;
        case 3: return "pending"; // awaiting email verification;
        case 4: return "pending"; // awaiting manual verification;
        case 5: return "pending"; // awaiting identity verification
    }
    return "";
}

QString SwiftApiParserOkcoin::depositStatusParse(int status) const {
    switch (status) {
        case 0: return "pending"; // waiting for confirmation;
        case 1: return "completed"; // deposit credited;
        case 2: return "completed"; // deposit successful;
        //case 3: return "failed"; // deposit failed;
        case 4: return "pending"; // waiting for confirmation;
        case 5: return "completed"; // deposit successful
    }
    return "";
}

QString SwiftApiParserOkcoin::depositTypeParse(int status) const {
    switch (status) {
        case 0: return "sub"; // sub account;
        case 1: return "spot"; // spot;
        case 5: return "margin"; // margin;
        case 6: return "funding"; // Funding Account;
    }
    return "unknown";
}

QString SwiftApiParserOkcoin::fromISOToStr(const QString& time_iso) const {
    return QString::number(QDateTime::fromString(time_iso, Qt::DateFormat::ISODateWithMs).toMSecsSinceEpoch());
}
