#include "swiftapiclientbittrex.h"

QString SwiftApiClientBittrex::getExchangeName() const
{
    return "bittrex";
}

QString SwiftApiClientBittrex::getApiVersionString()
{
    return "2.0.beta";
}

void SwiftApiClientBittrex::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QUrl url("https://api.bittrex.com/v3/currencies");
    QNetworkRequest request( url );
    QNetworkReply * reply = getManager()->get( request );
    reply->setProperty("uuid",async_uuid);
}

void SwiftApiClientBittrex::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QUrl url("https://bittrex.com/api/v1.1/public/getmarkets");
    QNetworkRequest request( url );
    QNetworkReply * reply = getManager()->get( request );
    reply->setProperty("uuid",async_uuid);
}

void SwiftApiClientBittrex::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QUrl url("https://bittrex.com/api/v1.1/public/getorderbook");
    QUrlQuery urlquery;
    if ( j_params.contains("market") ) {
        urlquery.addQueryItem("market", j_params.value("market").toString() );
    } else if ( j_params.contains("market_id") ) {
        urlquery.addQueryItem("market", "USD-BTC" );
    } else {
        urlquery.addQueryItem("market", "USD-BTC" );
    }
    urlquery.addQueryItem("type", "both" );
    url.setQuery( urlquery );
    QNetworkRequest request( url );
    QNetworkReply * reply = getManager()->get( request );
    reply->setProperty("uuid",async_uuid);
}

void SwiftApiClientBittrex::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{

    const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    if ( isDebug() ) {
        qWarning() << getExchangeName() << "PLACING ORDER" << j_params << async_uuid;
    }
    if ( j_params.value("type").toString() == "sell" ) {
        QUrl url("https://api.bittrex.com/api/v1.1/market/selllimit");
        QUrlQuery urlquery;
        urlquery.addQueryItem("apikey", getExchangeApiKey() );
        urlquery.addQueryItem("market", SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toInt() ) );
        urlquery.addQueryItem("quantity", QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 ) );
        urlquery.addQueryItem("rate", QString::number( j_params.value("rate").toString().toDouble(), 'f', 8 ) );
        urlquery.addQueryItem("nonce", QString::number( nnc ) );
        url.setQuery( urlquery );
        const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
        QNetworkRequest request( url );
        request.setRawHeader("apisign", msg_hash.toUtf8() );
        QNetworkReply * reply = getManager()->get(request);
        reply->setProperty("uuid", async_uuid);
    } else if ( j_params.value("type").toString() == "buy" ) {
        QUrl url("https://api.bittrex.com/api/v1.1/market/buylimit");
        QUrlQuery urlquery;
        urlquery.addQueryItem("apikey", getExchangeApiKey() );
        urlquery.addQueryItem("market", SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toInt() ) );
        urlquery.addQueryItem("quantity", QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 ) );
        urlquery.addQueryItem("rate", QString::number( j_params.value("rate").toString().toDouble(), 'f', 8 ) );
        urlquery.addQueryItem("nonce", QString::number( nnc ) );
        url.setQuery( urlquery );
        const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
        QNetworkRequest request( url );
        request.setRawHeader("apisign", msg_hash.toUtf8() );
        QNetworkReply * reply = getManager()->get(request);
        reply->setProperty("uuid", async_uuid);
    } else {
        qWarning() << "WRONG ORDER TYPE";
    }
}

void SwiftApiClientBittrex::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/market/cancel");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
    urlquery.addQueryItem("uuid", j_params.value("remote_id").toString() );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBittrex::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/account/getorder");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
    urlquery.addQueryItem("uuid", j_params.value("remote_id").toString() );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBittrex::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
     Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBittrex::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
     Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBittrex::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/account/getwithdrawalhistory");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", QString::number( async_uuid ) );
}

void SwiftApiClientBittrex::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
     const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/account/getwithdrawalhistory");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", QString::number( async_uuid ) );
}

void SwiftApiClientBittrex::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBittrex::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    // SEND
     const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/account/withdraw");
    QUrlQuery urlquery;

    QJsonObject j_cached;
    j_cached["exchange_id"] =QString::number( getExchangeId() );
    j_cached["currency_id"] = j_params.value("currency_id");
    j_cached["created_at"] = QString::number( QDateTime::currentMSecsSinceEpoch() );
    j_cached["status"] = "pending";
    j_cached["type"] = "external";
    j_cached["local_id"] = "bittrex_withdraw_"+QString::number( QDateTime::currentMSecsSinceEpoch() );
    j_cached["amount"] = QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 );
    j_cached["fee"] = QString::number( 0.005,'f', 8 );
    j_cached["deposit_address"] = j_params.value("address").toString();

    urlquery.addQueryItem("apikey", getExchangeApiKey() );
    urlquery.addQueryItem("currency", SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() ) );
    urlquery.addQueryItem("quantity", QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 ) );
    urlquery.addQueryItem("address",  j_params.value("address").toString() );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QJsonObject jres;
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", QString::number( async_uuid ) );

}

void SwiftApiClientBittrex::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBittrex::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["async_uuid"] = QString::number( async_uuid );
    QJsonArray ja;
    QJsonObject jo;
    jo["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName("BTC", getExchangeId() ) );
    jo["fee"] = "0.0005";
    QJsonObject jo2;
    jo2["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName("USDT", getExchangeId() ) );
    jo2["fee"] = "5";
    ja.push_back( jo );
    ja.push_back( jo2 );
    ret["fees"] = ja;
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBittrex::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED( j_params )
    const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/account/getbalances");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientBittrex::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
     const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/account/getdeposithistory");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", QString::number( async_uuid ) );
}

void SwiftApiClientBittrex::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QJsonObject ret;

    const quint32 currency_id = j_params.value("currency_id").toString().toUInt();
    ret["success"] = true;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["exchange_id"] = QString::number( getExchangeId() );
    ret["currency_id"] = QString::number( currency_id );
    ret["deposit_address"] = "";
    for( auto it = _cached_currencies.begin(); it != _cached_currencies.end(); it++ ) {
        if ( j_params.value("currency_id").toString().toUInt() == it->toObject().value("currency_id").toString().toUInt() ) {
            ret["deposit_address"] = it->toObject().value("deposit_address").toString();
        }
    }
    if ( ret.value("deposit_address").toString() == "" ) {
        const quint64 nnc = QDateTime::currentSecsSinceEpoch();
        QUrl url("https://api.bittrex.com/api/v1.1/account/getdepositaddress");
        QUrlQuery urlquery;
        urlquery.addQueryItem("apikey", getExchangeApiKey() );
        urlquery.addQueryItem("nonce", QString::number( nnc ) );
        urlquery.addQueryItem("currency", SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() ) );
        url.setQuery( urlquery );
        const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
        QNetworkRequest request( url );
        request.setRawHeader("apisign", msg_hash.toUtf8() );
        QNetworkReply * reply = getManager()->get(request);
        reply->setProperty("uuid", QString::number( async_uuid ) );
    }
    registerAsyncResult( async_uuid, ret );

}

void SwiftApiClientBittrex::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
     const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/account/getorderhistory");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
   // urlquery.addQueryItem("market", "USD-BTC" );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", QString::number( async_uuid ) );
}

void SwiftApiClientBittrex::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
     const quint64 nnc = QDateTime::currentSecsSinceEpoch();
    QUrl url("https://api.bittrex.com/api/v1.1/market/getopenorders");
    QUrlQuery urlquery;
    urlquery.addQueryItem("apikey", getExchangeApiKey() );
  //  urlquery.addQueryItem("market", "USD-BTC" );
    urlquery.addQueryItem("nonce", QString::number( nnc ) );
    url.setQuery( urlquery );
    const QString msg_hash = QMessageAuthenticationCode::hash( url.toString(QUrl::FullyEncoded).toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex();
    QNetworkRequest request( url );
    request.setRawHeader("apisign", msg_hash.toUtf8() );
    QNetworkReply * reply = getManager()->get(request);
    reply->setProperty("uuid", QString::number( async_uuid ) );
}

void SwiftApiClientBittrex::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["async_uuid"] = QString::number( async_uuid );

    QJsonObject jo;
    jo["amount"] = QString::number( 0, 'f', 8 );
    jo["taker"] = QString::number( SwiftLib::getExchangeFee( getExchangeId() ), 'f', 8 );
    ret["fees"] = jo;
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBittrex::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

bool SwiftApiClientBittrex::beginUsdWithdraw(const double &amount) {
    freeze_auto_sell_usdt = true;
    QJsonObject j_params;
    j_params["type"] = "buy";
    j_params["market_id"] = "19";
    j_params["rate"] = "1.0025";
    j_params["amount"] = QString::number( amount ,'f', 8 );
    const quint64 order_uuid = createAsyncWaiter( SwiftApiClient::AsyncMethods::OrderPlace, j_params );
    bool started = false;
    QEventLoop loop;
    if ( !started ) {
        started = true;
        connect( this, &SwiftApiClient::onApiResponseParsed, [&](const quint64 &uuid, const QJsonObject &j_result){
            if ( order_uuid == uuid ) {
                loop.exit();
            }
            Q_UNUSED(j_result)
        } );
    }
    loop.exec();
    return true;
}
