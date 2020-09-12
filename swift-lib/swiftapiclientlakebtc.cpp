#include "swiftapiclientlakebtc.h"

SwiftApiClientLakeBTC::SwiftApiClientLakeBTC(QObject* parent) : SwiftApiClient(parent)
{
    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
}

QString SwiftApiClientLakeBTC::getExchangeName() const
{
    return "lakebtc";
}

QString SwiftApiClientLakeBTC::getApiVersionString()
{
    return "api-v2-art";
}

void SwiftApiClientLakeBTC::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    if(paramValidate(j_params, {"market_id"}, true))
        publicGetRequest(
                    {url_path + "/bcorderbook" + buildGetParamStr(j_params, {"symbol"})},
                    async_uuid);
}

void SwiftApiClientLakeBTC::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    if(paramValidate(j_params, {"type", "price", "amount", "market_id"}, true)) {
        switch (j_params.value("type").toInt()) {
        case 0: authPostRequest(url_path, "sellOrder", buildPostParamStr(j_params, {"price", "amount", "symbol"}), async_uuid);
            break;
        case 1: authPostRequest(url_path, "buyOrder", buildPostParamStr(j_params, {"price", "amount", "symbol"}), async_uuid);
            break;
        default:
            addError("Order \'side\' not determinate!");
            return;
        }
    }
}

void SwiftApiClientLakeBTC::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    if(paramValidate(j_params, {"orders_id"}, false))
        authPostRequest(url_path, "cancelOrders", buildPostParamStr(j_params, {"orders_id"}), async_uuid);
}

void SwiftApiClientLakeBTC::orderGet(const QJsonObject &j_params, const quint64 &async_uuid) {
    if(paramValidate(j_params, {"orders_id"}, false))
        authPostRequest(url_path, "getOrders", buildPostParamStr(j_params, {"orders_id"}), async_uuid);
}

void SwiftApiClientLakeBTC::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid){
    // ??? в документации указан "External Account ID". Если подразумевается не адресс кошелька,
    // то нужно определиться с именем, сейчас это address
    if(paramValidate(j_params, {"amount", "currency", "address"}, false))
        authPostRequest(url_path, "createWithdraw", buildPostParamStr(j_params, {"amount", "currency", "address"}), async_uuid);
}

void SwiftApiClientLakeBTC::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::getBalances(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    authPostRequest(QUrl(url_path), "getAccountInfo", "", async_uuid);
}

void SwiftApiClientLakeBTC::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid){
    Q_UNUSED(j_params)
    authPostRequest(url_path, "getAccountInfo", "", async_uuid);
}

void SwiftApiClientLakeBTC::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid){
    // getTrades в доке не указано явно закрытые ли только ордера возвращает
    authPostRequest(url_path, "getTrades", buildPostParamStr(j_params, {"ts"}), async_uuid);
}

void SwiftApiClientLakeBTC::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid){
    if(paramValidate(j_params, {}, false))
        authPostRequest(url_path, "openOrders", "", async_uuid);
}

void SwiftApiClientLakeBTC::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}

void SwiftApiClientLakeBTC::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid){
    unrealizedMethod(j_params, async_uuid);
}




bool SwiftApiClientLakeBTC::paramValidate(const QJsonObject& j_params, const QStringList &mandatoryParams, bool isOrderCheck)
{
    if(isOrderCheck){
        OrderParams orderArgs(j_params);
        if ( !orderArgs.isValid() ) {
             addError("Incorrect order params");
             return false;
        }
    }
    for(auto& name : mandatoryParams){
        if(!j_params.contains(name)){
            addError( "Absent parameter: " + name );
            return false;
        }
    }
    return true;
}

QString SwiftApiClientLakeBTC::buildGetParamStr(const QJsonObject& j_params, const QStringList& params) const
{
    auto assetsLib = SwiftLib::getAssets();
    QString result = "?";
    for(auto& name : params){
        if(name == "symbol")
            result += name + "=" + assetsLib->getMarketName(j_params.value("market_id").toInt()) + "&";
        else
            result += name + "=" + j_params.value(name).toString() + "&";
    }
    return result.remove(result.size()-1,1);
}

QString SwiftApiClientLakeBTC::buildPostParamStr(const QJsonObject& j_params, const QStringList& params) const
{
    auto assetsLib = SwiftLib::getAssets();
    QString result;
    for(auto& name : params){
        if(name == "symbol")
            result += assetsLib->getMarketName(j_params.value("market_id").toInt()) + ",";
        else if(name == "price" || name == "amount" || name == "rate")
            result += QString::number(j_params.value(name).toDouble(), 'f', 8) + ",";
        else if(name == "orders_id"){
            // ??? Смотря как сохраняется список идентификаторов
            auto arr = j_params.value("orders_id").toArray();
            for(auto id = arr.cbegin(); id != arr.cend(); ++id)
                result += QString::number(id->toInt()) + ",";
        }
        else result += j_params.value(name).toString() + ",";
    }
    return result.remove(result.size()-1,1);
}

QByteArray SwiftApiClientLakeBTC::buildSignature(const QUrlQuery &body) const
{
    return (api_key.toUtf8() + ":" +
            QMessageAuthenticationCode::hash(body.toString().toUtf8(), api_secret.toUtf8(), QCryptographicHash::Sha1).toHex()
            ).toBase64();
}

QUrlQuery SwiftApiClientLakeBTC::buildBody(qint64 nonce, const QString &method, const QString& paramsString) const
{
    return QUrlQuery({
                       {"tonce", QString::number(nonce)},
                       {"accesskey", api_key},
                       {"requestmethod", "post"},
                       {"id", "1"},
                       {"method", method},
                       {"params", paramsString}
                   });
}

void SwiftApiClientLakeBTC::authPostRequest(const QUrl &url, const QString &method, const QString &paramsString, const quint64 &async_uuid)
{
    quint64 nonce = QDateTime::currentMSecsSinceEpoch() * 1000;
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", "Basic " + buildSignature(buildBody(nonce, method, paramsString)));
    request.setRawHeader("Json-Rpc-Tonce", QString::number(nonce).toUtf8());
    request.setRawHeader("Content-Type", "application/json");
    QJsonObject j_body({
                           {"id", "1"},
                           {"method", method},
                           {"params", paramsString}
                       });

    QNetworkReply* reply = SwiftApiClient::getManager()->post(request, QJsonDocument ( j_body ).toJson());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientLakeBTC::publicGetRequest(const QUrl& url, const quint64 &async_uuid)
{
    QNetworkReply* reply = SwiftApiClient::getManager()->get(QNetworkRequest(url));
    reply->setProperty("uuid", async_uuid);
}
