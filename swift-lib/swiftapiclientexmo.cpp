#include "swiftapiclientexmo.h"

SwiftApiClientExmo::SwiftApiClientExmo(QObject *parent) : SwiftApiClient(parent ) {
    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
    noncer = QDateTime::currentSecsSinceEpoch();
}

QNetworkReply *SwiftApiClientExmo::sendGet(const QString &url, const QMap<QString, QString> params) {
    QUrlQuery q;
    for( auto it = params.begin(); it != params.end(); it++ ) {
        q.addQueryItem( it.key(), it.value() );
    }
    const quint64 nnc( getNoncer() );
    q.addQueryItem("nonce", QString::number( nnc ) );
    const QByteArray to_sign( q.toString().toUtf8() );
    const QByteArray sign( QMessageAuthenticationCode::hash( to_sign, getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex() );
    QUrl uu( "https://api.exmo.com/v1/"+url );
    uu.setQuery( q );
    QNetworkRequest request( uu );
    request.setRawHeader("Content-type", "application/x-www-form-urlencoded");
    request.setRawHeader("Key", getExchangeApiKey().toUtf8() );
    request.setRawHeader("Sign", sign);
    return  getManager()->get( request );
}

QNetworkReply *SwiftApiClientExmo::sendPost(const QString &url, const QMap<QString, QString> params) {
    QUrlQuery q;
    for( auto it = params.begin(); it != params.end(); it++ ) {
        q.addQueryItem( it.key(), it.value() );
    }
    const quint64 nnc( getNoncer() );
    q.addQueryItem("nonce", QString::number( nnc ) );
    const QByteArray to_sign( q.toString().toUtf8() );
    const QByteArray sign( QMessageAuthenticationCode::hash( to_sign, getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha512 ).toHex() );
    QUrl uu( "https://api.exmo.com/v1/"+url );
    QNetworkRequest request( uu );
    request.setRawHeader("Content-type", "application/x-www-form-urlencoded");
    request.setRawHeader("Key", getExchangeApiKey().toUtf8() );
    request.setRawHeader("Sign", sign);
    return  getManager()->post( request, q.toString().toUtf8() );
}

QString SwiftApiClientExmo::getExchangeName() const
{
    return "exmo";
}

QString SwiftApiClientExmo::getApiVersionString()
{
    return "beta";
}

void SwiftApiClientExmo::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientExmo::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientExmo::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientExmo::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["pair"] = SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toUInt() );
    _params["quantity"] =  j_params.value("amount").toString();
    _params["price"] = QString::number( j_params.value("rate").toString().toDouble(), 'f', SwiftLib::getAssets()->getMarketPricePrecision(  j_params.value("market_id").toString().toUInt() ));
    _params["type"] = j_params.value("type").toString();
    QNetworkReply * reply = sendPost("order_create", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["order_id"] =  j_params.value("remote_id").toString();
    QNetworkReply * reply = sendPost("order_cancel", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["order_id"] =  j_params.value("remote_id").toString();
    QNetworkReply * reply = sendPost("order_trades", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    orderCancel( j_params, 0 );
    orderPlace( j_params, async_uuid );
}

void SwiftApiClientExmo::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientExmo::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    withdrawHistory( j_params, async_uuid );
}

void SwiftApiClientExmo::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["date"] = QString::number( QDateTime::currentSecsSinceEpoch()  );
    QNetworkReply * reply = sendPost("wallet_history", _params );
    reply->setProperty("uuid", async_uuid );
    Q_UNUSED( j_params );
}

void SwiftApiClientExmo::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientExmo::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["amount"] = j_params.value("amount").toString();
    const quint32 curid = j_params.value("currency_id").toString().toUInt();
    const QString curname = SwiftLib::getAssets()->getCurrencyName( curid );
    if ( curname == "???" ) {
        qWarning() << "Error currency";
        return;
    }
    _params["currency"] = curname;
    _params["address"] = j_params.value("address").toString();
    if ( curname == "USDT" ) {
        _params["transport"] = "ERC20";
    }
    if ( j_params.contains("transport") ) {
        _params["transport"] = j_params.value("transport").toString();
    }
    QNetworkReply * reply = sendPost("withdraw_crypt", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientExmo::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["async_uuid"] = QString::number( async_uuid );
    QJsonArray ja;
    QJsonObject jo;
    jo["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName("BTC", getExchangeId() ) );
    jo["fee"] = "0.0003";
    QJsonObject jo2;
    jo2["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName("USDT", getExchangeId() ) );
    jo2["fee"] = "0.5";
    ja.push_back( jo );
    ja.push_back( jo2 );
    ret["fees"] = ja;
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientExmo::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendPost("user_info", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["date"] = QString::number( QDateTime::currentSecsSinceEpoch() );

    Q_UNUSED( j_params );
    QNetworkReply * reply = sendPost("wallet_history", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendPost("deposit_address", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> _params;
    _params["pair"] = "BTC_USD";
    if ( j_params.contains("limit") ) {
        _params["limit"] = j_params.value("limit").toString();
    }
    QNetworkReply * reply = sendPost("user_trades", _params );
    reply->setProperty("uuid", async_uuid );

}

void SwiftApiClientExmo::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QMap<QString,QString> _params;
    QNetworkReply * reply = sendPost("user_open_orders", _params );
    reply->setProperty("uuid", async_uuid );
}

void SwiftApiClientExmo::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
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

void SwiftApiClientExmo::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
     Q_UNUSED(j_params)
     Q_UNUSED(async_uuid)
}
