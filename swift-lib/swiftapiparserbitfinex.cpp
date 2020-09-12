#include "swiftapiparserbitfinex.h"

SwiftApiParserBitfinex::SwiftApiParserBitfinex(QObject *parent) : SwiftApiParser(parent)
{

}


QString SwiftApiParserBitfinex::getExchangeName() const
{
    return "bitfinex";
}

void SwiftApiParserBitfinex::parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonDocument json_data( QJsonDocument::fromJson( data ) );
    if ( json_data.isArray() ) {
        if ( json_data.array().at(0).toString() == "error" ) {
            QJsonObject ret;
            ret["success"] = false;
            ret["async_uuid"] = QString::number( uuid );
            ret["error"] = QString::number( json_data.array().at(1).toInt() ) + " : " + json_data.array().at(2).toString();
            emit resultParsed( uuid,  ret );
            return;
        } else {
            QJsonObject j_ret;
            j_ret["success"] = true;
            QJsonArray j_resp( json_data.array() );
            if ( SwiftApiClient::AsyncMethods::GetBalances == method ) {
                QJsonArray balances_arr;
                for ( auto it = j_resp.begin(); it != j_resp.end(); it++ ) {
                    const QJsonArray j_curr( it->toArray( ) );
                    QJsonObject j_item;
                    /*
                    WALLET_TYPE,
                    CURRENCY,
                    BALANCE,
                    UNSETTLED_INTEREST,
                    BALANCE_AVAILABLE,
                    */
                    const QString currency_name( j_curr.at(1).toString()  );
                    const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                    if ( currency_id > 0  ) {
                        j_item["currency_id"] = QString::number( currency_id );
                        j_item["name"] = currency_name;
                        j_item["total"] = QString::number( j_curr.at(2).toDouble(), 'f', 8 );
                        j_item["available"] = QString::number( j_curr.at(2).toDouble(0), 'f', 8 );
                        j_item["in_orders"] = QString::number( j_curr.at(3).toDouble(0), 'f', 8 );
                        j_item["reserved"] = QString::number( 0 );
                        j_item["pending"] = QString::number( 0, 'f', 8 );
                        j_item["today_in_trades"] = QString::number( 0 );
                        j_item["required"] = QString::number( 0 );
                        balances_arr.push_back( j_item );
                    }
                }
                j_ret["success"] = true;
                j_ret["async_uuid"] = QString::number( uuid );
                j_ret["balances"] = balances_arr;
                emit resultParsed( uuid, j_ret );
                return;
            }
            else if(SwiftApiClient::AsyncMethods::GetCurrencies == method){

            } else if(SwiftApiClient::AsyncMethods::GetMarkets == method){

            } else if(SwiftApiClient::AsyncMethods::GetOrderbooks == method){

            } else if(SwiftApiClient::AsyncMethods::OrderPlace == method){
                j_ret = parseBitfinexOrder(j_resp.at(4).toArray().at(0).toArray());
            } else if(SwiftApiClient::AsyncMethods::OrderCancel == method){
                j_ret = parseBitfinexOrder(j_resp.at(4).toArray());
            } else if(SwiftApiClient::AsyncMethods::OrderGet == method){
                auto order = unpackArray(j_resp, [=](const QJsonArray& src) -> const QJsonObject { return parseBitfinexOrder(src); });
                if(!order.empty())
                    j_ret = order.cbegin()->toObject();
            } else if(SwiftApiClient::AsyncMethods::OrderReplace == method){

            } else if(SwiftApiClient::AsyncMethods::WithdrawGetLimits == method){

            } else if(SwiftApiClient::AsyncMethods::WithdrawList == method || SwiftApiClient::AsyncMethods::WithdrawHistory == method ){
                j_ret["withdraws"] = unpackArrayIf(j_resp,
                                               [=](const QJsonArray& src) -> bool { return (src.at(12).toDouble() < 0);},
                                               [=](const QJsonArray& src) -> const QJsonObject { return parseBitfinexMove(src); });
            } else if(SwiftApiClient::AsyncMethods::WithdrawCancel == method){

            } else if(SwiftApiClient::AsyncMethods::WithdrawCreate == method){
                auto bitfinex_withdraw = j_resp.at(4).toArray();
                j_ret["exchange_id"] = QString::number( getExchangeId() );
                j_ret["type"] = "external";
                if ( j_resp.at(7).isString() && j_resp.at(7).toString().size() > 5 ) {
                    j_ret["success"] = j_resp.at(6).toString() == "SUCCESS";
                    j_ret["error"] = j_resp.at(7).toString();
                } else {
                    j_ret["status"] = j_resp.at(6).toString();
                }
                j_ret["remote_id"] = j_resp.at(0).toString();
                j_ret["amount"] = QString::number( bitfinex_withdraw.at(5).toDouble(), 'f', 8 );
                j_ret["fee"] = QString::number( bitfinex_withdraw.at(8).toDouble(), 'f', 8 );
                j_ret["deposit_address"] = bitfinex_withdraw.at(6).toString();
            } else if(SwiftApiClient::AsyncMethods::WithdrawInner == method){

            } else if(SwiftApiClient::AsyncMethods::WithdrawGetFee == method){

            } /*else if(SwiftApiClient::AsyncMethods::GetBalances == method){
                j_ret["balances"] = unpackArrayIf(j_resp,
                                                [=](const QJsonArray& src) -> bool { return (src.at("AMOUNT").toDouble() > 0);},
                                                [=](const QJsonArray& src) -> const QJsonObject { return parseBitfinexWallet(src); });
            }*/ else if(SwiftApiClient::AsyncMethods::GetDeposits == method){
                j_ret["deposits"] = unpackArrayIf(j_resp,
                                               [=](const QJsonArray& src) -> bool { return (src.at(12).toDouble() > 0);},
                                               [=](const QJsonArray& src) -> const QJsonObject { return parseBitfinexMove(src); });
            } else if(SwiftApiClient::AsyncMethods::GetDepositAddress == method){
                QJsonArray deposit = j_resp.at(4).toArray();
                const QString currency_name( deposit.at(2).toString()  );
                const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
                j_ret["exchange_id"] = QString::number( currency_id );
                j_ret["currency_id"] = QString::number( currency_id );
                j_ret["deposit_address"] = deposit.at(4).toString();
            } else if(SwiftApiClient::AsyncMethods::TradeHistory == method){
                j_ret["orders"] = unpackArray(j_resp, [=](const QJsonArray& src) -> const QJsonObject { return parseBitfinexOrder(src); });
            } else if(SwiftApiClient::AsyncMethods::TradeOpenOrders == method){
                j_ret["orders"] = unpackArray(j_resp, [=](const QJsonArray& src) -> const QJsonObject { return parseBitfinexOrder(src); });
            } else if(SwiftApiClient::AsyncMethods::TradeGetFee == method){

            } else if(SwiftApiClient::AsyncMethods::TradeGetLimits == method){

            } else {
                j_ret["error"] = "UNKNOWN METHOD PARSING"+QString::number( method );
            }

            j_ret["async_uuid"] = QString::number( uuid );
            emit resultParsed( uuid, j_ret );
        }
    } else {
        QJsonObject ret;
        ret["success"] = false;
        ret["async_uuid"] = QString::number( uuid );
        ret["error"] = "Unknown response format";
        emit resultParsed( uuid,  ret );
        return;
    }
}


const QJsonArray SwiftApiParserBitfinex::unpackArray(const QJsonArray &data, const ParsingArrayToObj func) const {
    QJsonArray dst;
    for(auto it = data.cbegin(); it != data.cend(); ++it)
        dst.push_back(func(it->toArray()));
    return dst;
}

const QJsonArray SwiftApiParserBitfinex::unpackArray(const QJsonObject &data, const ParsingArrayToObj func) const {
    QJsonArray dst;
    for(auto it = data.constBegin(); it != data.constEnd(); ++it)
        dst.push_back(func(it->toArray()));
    return dst;
}

const QJsonArray SwiftApiParserBitfinex::unpackArrayIf(const QJsonArray &data, ConditionFunction condition, const ParsingArrayToObj func) const {
    QJsonArray dst;
    for(auto it = data.cbegin(); it != data.cend(); ++it){
        auto obj = it->toArray();
        if(condition(obj))
            dst.push_back(func(obj));
    }
    return dst;
}

const QJsonArray SwiftApiParserBitfinex::unpackArrayIf(const QJsonObject &data, ConditionFunction condition, const ParsingArrayToObj func) const {
    QJsonArray dst;
    for(auto it = data.constBegin(); it != data.constEnd(); ++it){
        auto obj = it->toArray();
        if(condition(obj))
            dst.push_back(func(obj));
    }
    return dst;
}

const QJsonObject SwiftApiParserBitfinex::parseBitfinexOrder(const QJsonArray& src) const {
    QJsonObject result;
    const QString market_name = src.at(3).toString();
    result["market_id"] = QString::number(SwiftLib::getAssets()->getMarketIdByName( market_name, getExchangeId() ));
    result["remote_id"] = QString::number( src.at(0).toVariant().toUInt() );
    double amount = src.at(6).toDouble();
    double price = src.at(16).toDouble();
    result["amount"] = QString::number( abs(src.at(7).toDouble()), 'f', 8);
    result["amount_left"] = QString::number(abs(amount), 'f', 8);
    result["type"] = (amount >= 0) ? "buy" : "sell";
    result["rate"] = QString::number(price, 'f', 8);
    result["price"] = QString::number(abs(price * amount), 'f', 8);
    result["exchange_id"] = QString::number( getExchangeId() );
    QString status( src.at(13).toString() );
    result["status"] = status == "ACTIVE" ? "1" : status == "PARTIALLY" ? "1" : status == "EXECUTED" ? "2" : "3";
    result["created_at"] = QString::number(src.at(4).toVariant().toLongLong() );
    result["updated_at"] = QString::number(src.at(5).toVariant().toLongLong() );
    return result;
}

const QJsonObject SwiftApiParserBitfinex::parseBitfinexMove(const QJsonArray& src) const {
    QJsonObject result;
    const QString currency_name(src.at(1).toString());
    const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
    const QString status = src.at(9).toString();
    result["currency_id"] = QString::number( currency_id );
    result["exchange_id"] = QString::number( getExchangeId() );
    result["created_at"] = QString::number(src.at(5).toVariant().toULongLong() / 1000 );
    if(status.contains("done"))
        result["confirmed_at"] = QString::number(src.at(6).toVariant().toULongLong() / 1000 );
    else
        result["confirmed_at"] = result["recieved_at"];
    result["status"] = status;
    result["remote_id"] = QString::number( src.at(0).toVariant().toUInt() );
    result["tx"] = src.at(20).toString();
    // ? item["local_id"] = _move.at(20).toString();
    result["amount"] = QString::number( src.at(12).toDouble(), 'f',8 ).replace("-","");
    result["fee"] = QString::number( src.at(13).toDouble(), 'f', 8).replace("-",""); // ?
    result["deposit_address"] = src.at(16).toString();
    return result;
}

const QJsonObject SwiftApiParserBitfinex::parseBitfinexWallet(const QJsonArray &src) const {
    QJsonObject result;
    /*
    WALLET_TYPE,
    CURRENCY,
    BALANCE,
    UNSETTLED_INTEREST,
    BALANCE_AVAILABLE,
    */
    const QString currency_name( src.at(1).toString()  );
    const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
    if ( currency_id > 0  ) {
        result["currency_id"] = QString::number( currency_id );
        result["name"] = currency_name;
        result["total"] = QString::number( src.at(2).toDouble(), 'f', 8 );
        result["available"] = QString::number( src.at(4).toDouble(0), 'f', 8 );
        result["in_orders"] = QString::number( src.at(3).toDouble(0), 'f', 8 );
        result["reserved"] = QString::number( 0 );
        result["pending"] = QString::number( 0, 'f', 8 );
        result["today_in_trades"] = QString::number( 0 );
        result["required"] = QString::number( 0 );
    }
    return result;
}

const QJsonObject SwiftApiParserBitfinex::parseBitfinexDeposit(const QJsonArray &src) const {
    Q_UNUSED(src)
    QJsonObject result;
    /*const QString currency_name( src.value("Currency").toString()  );
    const quint32 currency_id = SwiftLib::getAssets()->getCurrencyIdByName( currency_name, getExchangeId() );
    double balance = src.value("BALANCE").toDouble();
    double balance_av = src.value("BALANCE_AVAILABLE").toDouble();
    result["currency_id"] = QString::number( currency_id );
    result["name"] = currency_name;
    result["total"] = QString::number(balance, 'f', 8);
    result["available"] = QString::number(balance_av, 'f', 8); // documentation problem
    result["in_orders"] = QString::number(balance - balance_av, 'f', 8);
    result["reserved"] = QString::number( 0 );
    result["pending"] = QString::number( 0 );
    result["today_in_trades"] = QString::number( 0 );
    result["required"] = QString::number( 0 );*/
    return result;
}


