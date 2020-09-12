#include "swiftapiclienthitbtc.h"

SwiftApiClientHitbtc::SwiftApiClientHitbtc(QObject *parent) : SwiftApiClient( parent )
{
    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
    QTimer::singleShot( 360000, this, &SwiftApiClientHitbtc::transferToTrades );

}
QNetworkReply *SwiftApiClientHitbtc::sendRequest(const QString &url, const QString &method, const QMap<QString, QString> params) {
    QUrlQuery q;
    QJsonObject j_q;
    for( auto it = params.begin(); it != params.end(); it++ ) {
        j_q[ it.key() ] = it.value();
    }
    const quint64 nnc( QDateTime::currentSecsSinceEpoch() );
    q.addQueryItem("nonce", QString::number( nnc ) );
    QUrl uu( "https://api.hitbtc.com/api/2/"+url );
    QNetworkRequest request( uu );
    QString concatenated = getExchangeApiKey() + ":" + getExchangeApiSecret();
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    request.setRawHeader("Connection", "Keep-Alive");
    request.setRawHeader("Content-type", "application/json");
    if ( method == "POST" ) {
        return getManager()->post( request, QJsonDocument( j_q ).toJson( QJsonDocument::Compact ) );
    } else if ( method == "GET" )  {
        uu.setQuery( q );
        request.setUrl( uu );
        return getManager()->get( request );
    } else if ( method == "PUT" )  {
        return getManager()->put( request, q.toString().toUtf8() );
    } else if ( method == "DELETE" )  {
        request.setUrl( uu );
        return getManager()->deleteResource( request );
    } else {
        uu.setQuery( q );
        request.setUrl( uu );
        return getManager()->get( request );
    }
}

QString SwiftApiClientHitbtc::getExchangeName() const
{
    return "hitbtc";
}

QString SwiftApiClientHitbtc::getApiVersionString()
{
    return "beta";
}

void SwiftApiClientHitbtc::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientHitbtc::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientHitbtc::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientHitbtc::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{

    QMap<QString,QString> _params;
    _params["clientOrderId"] = j_params.value("local_id").toString();
    _params["symbol"] = SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toUInt() );
    _params["side"] = j_params.value("type").toString();
    _params["quantity"] = j_params.value("amount").toString();
    _params["price"] = j_params.value("rate").toString();
    QNetworkReply * reply = sendRequest("order", "POST", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendRequest("order/"+j_params.value("local_id").toString(), "DELETE", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendRequest("order/"+j_params.value("local_id").toString(), "GET", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    orderCancel( j_params, 6000 );
    orderPlace( j_params, async_uuid );
}

void SwiftApiClientHitbtc::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientHitbtc::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    withdrawHistory( j_params, async_uuid );
}

void SwiftApiClientHitbtc::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    // account/transactions
    QMap<QString,QString> _params;
    const QString currency_name = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    QNetworkReply * reply = sendRequest("account/transactions", "GET", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientHitbtc::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientHitbtc::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    // account/crypto/withdraw
    // get if dosnt have on bank acc -> transfer
    const double reserved = getSession()->call("swiftbot.balances.get.reserved", {j_params.value("currency_id").toString().toUInt() }).toDouble();
    if ( reserved < j_params.value("amount").toString().toDouble() ) {
        QJsonObject j_inner_withdraw_params;
        j_inner_withdraw_params["currency_id"] = j_params.value("currency_id");
        j_inner_withdraw_params["amount"] = QString::number( j_params.value("amount").toString().toDouble() - reserved, 'f', 8 );
        j_inner_withdraw_params["type"] = "exchangeToBank";
        withdrawInner( j_inner_withdraw_params, 50000 );
    }


    QMap<QString,QString> _params;
    const QString currency_name = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    _params["currency"] = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    _params["amount"] = j_params.value("amount").toString();
    _params["address"] = j_params.value("address").toString();
    _params["includeFee"] = "true";
    QNetworkReply * reply = sendRequest("account/crypto/withdraw", "POST", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientHitbtc::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{
    // account/transfer
    QMap<QString,QString> _params;
    const QString currency_name = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    _params["currency"] = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    _params["amount"] = j_params.value("amount").toString();
    if( j_params.contains("type") ) {
        _params["type"] = j_params.value("type").toString();
    } else {
        _params["type"] = "bankToExchange";
    }
    QNetworkReply * reply = sendRequest("account/transfer", "POST", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientHitbtc::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["currency"] = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    _params["amount"] = QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 );
    QNetworkReply * reply = sendRequest("/api/2/account/crypto/estimate-withdraw", "GET", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientHitbtc::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    createAsyncWaiter( SwiftApiClient::AsyncMethods::TimeSyncMethod, QJsonObject() );
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendRequest("trading/balance", "GET", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid)
{
   // account/transactions
    QMap<QString,QString> _params;
    const QString currency_name = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    QNetworkReply * reply = sendRequest("account/transactions", "GET", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientHitbtc::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    const QString currency_name = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    QNetworkReply * reply = sendRequest("account/crypto/address/"+currency_name, "GET", _params );
    reply->setProperty("uuid", async_uuid );

}

void SwiftApiClientHitbtc::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendRequest("history/order", "GET", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendRequest("order", "GET", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid){
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendRequest("trading/fee/BTCUSDT", "GET", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientHitbtc::customMethod(const QJsonObject &j_params, const quint64 &async_uuid) {
    static quint32 counter = 0;
    if ( counter == 0 || counter > 5 ) {
        counter = 0;
        QMap<QString,QString> _params;
        QNetworkReply * reply = sendRequest("account/balance", "GET", _params );
        reply->setProperty("uuid", async_uuid );
        counter++;
    } else {
        counter++;
    }
    Q_UNUSED(j_params)
}

void SwiftApiClientHitbtc::transferToTrades() {
    if ( getSession() == nullptr || !getSession()->isJoined() ) {
        return;
    }
    QList<quint32> _currencies( SwiftLib::getAssets()->getExchangeCurrencies( getExchangeId() ) );
    for( auto it = _currencies.begin(); it != _currencies.end(); it++ ) {
        const double reserved = getSession()->call("swiftbot.balances.get.reserved", {*it}).toDouble();
        if ( reserved > 0 ) {
            QJsonObject j_inner_withdraw_params;
            j_inner_withdraw_params["currency_id"] = QString::number( *it );
            j_inner_withdraw_params["amount"] = QString::number( reserved, 'f', 8 );
            j_inner_withdraw_params["type"] = "bankToExchange";
            qWarning() << "Inner transfers" << j_inner_withdraw_params;
            withdrawInner( j_inner_withdraw_params, 50000 );
        }
    }
    QTimer::singleShot( 360000, this, &SwiftApiClientHitbtc::transferToTrades );
}

