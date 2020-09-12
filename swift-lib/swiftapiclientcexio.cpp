#include "swiftapiclientcexio.h"


QString SwiftApiClientCexio::getExchangeName() const
{
    return "cexio";
}

void SwiftApiClientCexio::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QUrl url("https://cex.io/api/currency_limits");
    QNetworkRequest request( url );
    QNetworkReply * reply = getManager()->get( request );
    reply->setProperty("uuid",async_uuid);
}

void SwiftApiClientCexio::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QUrl url("https://cex.io/api/currency_limits");
    QNetworkRequest request( url );
    QNetworkReply * reply = getManager()->get( request );
    reply->setProperty("uuid",async_uuid);
}

void SwiftApiClientCexio::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QUrl url("https://cex.io/api/order_book/BTC/USD?depth=20");
    QNetworkRequest request( url );
    QNetworkReply * reply = getManager()->get( request );
    reply->setProperty("uuid",async_uuid);
}

void SwiftApiClientCexio::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    const QString pair( SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toUInt() ) );
    const QString f_symbol = pair.split(":").at(0);
    const QString s_symbol = pair.split(":").at(1);
    QUrl url("https://cex.io/api/place_order/"+f_symbol+"/"+s_symbol);
    QUrlQuery query;
    const QByteArray cod2( QMessageAuthenticationCode::hash( QString::number( async_uuid ).toUtf8()+getExchangeApiAdditional().toUtf8()+getExchangeApiKey().toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex().toUpper());
    query.addQueryItem("key", getExchangeApiKey().toUtf8() );
    query.addQueryItem("signature", cod2 );
    query.addQueryItem("nonce", QString::number( async_uuid ).toUtf8() );
    query.addQueryItem("type",  j_params.value("type").toString());
    query.addQueryItem("amount", QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 )  );
    query.addQueryItem("price", QString::number( j_params.value("rate").toString().toDouble(), 'f', 8 )  );
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply * reply = getManager()->post( request, query.toString().toUtf8() );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientCexio::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{

    QUrl url("https://cex.io/api/cancel_order/");
    QUrlQuery query;
    const QByteArray cod2( QMessageAuthenticationCode::hash( QString::number( async_uuid ).toUtf8()+getExchangeApiAdditional().toUtf8()+getExchangeApiKey().toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex().toUpper());
    query.addQueryItem("key", getExchangeApiKey().toUtf8() );
    query.addQueryItem("signature", cod2 );
    query.addQueryItem("nonce", QString::number( async_uuid ).toUtf8() );
    query.addQueryItem("id",  j_params.value("remote_id").toString());
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply * reply = getManager()->post( request, query.toString( QUrl::FullyDecoded).toUtf8() );
    reply->setProperty("uuid", async_uuid);

}

void SwiftApiClientCexio::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QUrl url("https://cex.io/api/get_order/");
    QUrlQuery query;
    const QByteArray cod2( QMessageAuthenticationCode::hash( QString::number( async_uuid ).toUtf8()+getExchangeApiAdditional().toUtf8()+getExchangeApiKey().toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex().toUpper());
    query.addQueryItem("key", getExchangeApiKey().toUtf8() );
    query.addQueryItem("signature", cod2 );
    query.addQueryItem("nonce", QString::number( async_uuid ).toUtf8() );
    query.addQueryItem("id",  j_params.value("remote_id").toString());
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply * reply = getManager()->post( request, query.toString().toUtf8() );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientCexio::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["withdraws"] = QJsonArray();
    ret["async_uuid"] = QString::number( async_uuid );
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["withdraws"] = QJsonArray();
    ret["async_uuid"] = QString::number( async_uuid );
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED( j_params )
    QUrl url("https://cex.io/api/balance/");
    QUrlQuery query;

    const QByteArray cod2( QMessageAuthenticationCode::hash( QString::number( async_uuid ).toUtf8()+getExchangeApiAdditional().toUtf8()+getExchangeApiKey().toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex().toUpper());

    query.addQueryItem("key", getExchangeApiKey().toUtf8() );
    query.addQueryItem("signature", cod2 );
    query.addQueryItem("nonce", QString::number( async_uuid ).toUtf8() );
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply * reply = getManager()->post( request, query.toString().toUtf8() );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientCexio::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["deposits"] = QJsonArray();
    ret["async_uuid"] = QString::number( async_uuid );
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientCexio::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{

    Q_UNUSED( j_params )
    QUrl url("https://cex.io/api/get_address/");
    QUrlQuery query;
    const QString currency( SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() ) );
    const QByteArray cod2( QMessageAuthenticationCode::hash( QString::number( async_uuid ).toUtf8()+getExchangeApiAdditional().toUtf8()+getExchangeApiKey().toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex().toUpper());
    query.addQueryItem("key", getExchangeApiKey().toUtf8() );
    query.addQueryItem("signature", cod2 );
    query.addQueryItem("currency", currency );
    query.addQueryItem("nonce", QString::number( async_uuid ).toUtf8() );
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply * reply = getManager()->post( request, query.toString().toUtf8() );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientCexio::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    const QString pair( j_params.contains("market_id") ? SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toUInt() ) : "BTC:USD" );
    const QString f_symbol = pair.split(":").at(0);
    const QString s_symbol = pair.split(":").at(1);
    QUrl url("https://cex.io/api/archived_orders/"+f_symbol+"/"+s_symbol);
    QUrlQuery query;
    const QByteArray cod2( QMessageAuthenticationCode::hash( QString::number( async_uuid ).toUtf8()+getExchangeApiAdditional().toUtf8()+getExchangeApiKey().toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex().toUpper());
    query.addQueryItem("key", getExchangeApiKey().toUtf8() );
    query.addQueryItem("signature", cod2 );
    query.addQueryItem("nonce", QString::number( async_uuid ).toUtf8() );
    query.addQueryItem("dateFrom", QString::number( QDateTime::currentSecsSinceEpoch() - (60*60*24*30) ));
    query.addQueryItem("dateTo",  QString::number( QDateTime::currentSecsSinceEpoch() ) );
    query.addQueryItem("lastTxDateFrom",  QString::number( QDateTime::currentSecsSinceEpoch() -(60*60*24*30) ) );
    query.addQueryItem("lastTxDateTo",  QString::number( QDateTime::currentSecsSinceEpoch() ));

    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply * reply = getManager()->post( request, query.toString().toUtf8() );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientCexio::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED( j_params )
    QUrl url("https://cex.io/api/open_orders/");
    QUrlQuery query;
    const QByteArray cod2( QMessageAuthenticationCode::hash( QString::number( async_uuid ).toUtf8()+getExchangeApiAdditional().toUtf8()+getExchangeApiKey().toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex().toUpper());
    query.addQueryItem("key", getExchangeApiKey().toUtf8() );
    query.addQueryItem("signature", cod2 );
    query.addQueryItem("nonce", QString::number( async_uuid ).toUtf8() );
    QNetworkRequest request( url );
    request.setHeader( QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply * reply = getManager()->post( request, query.toString().toUtf8() );
    reply->setProperty("uuid",async_uuid);

}

void SwiftApiClientCexio::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
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

void SwiftApiClientCexio::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}
