#include "swiftapiclientbitfinex.h"
#include "orderparams.h"

SwiftApiClientBitfinex::SwiftApiClientBitfinex(QObject* parent) : SwiftApiClient(parent)
{
    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
}


QString SwiftApiClientBitfinex::getExchangeName() const
{
    // название в нижнем регистре без спецсимволов
    return "bitfinex";
}

QString SwiftApiClientBitfinex::getApiVersionString()
{
    // В строке версии можно указывать автора
    return "api-v2-art";
}

void SwiftApiClientBitfinex::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBitfinex::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBitfinex::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    /*
    Что бы не запутаться в разделяющих слешах:
    */
    QStringList urlparts({url_pub, "v2","book"});

    if (j_params.contains("market_id")) {
        urlparts.append( SwiftLib::getAssets()->getMarketName(j_params.value("market_id").toInt()) );
    } else {
        urlparts.append("tBTCUSD");
    }
    urlparts.append("R0");
    QUrl url( urlparts.join("/") );
    QUrlQuery urlquery;
    // Конкретно в этом методе, параметры статичны - меняться может только маркет
    urlquery.addQueryItem("len", "25" );
    url.setQuery( urlquery );
    QNetworkRequest request( url );
    QNetworkReply * reply = getManager()->get( request );
    reply->setProperty("uuid",async_uuid);
}

void SwiftApiClientBitfinex::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    // qWarning() << j_params;
    if ( isDebug() ) {
       addLog("Placing order " + QJsonDocument( j_params ).toJson( QJsonDocument::Compact ) );
    }

    OrderParams params( j_params );
    if ( !params.isValid() ) {
         addError("Incorrect order params:" + QJsonDocument( params.toJson() ).toJson( QJsonDocument::Compact ) );
         return;
    }
    QUrl url(QStringList({url_auth, "v2", "auth", "w", "order", "submit"}).join("/"));
    QJsonObject j_body;
    j_body["type"] = "EXCHANGE LIMIT";
    j_body["symbol"] = SwiftLib::getAssets()->getMarketName(j_params.value("market_id").toString().toUInt());
    j_body["price"] = j_params.value("rate").toString();
    j_body["amount"] = ( j_params.value("type").toString() == "sell" ? "-" : "" ) + j_params.value("amount").toString();
    QNetworkReply* reply = authPostRequest(url, url.path(), QString( QJsonDocument( j_body ).toJson( QJsonDocument::Compact ) ) );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QUrl url(QStringList({url_auth, "v2", "auth","w","order","cancel"}).join("/"));
    QJsonObject j_body;
    j_body["id"] =  j_params.value("remote_id").toString();
    const QString bodystr( QJsonDocument( j_body ).toJson( QJsonDocument::Compact ) );
    QNetworkReply* reply = authPostRequest(url, url.path(), bodystr  );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QUrl url(QStringList({url_auth, "v2", "auth","r","orders"}).join("/"));
    QJsonObject j_body;
    j_body["id"] =  QJsonArray({ j_params.value("remote_id")});
    QNetworkReply* reply = authPostRequest(url, url.path(), QString( QJsonDocument( j_body ).toJson( QJsonDocument::Compact ) ) );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );

    //QString api_path = "/v2/auth/w/order/update";
}

void SwiftApiClientBitfinex::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBitfinex::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QStringList strList({url_auth, "v2", "auth","r","movements"});
    if(j_params.contains("base_currency_id"))
        strList.append(SwiftLib::getAssets()->getCurrencyName(j_params.value("base_currency_id").toInt()));
    strList.append("hist");
    QUrl url(strList.join("/"));
    QUrlQuery body;
    if(j_params.contains("start"))
        body.addQueryItem("start", j_params.value("start").toString());
    if(j_params.contains("end"))
        body.addQueryItem("end", j_params.value("end").toString());
    if(j_params.contains("limit"))
        body.addQueryItem("limit", j_params.value("limit").toString());

    QNetworkReply* reply = authPostRequest(url, url.path(), body.toString());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    // Same as withdrawList
    QStringList strList({url_auth, "v2", "auth","r","movements"});
    strList.append("hist");
    QUrl url(strList.join("/"));
    QUrlQuery body;
    if(j_params.contains("start"))
        body.addQueryItem("start", j_params.value("start").toString());
    if(j_params.contains("end"))
        body.addQueryItem("end", j_params.value("end").toString());
    if(j_params.contains("limit"))
        body.addQueryItem("limit", j_params.value("limit").toString());

    QNetworkReply* reply = authPostRequest(url, url.path(), body.toString());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientBitfinex::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QUrl url(QStringList({url_auth, "v2", "auth","w","withdraw"}).join("/"));
    QJsonObject j_body;
    j_body["wallet"] = "exchange";
    if ( j_params.value("currency_id").toString().toUInt() == 39 ) {
        j_body["amount"] = QString::number( j_params.value("amount").toString().toDouble() - 0.001, 'f', 8 );
        j_body["method"] = "bitcoin";
    } else if ( j_params.value("currency_id").toString().toUInt() == 48 ) {
        j_body["amount"] = QString::number( j_params.value("amount").toString().toDouble() - 0.005, 'f', 8 );
        j_body["method"] = "ethereum";
    }

    j_body["address"] = j_params.value("address");
    QNetworkReply* reply = authPostRequest(url, url.path(), QString( QJsonDocument( j_body ).toJson( QJsonDocument::Compact ) ) );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{

    QUrl url(QStringList({url_auth, "v2", "auth","w","transfer"}).join("/"));
    QJsonObject j_body;
    if( j_params.contains("from") ) {
        j_body["from"] = j_params.value("from").toString();
        j_body["to"] =  j_params.value("to").toString();
    } else {
        j_body["from"] = "exchange";
        j_body["to"] =  "funding";
    }
    j_body["currency"] = SwiftLib::getAssets()->getCurrencyName(j_params.value("currency_id").toString().toUInt() );
    j_body["amount"] = j_params.value("amount");
    QNetworkReply* reply = authPostRequest(url, url.path(), QString( QJsonDocument( j_body ).toJson( QJsonDocument::Compact ) ) );
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
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

void SwiftApiClientBitfinex::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QUrl url(QStringList({url_auth, "v2", "auth","r","wallets"}).join("/"));

    QNetworkReply* reply = authPostRequest(url, url.path());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QStringList strList({url_auth, "v2", "auth","r","movements"});
    strList.append("hist");
    QUrl url(strList.join("/"));
    QUrlQuery body;
    if(j_params.contains("start"))
        body.addQueryItem("start", j_params.value("start").toString());
    if(j_params.contains("end"))
        body.addQueryItem("end", j_params.value("end").toString());
    if(j_params.contains("limit"))
        body.addQueryItem("limit", j_params.value("limit").toString());

    QNetworkReply* reply = authPostRequest(url, url.path(), body.toString());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QUrl url(QStringList({url_auth, "v2", "auth","w","deposit", "address"}).join("/"));
    QUrlQuery body;
    // Входящие параметры могут быть только currency_id
    // система никогда не должна запрашивать изменение депозитного адреса! это важно

    if(j_params.contains("wallet") &&
            j_params.contains("method") &&
            (!j_params.contains("op_renew") || j_params.value("op_renew").toInt() == 0))
    {
        //const QString currency_name ( SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() ));
        body.addQueryItem("wallet", j_params.value("wallet").toString() );
        // method параметр обязательный, должен быть указан
        body.addQueryItem("method", j_params.value("method").toString());
        body.addQueryItem("op_renew", "0");
    }
    else{
        addError( "Absent parameters: wallet, method, amount, address" );
        return;
    }

    QNetworkReply* reply = authPostRequest(url, url.path(), body.toString());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED( j_params )
    QString api_path = "/v2/auth/r/orders/";
    // По умолчанию, этот метод должен выполнить серию запросов по всем активным маркетам, если не задан конкретный
    QStringList strList({url_auth, "v2", "auth","r","orders", "hist"});

    QUrl url(strList.join("/"));
    QUrlQuery body;
    QNetworkReply* reply = authPostRequest(url, url.path(), body.toString());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED( j_params )
    auto api_path = QStringList({url_auth, "v2", "auth","r","orders"}).join("/");
    QUrl url(api_path);
    QUrlQuery body;
    QNetworkReply* reply = authPostRequest(url, url.path(), body.toString());
    reply->setProperty("uuid", async_uuid);
}

void SwiftApiClientBitfinex::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
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

void SwiftApiClientBitfinex::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}


void SwiftApiClientBitfinex::auth(QNetworkRequest& req, const QString& api_path, const QString& body_str) const
{
    auto nonce = QString::number( QDateTime::currentMSecsSinceEpoch() * 1000 );
    auto signature = QString("/api") + api_path + nonce + body_str;
    const QString sig = QMessageAuthenticationCode::hash( signature.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha384 ).toHex();
    req.setRawHeader("Content-type", "application/json");
    req.setRawHeader("bfx-nonce", nonce.toUtf8() );
    req.setRawHeader("bfx-apikey", api_key.toUtf8() );
    req.setRawHeader("bfx-signature", sig.toUtf8() );
}

QNetworkReply* SwiftApiClientBitfinex::authPostRequest(const QUrl& url, const QString& api_path, const QString& body_str)
{
    QNetworkRequest request( url );
    auth(request, api_path, body_str);
    return getManager()->post(request, body_str.toUtf8());
}
