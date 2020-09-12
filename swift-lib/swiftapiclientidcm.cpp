#include "swiftapiclientidcm.h"


SwiftApiClientIDCM::SwiftApiClientIDCM(QObject* parent) : SwiftApiClient(parent)
{
    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
}

QString SwiftApiClientIDCM::getExchangeName() const{
    return "idcm";
}

QString SwiftApiClientIDCM::getApiVersionString(){
    return "api-v1-art";
}

void SwiftApiClientIDCM::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"market_id"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();

        if(isUintValid(market_id, errorMessage, "market_id")){
            QJsonObject params;
            params["Symbol"] = SwiftLib::getAssets()->getMarketName(market_id);
            buildPostRequest_withSignature(rest_url + "getdepth", params, async_uuid);
            return;
        }
    }
    addError("getOrderbooks " + errorMessage);
}

void SwiftApiClientIDCM::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"market_id", "type", "amount", "rate"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();
        auto type = j_params.value("type").toString();
        auto amount = j_params.value("amount").toString().toDouble();
        auto price = j_params.value("rate").toString().toDouble();

        if(isUintValid(market_id, errorMessage, "market_id")
                && isSideValid(type, errorMessage)
                && isDoublePlus(amount, errorMessage, "amount")
                && isDoublePlus(price, errorMessage, "rate")){
            QJsonObject params({
                {"Symbol", SwiftLib::getAssets()->getMarketName(market_id)},
                {"Size", amount},
                {"Price", price},
                {"Side", type == "sell" ? 1 : 0},
                {"Type", 1}
                //{"Amount", amount * price}
            });

            buildPostRequest_withSignature(rest_url + "trade", params, async_uuid);
            return;
        }
    }
    addError("orderPlace " + errorMessage);
}

void SwiftApiClientIDCM::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"market_id", "remote_id", "type"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();
        auto remote_id = j_params.value("remote_id").toString();
        auto type = j_params.value("type").toString();

        if(isUintValid(market_id, errorMessage, "market_id")
                && isSideValid(type, errorMessage)){
            QJsonObject params({
                {"Symbol", SwiftLib::getAssets()->getMarketName(market_id)},
                {"OrderID", remote_id},
                {"Side", type == "sell" ? 1 : 0}
            });

            buildPostRequest_withSignature(rest_url + "cancel_order", params, async_uuid);
            return;
        }
    }
    addError("orderCancel " + errorMessage);
}

void SwiftApiClientIDCM::orderGet(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    QMap<QString, QString> body;
    if(isParamExist({"market_id", "remote_id"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();
        auto remote_id = j_params.value("remote_id").toString();

        if(isUintValid(market_id, errorMessage, "market_id")) {
            QJsonObject params({
                {"Symbol", SwiftLib::getAssets()->getMarketName(market_id)},
                {"OrderID", remote_id}
            });

            buildPostRequest_withSignature(rest_url + "getorderinfo", params, async_uuid);
            return;
        }
    }
    addError("[orderGet] " + errorMessage);
}

void SwiftApiClientIDCM::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid){
    withdrawHistory(j_params, async_uuid);
}

void SwiftApiClientIDCM::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    auto currency_id = j_params.value("currency_id").toString().toUInt();
    auto remote_id = j_params.value("remote_id").toString();
    QJsonObject params({
        {"Symbol", SwiftLib::getAssets()->getCurrencyName(currency_id)},
        {"WithdrawID", remote_id}
    });

    buildPostRequest_withSignature(rest_url + "getwithdrawinfo", params, async_uuid);
}

void SwiftApiClientIDCM::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"currency_id", "remote_id"}, j_params, errorMessage)) {
        auto currency_id = j_params.value("currency_id").toString().toUInt();
        auto remote_id = j_params.value("remote_id").toString();

        if(isUintValid(currency_id, errorMessage, "currency_id")) {
            QJsonObject params({
                {"Symbol", SwiftLib::getAssets()->getCurrencyName(currency_id)},
                {"WithdrawID", remote_id}
            });

            buildPostRequest_withSignature(rest_url + "cancel_withdraw", params, async_uuid);
            return;
        }
    }
    addError("[withdrawCancel] " + errorMessage);
}

void SwiftApiClientIDCM::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid){
    QString errorMessage;
    if(isParamExist({"address", "currency_id", "amount"}, j_params, errorMessage)) {
        auto address = j_params.value("address").toString();
        auto currency_id = j_params.value("currency_id").toString().toUInt();
        auto amount = j_params.value("amount").toString();

        if(isUintValid(currency_id, errorMessage, "currency_id")
                && isDoublePlus(amount.toDouble(), errorMessage, "amount")){
            QJsonObject params({
                {"Symbol", SwiftLib::getAssets()->getCurrencyName(currency_id)},
                {"Address", address},
                {"Amount", amount }
            });

            buildPostRequest_withSignature(rest_url + "withdraw", params, async_uuid);
            return;
        }
    }
    addError("withdrawCreate" + errorMessage);
}

void SwiftApiClientIDCM::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::getBalances(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    QJsonObject params({{ "Method", "Balance" }});
    buildPostRequest_withSignature(rest_url + "getuserinfo", params, async_uuid);
}

void SwiftApiClientIDCM::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    getHistoryOrders(j_params, async_uuid, QJsonArray({ 1, 2}), "tradeHistory");
}

void SwiftApiClientIDCM::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid){
    QJsonArray orderType;
    orderType.push_back(0);
    getHistoryOrders(j_params, async_uuid, orderType, "tradeOpenOrders");
}

void SwiftApiClientIDCM::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientIDCM::getHistoryOrders(const QJsonObject &j_params, const quint64 &async_uuid, const QJsonArray &orderType, const QString &methodName)
{
    QString errorMessage;
    if(isParamExist({"market_id"}, j_params, errorMessage)) {
        auto market_id = j_params.value("market_id").toString().toUInt();
        auto page_limit = int(j_params.value("page_limit").toString("50").toUInt());
        if(isUintValid(market_id, errorMessage, "market_id")){
            QJsonObject params({
                                   {"Symbol", SwiftLib::getAssets()->getMarketName(market_id)},
                                   {"PageIndex", 1},
                                   {"PageSize", page_limit},
                                   {"Status", orderType}
                               });
            buildPostRequest_withSignature(rest_url + "gethistoryorder", params, async_uuid);
            return;
        }
    }
    addError("[" + methodName + "] " + errorMessage);
}

void SwiftApiClientIDCM::buildPostRequest_withSignature(const QString &api_path, const QJsonObject& params, const quint64 &uuid)
{
    const auto input = QJsonDocument(params).toJson(QJsonDocument::Compact);
    const QByteArray signature = QMessageAuthenticationCode::hash(input, api_secret.toUtf8(), QCryptographicHash::Sha384).toBase64();
    QUrl url(api_path);
    QNetworkRequest request(url);

    request.setRawHeader("X-IDCM-APIKEY", api_key.toUtf8());
    request.setRawHeader("X-IDCM-SIGNATURE", signature);
    request.setRawHeader("X-IDCM-INPUT", input);
    request.setRawHeader("Content-Type", "application/json");

    auto reply = getManager()->post(request, input);
    reply->setProperty("uuid", uuid);
}
