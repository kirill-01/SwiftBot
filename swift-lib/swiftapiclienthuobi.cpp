#include "swiftapiclienthuobi.h"


SwiftApiClientHuobi::SwiftApiClientHuobi(QObject* parent) : SwiftApiClient(parent)
{
    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
}


QString SwiftApiClientHuobi::getExchangeName() const{
    return "huobi";
}

QString SwiftApiClientHuobi::getApiVersionString(){
    return "api-v2-art";
}

void SwiftApiClientHuobi::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    QNetworkRequest request(QUrl(rest_url + "/v1/common/currencys"));
    request.setRawHeader("Content-Type", "application/application/json");
    QNetworkReply* reply = getManager()->get(request);
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientHuobi::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    QNetworkRequest request(QUrl(rest_url + "/v1/common/symbols"));
    request.setRawHeader("Content-Type", "application/application/json");
    QNetworkReply* reply = getManager()->get(request);
    reply->setProperty("uuid", async_uuid);
}

// api params -> {"market_id":""} Opt[ "page_limit":"", "precision":"" ]
void SwiftApiClientHuobi::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"market_id", "precision"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();
        auto page_limit = j_params.value("page_limit").toString();

        if(isUintValid(market_id, errorMessage, "market_id")){
            QMap<QString, QString> params = {
                {"symbol", SwiftLib::getAssets()->getMarketName(market_id)},
                {"type", "step0"}
            };
            if(!page_limit.isEmpty() && page_limit.toUInt() != 0)
                params["depth"] = page_limit;

            QNetworkRequest request(QUrl(rest_url + "/market/depth?" + paramMapToStr(params)));
            request.setRawHeader("Content-Type", "application/application/json");
            QNetworkReply* reply = getManager()->get(request);
            reply->setProperty("uuid", async_uuid);
            return;
        }
    }
    addError("[getOrderbooks] " + errorMessage);
}

void SwiftApiClientHuobi::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid){
        const quint32 market_id = j_params.value("market_id").toString().toUInt();
        const QString type = j_params.value("type").toString();
        const double amount = j_params.value("amount").toString().toDouble();
        const double price = j_params.value("rate").toString().toDouble();
        const QString _amount = QString::number( amount, 'f', SwiftLib::getAssets()->getMarketAmountPrecision( market_id ) );
        const QString _price = QString::number( price, 'f', SwiftLib::getAssets()->getMarketPricePrecision( market_id ) );

        QUrlQuery body = {
            {"account-id", api_user},
            {"amount", _amount},
            {"price", _price},
            {"symbol", SwiftLib::getAssets()->getMarketName(market_id)},
            {"type", type + "-limit"}
        };

        buildPostRequest_withSignature("/v1/order/orders/place", body, async_uuid);
}
void SwiftApiClientHuobi::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    QUrlQuery body;
    if(isParamExist({"remote_id"}, j_params, errorMessage)) {
        auto remote_id = j_params.value("remote_id").toString();

        buildPostRequest_withSignature("/v1/order/orders/" + j_params.value("remote_id").toString() + "/submitcancel", body, async_uuid);
        return;
    }
    addError("[orderCancel] " + errorMessage);
}

// api params -> {"remote_id":""}
void SwiftApiClientHuobi::orderGet(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    QMap<QString, QString> body;
    if(isParamExist({"remote_id"}, j_params, errorMessage)) {
        buildGetRequest_withSignature("/v1/order/orders/" + j_params.value("remote_id").toString(), body, async_uuid);
        return;
    }
    addError("[orderGet] " + errorMessage);
}

void SwiftApiClientHuobi::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientHuobi::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientHuobi::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid){
    withdrawHistory(j_params, async_uuid);
}

void SwiftApiClientHuobi::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    QMap<QString, QString> params = { {"type", "withdraw"} };
    auto currency_id = j_params.value("currency_id").toString().toUInt();
    auto page_limit = j_params.value("page_limit").toString();
    if(currency_id != 0)
        params["currency"] = SwiftLib::getAssets()->getCurrencyName(currency_id);
    if(page_limit.toUInt() != 0 && isUintInRange(page_limit.toUInt(), 1, 500))
        params["size"] = page_limit;

    buildGetRequest_withSignature("/v1/query/deposit-withdraw", params, async_uuid);
}

void SwiftApiClientHuobi::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    QUrlQuery body;
    if(isParamExist({"remote_id"}, j_params, errorMessage)) {
        buildPostRequest_withSignature("/v1/dw/withdraw-virtual/" + j_params.value("remote_id").toString() + "/cancel", body, async_uuid);
        return;
    }
    addError("[withdrawCancel] " + errorMessage);
}

void SwiftApiClientHuobi::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid){
    QString address = j_params.value("address").toString();
    quint32 currency_id = j_params.value("currency_id").toString().toUInt();
    double amount = j_params.value("amount").toString().toDouble();
    const QString currency_name = SwiftLib::getAssets()->getCurrencyName(currency_id);
    const double fee = SwiftLib::getAssets()->getCurrencyWithdrawFee( currency_id );
    QUrlQuery body = {
        {"address", address},
        {"currency", currency_name},
        {"fee", QString::number(fee, 'f', currency_name == "usdt" ? 6 : 8 )},
        {"amount", QString::number( amount - fee, 'f', currency_name == "usdt" ? 6 : 8 ) }
    };
    if(currency_name == "usdt") {
        body.addQueryItem("chain", "usdterc20");
    }
    buildPostRequest_withSignature("/v1/dw/withdraw/api/create", body, async_uuid);
}

void SwiftApiClientHuobi::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientHuobi::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

// api params -> {}
void SwiftApiClientHuobi::getBalances(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    if ( api_user == "" ) {
        const quint64 asuid = createAsyncWaiter( SwiftApiClient::TimeSyncMethod, QJsonObject() );
      customMethod( QJsonObject(), asuid );
    }
    QMap<QString, QString> params;
    buildGetRequest_withSignature("/v1/account/accounts/" + api_user + "/balance", params, async_uuid);
}

// api params -> Opt[ "currency_id":"", "page_limit":"" ]
void SwiftApiClientHuobi::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid){
    QMap<QString, QString> params = { {"type", "deposit"} };
    auto currency_id = j_params.value("currency_id").toString().toUInt();
    auto page_limit = j_params.value("page_limit").toString();
    if(currency_id != 0)
        params["currency"] = SwiftLib::getAssets()->getCurrencyName(currency_id);
    if(page_limit.toUInt() != 0 && isUintInRange(page_limit.toUInt(), 1, 500))
        params["size"] = page_limit;

    buildGetRequest_withSignature("/v1/query/deposit-withdraw", params, async_uuid);
}

// api params -> {"currency_id":""}
void SwiftApiClientHuobi::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"currency_id"}, j_params, errorMessage)) {
        auto currency_id = j_params.value("currency_id").toString().toUInt();

        if(isUintValid(currency_id, errorMessage, "currency_id")){
            QMap<QString, QString> params = { {"currency", SwiftLib::getAssets()->getCurrencyName(currency_id)} };

            buildGetRequest_withSignature("/v2/account/deposit/address", params, async_uuid);
            return;
        }
    }
    addError("[getDepositAddress] " + errorMessage);
}

// api params -> {"market_id":""} Opt [ "page_limit":"" ]
void SwiftApiClientHuobi::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    auto market_id = j_params.value("market_id").toString().toUInt();
    auto page_limit = j_params.value("page_limit").toString("100");

    QMap<QString, QString> params;
    if(market_id != 0)
        params["symbol"] = SwiftLib::getAssets()->getMarketName(market_id);
    if(page_limit.toUInt() != 0 && isUintInRange(page_limit.toUInt(), 10, 1000))
        params["size"] = page_limit;

    buildGetRequest_withSignature("/v1/order/history", params, async_uuid);
    return;
    addError("[tradeHistory] " + errorMessage);
}

// api params -> {"market_id":""} Opt [ "page_limit":"" ]
void SwiftApiClientHuobi::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"market_id"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();
        auto page_limit = j_params.value("page_limit").toString();

        if(isUintValid(market_id, errorMessage, "market_id")){
            QMap<QString, QString> params = {
                {"account-id", api_user},
                {"symbol", SwiftLib::getAssets()->getMarketName(market_id)},
                //{"states", "submitted,partial-filled,created"}
            };
            if(page_limit.toUInt() != 0 && isUintInRange(page_limit.toUInt(), 1, 100))
                params["size"] = page_limit;

            buildGetRequest_withSignature("/v1/order/openOrders", params, async_uuid);
            return;
        }
    }
    addError("[tradeOpenOrders] " + errorMessage);
}

void SwiftApiClientHuobi::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"market_id"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();

        if(isUintValid(market_id, errorMessage, "market_id")){
            QMap<QString, QString> params = { {"symbol", SwiftLib::getAssets()->getMarketName(market_id)} };

            buildGetRequest_withSignature("/v2/reference/transact-fee-rate", params, async_uuid);
            return;
        }
    }
    addError("[tradeGetFee] " + errorMessage);
}

void SwiftApiClientHuobi::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}


QByteArray SwiftApiClientHuobi::buildSignature(const QByteArray& method, const QByteArray& api_path, const QByteArray& ordered_params) const
{
    QByteArray payload = method + "\napi.huobi.pro\n" + api_path + "\n" + ordered_params;
    return QMessageAuthenticationCode::hash(payload, api_secret.toUtf8(), QCryptographicHash::Sha256).toBase64();
}

void SwiftApiClientHuobi::buildGetRequest_withSignature(const QString& api_path, QMap<QString, QString>& params, const quint64& uuid) {
    params["AccessKeyId"] = api_key;
    params["SignatureMethod"] = "HmacSHA256";
    params["SignatureVersion"] = "2";
    auto date = QDateTime::fromSecsSinceEpoch(QDateTime::currentSecsSinceEpoch(), Qt::UTC).toString(Qt::DateFormat::ISODate);
    date.remove(date.size()-1,1);
    params["Timestamp"] = QUrl::toPercentEncoding(date);
    QByteArray param_u = paramMapToStr(params).toUtf8();

    const auto signature = QUrl::toPercentEncoding(buildSignature("GET", api_path.toUtf8(), param_u));
    QNetworkRequest request(QUrl(rest_url + api_path + "?" + param_u + "&Signature=" + signature));
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    QNetworkReply* reply = getManager()->get(request);
    reply->setProperty("uuid", uuid);
}

void SwiftApiClientHuobi::buildPostRequest_withSignature(const QString& api_path, QUrlQuery &body, const quint64& uuid) {
    auto date = QDateTime::fromSecsSinceEpoch(QDateTime::currentSecsSinceEpoch(), Qt::UTC).toString(Qt::DateFormat::ISODate);
    date.remove(date.size()-1,1);
    QMap<QString, QString> params = {
        {"AccessKeyId", api_key},
        {"SignatureMethod", "HmacSHA256"},
        {"SignatureVersion","2"},
        {"Timestamp", QUrl::toPercentEncoding(date)}
    };
    QByteArray param_u = paramMapToStr(params).toUtf8();
    const auto signature  = QUrl::toPercentEncoding(buildSignature("POST", api_path.toUtf8(), param_u));
    param_u + "&Signature=" + signature;

    QJsonObject j_body;
    for(auto it = body.queryItems().cbegin(); it != body.queryItems().cend(); ++it)
        j_body[it->first] = it->second;

    QNetworkRequest request(QUrl(rest_url + api_path + "?" + param_u + "&Signature=" + signature));
    request.setRawHeader("Content-Type", "application/json");
    QNetworkReply* reply = getManager()->post(request, QJsonDocument ( j_body ).toJson());
    reply->setProperty("uuid", uuid);
}

QString SwiftApiClientHuobi::parseStatus(const QString &stat) {
    if ( stat == "submitted" ) {
        return  "1";
    } else if ( stat == "partial-filled" ) {
        return  "2";
    } else if ( stat == "cancelling" ) {
        return  "3";
    } else if ( stat == "created" ) {
        return  "1";
    } else {
        return  "4";
    }
}

void SwiftApiClientHuobi::customParser(const SwiftApiClient::AsyncMethods &method, const QJsonObject &j_result)
{
    Q_UNUSED( method )
    if ( j_result.value("data").isArray() ) {
        const QJsonArray j_result_a( j_result.value("data").toArray() );
        for( auto it = j_result_a.begin(); it != j_result_a.end(); it++ ) {
            if ( it->toObject().value("type").toString() == "spot" ) {
                api_user = QString::number( it->toObject().value("id").toInt() );
            }
        }
    }
}

void SwiftApiClientHuobi::customMethod(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QMap<QString, QString> p;
    buildGetRequest_withSignature("/v1/account/accounts", p, async_uuid);
}
