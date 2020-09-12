#include "swiftapiclientokcoin.h"

SwiftApiClientOkcoin::SwiftApiClientOkcoin(QObject* parent) : SwiftApiClient(parent)
{
    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional(); // pass_phrase
}

QString SwiftApiClientOkcoin::getExchangeName() const{
    return "okcoin";
}

QString SwiftApiClientOkcoin::getApiVersionString(){
    return "api-v3-art";
}

void SwiftApiClientOkcoin::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
    // replace with unrelizedMethod from lakebtcapi branch
}

void SwiftApiClientOkcoin::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
    // replace with unrelizedMethod from lakebtcapi branch
}

void SwiftApiClientOkcoin::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid){
    QString url_str(url_private + path_spot + "instruments/");
    if(j_params.contains("market_id"))
        url_str += SwiftLib::getAssets()->getMarketName(j_params.value("market_id").toString().toUInt());
    else {
        addError("[getOrderbooks] Absent params: <market_id>");
        return;
    }
    QUrl url(url_str + "/book");

    QUrlQuery args;
    if(j_params.contains("size"))
        args.addQueryItem("size", j_params.value("size").toString());
    if(j_params.contains("depth"))
        args.addQueryItem("depth", j_params.value("depth").toString());
    url.setQuery(args);

    getManager()->get(QNetworkRequest(url))->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid){
    OrderParams check(j_params);
    if(!check.isValid()){
        addError("[orderPlace] Order params are invalid or absent!");
        return;
    }
    QUrl url(url_private + path_spot + "orders");
    QUrlQuery body({
                       {"side", j_params.value("type").toString()},
                       {"instrument_id", SwiftLib::getAssets()->getMarketName(check.market_id)}
                   });
    if(j_params.contains("order_type")){
        QString order_type = j_params.value("order_type").toString();
        if(order_type.contains("market", Qt::CaseInsensitive)){
            body.addQueryItem("type", "market");
            //body.addQueryItem("size", j_params.value("amount").toString());
            //body.addQueryItem("notional", j_params.value("rate").toString());
        }
    }
    else {
        body.addQueryItem("type", "limit");
        body.addQueryItem("price", j_params.value("rate").toString());
        body.addQueryItem("size", j_params.value("amount").toString());
    }

    if(j_params.contains("remote_id"))
        body.addQueryItem("client_oid", check.remote_id);

    sendPostAuth(url, body)->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    if(!j_params.contains("market_id") || !j_params.contains("remote_id")){
        addError("[orderCancel] Absent params: <market_id, remote_id>");
        return;
    }
    QUrl url(url_private + path_spot + "cancel_orders/" + j_params.value("remote_id").toString());
    QUrlQuery body({
                       {"instrument_id", SwiftLib::getAssets()->getMarketName(j_params.value("market_id").toString().toUInt())}
                   });
    sendPostAuth(url, body)->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::orderGet(const QJsonObject &j_params, const quint64 &async_uuid){
    if(!j_params.contains("market_id") || !j_params.contains("remote_id")){
        addError("[orderGet] Absent params: <market_id, remote_id>");
        return;
    }
    QUrl url(url_private + path_spot + "orders/" + j_params.value("remote_id").toString());
    url.setQuery({{"instrument_id", SwiftLib::getAssets()->getMarketName(j_params.value("market_id").toString().toUInt())}});

    sendGetAuth(url, url.query().toUtf8())->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
    // replace with unrelizedMethod from lakebtcapi branch
}

void SwiftApiClientOkcoin::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
    // replace with unrelizedMethod from lakebtcapi branch
}

void SwiftApiClientOkcoin::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid){
    withdrawHistory(j_params, async_uuid);
}

void SwiftApiClientOkcoin::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    QString url_str(url_private + path_account + "withdrawal/history");
    if(j_params.contains("currency_id"))
        url_str += "/" + SwiftLib::getAssets()->getCurrencyName(j_params.value("currency_id").toString().toUInt());

    sendGetAuth(QUrl(url_str))->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
    // replace with unrelizedMethod from lakebtcapi branch
}

void SwiftApiClientOkcoin::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid){
    if(!j_params.contains("currency_id") || !j_params.contains("amount") || !j_params.contains("destination")
            || !j_params.contains("to_address") || !j_params.contains("trade_pwd") || !j_params.contains("fee")){
        addError("[withdrawCreate] Absent params: <currency_id, amount, destination, to_address, trade_pwd, fee>");
        return;
    }
    QUrl url(url_private + path_account + "withdrawal");
    QUrlQuery body({
                       {"currency", SwiftLib::getAssets()->getCurrencyName(j_params.value("currency_id").toString().toUInt())},
                       {"amount", j_params.value("amount").toString()},
                       {"destination", j_params.value("destination").toString()},
                       {"to_address", j_params.value("to_address").toString()},
                       {"trade_pwd", j_params.value("trade_pwd").toString()},
                       {"fee", j_params.value("fee").toString()}
                   });
    sendPostAuth(url, body)->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid){
    if(!j_params.contains("currency_id") || !j_params.contains("amount")
            || !j_params.contains("from") || !j_params.contains("to")){
        addError("[withdrawInner] Absent params: <currency_id, amount, from, to>");
        return;
    }

    QUrl url(url_private + path_account + "transfer");
    QUrlQuery body({
                       {"currency", SwiftLib::getAssets()->getCurrencyName(j_params.value("currency_id").toString().toUInt())},
                       {"amount", j_params.value("amount").toString()},
                       {"from", j_params.value("from").toString()},
                       {"to", j_params.value("to").toString()},
                   });
    if(j_params.contains("sub_account"))
        body.addQueryItem("sub_account", j_params.value("sub_account").toString());
    if(j_params.contains("instrument_id"))
        body.addQueryItem("instrument_id", j_params.value("instrument_id").toString());
    if(j_params.contains("to_instrument_id"))
        body.addQueryItem("to_instrument_id", j_params.value("to_instrument_id").toString());

    sendPostAuth(url, body)->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    QUrl url(url_private + path_account + "withdrawal/fee");
    if(j_params.contains("currency_id"))
        url.setQuery({{ "currency", SwiftLib::getAssets()->getCurrencyName(j_params.value("currency_id").toString().toUInt())}});

    sendGetAuth(url, url.query().toUtf8())->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::getBalances(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    QUrl url(url_private + path_account + "wallet");
    sendGetAuth(url)->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    QString url_str(url_private + path_account + "deposit/history");
    if(j_params.contains("currency_id"))
        sendGetAuth(QUrl(url_str + "/" + j_params.value("currency_id").toString()))->setProperty("uuid", async_uuid);
    else
        sendGetAuth(QUrl(url_str))->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid){
    if(!j_params.contains("currency_id")){
        addError("[getDepositAddress] Absent params: <currency_id,>");
        return;
    }
    QUrl url(url_private + path_account + "deposit/address");
    url.setQuery({{ "currency", SwiftLib::getAssets()->getCurrencyName(j_params.value("currency_id").toString().toUInt())}});
    sendGetAuth(url, url.query().toUtf8())->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    if(!j_params.contains("market_id") || !j_params.contains("limit")){
        addError("[tradeHistory] Absent params: <market_id, limit>");
        return;
    }
    QUrl url(url_private + path_spot + "orders");
    int limit = j_params.value("limit").toString().toInt();
    if(limit > 100 || limit < 1)
        limit = 50;

    QUrlQuery body({
                     { "instrument_id", SwiftLib::getAssets()->getMarketName(j_params.value("market_id").toString().toUInt())},
                     {"state", "2"},
                     {"limit", QString::number(limit)}
                 });
    if(j_params.contains("after"))
        body.addQueryItem("after", j_params.value("after").toString());
    else if(j_params.contains("before"))
        body.addQueryItem("before", j_params.value("before").toString());
    url.setQuery(body);
    sendGetAuth(url, body.toString().toUtf8())->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid){
    if(!j_params.contains("market_id")){
        addError("[tradeOpenOrders] Absent params: <market_id, >");
        return;
    }
    QUrl url(url_private + path_spot + "orders_pending");
    QUrlQuery body({
                     { "instrument_id", SwiftLib::getAssets()->getMarketName(j_params.value("market_id").toString().toUInt())},
                 });
    if(j_params.contains("after"))
        body.addQueryItem("after", j_params.value("after").toString());
    else if(j_params.contains("before"))
        body.addQueryItem("before", j_params.value("before").toString());
    if(j_params.contains("limit")){
        int limit = j_params.value("limit").toString().toInt();
        if(limit <= 100 && limit > 0)
            body.addQueryItem("limit", j_params.value("limit").toString());
    }
    url.setQuery(body);
    sendGetAuth(url, body.toString().toUtf8())->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    QUrl url(url_private + path_spot + "trade_fee");
    sendGetAuth(url)->setProperty("uuid", async_uuid);
}

void SwiftApiClientOkcoin::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
    // replace with unrelizedMethod from lakebtcapi branch
}

QNetworkRequest SwiftApiClientOkcoin::buildAuth(const QUrl &url, const QByteArray& method, const QByteArray& body) const {
    auto timestamp = QString::number(double(QDateTime::currentMSecsSinceEpoch())/1000, 'f', 3).toUtf8();

    // Signature -> HMAC-SHA256 ( timestamp + method + reqPath + POST data ) and base64 decoded secret API key
    const auto signature = QMessageAuthenticationCode::hash(
                timestamp + method + url.path().toUtf8() + body,
                api_secret.toUtf8(),
                QCryptographicHash::Sha256
                ).toBase64();

    QNetworkRequest request(url);
    // Headers
    request.setRawHeader("Content-Type","application/json");
    request.setRawHeader("OK-ACCESS-KEY", api_key.toUtf8());
    request.setRawHeader("OK-ACCESS-SIGN", signature);
    request.setRawHeader("OK-ACCESS-TIMESTAMP", timestamp);
    request.setRawHeader("OK-ACCESS-PASSPHRASE", api_user.toUtf8());
    return request;
}

QNetworkReply * SwiftApiClientOkcoin::sendPostAuth(const QUrl& url, QUrlQuery& body) {
    return getManager()->post(
                std::move( buildAuth(url, "POST", body.toString().toUtf8() )),
                body.toString().toUtf8());
}

QNetworkReply * SwiftApiClientOkcoin::sendGetAuth(const QUrl& url, const QByteArray& body) {
    if(body.isEmpty())
        return getManager()->get(std::move(buildAuth(url, "GET")));
    return getManager()->get(std::move(buildAuth(url, "GET", "?" + body)));
}

inline QString SwiftApiClientOkcoin::sideToStr(int side) const {
    switch (side) {
        case 0: return "sell";
        case 1: return "buy";
    }
    return "undefined";
}
