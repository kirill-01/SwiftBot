#include "swiftapiclientkucoin.h"



SwiftApiClientKuCoin::SwiftApiClientKuCoin(QObject *parent) : SwiftApiClient(parent ) {

    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
    QObject::connect( this, &SwiftApiClient::customRequestResult, this, &SwiftApiClientKuCoin::parseCustomApiResponse );
    timeDiff = 0;
    _inner_noncer = QDateTime::currentSecsSinceEpoch();
    syncTimestamps();
    QTimer * t = new QTimer(this);
    t->setInterval( 30000 );
    connect( t, &QTimer::timeout, this, &SwiftApiClientKuCoin::syncTimestamps );
    t->start();
    QTimer::singleShot( 360000, this, &SwiftApiClientKuCoin::transferToTrades );
}

quint64 SwiftApiClientKuCoin::getInnerNoncer()  {
    QMutexLocker lock( &innermutex );
    quint64 res = QDateTime::currentSecsSinceEpoch() - timeDiff;
    return res;
}

QString SwiftApiClientKuCoin::getExchangeName() const
{
    return "kucoin";
}

void SwiftApiClientKuCoin::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QNetworkRequest req(QUrl("https://openapi-v2.kucoin.com/api/v1/currencies") );
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", async_uuid );

}

void SwiftApiClientKuCoin::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientKuCoin::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QNetworkRequest req(QUrl("https://openapi-v2.kucoin.com/api/v1/market/orderbook/level2_20?symbol=BTC-USDT") );
    req.setRawHeader("Connection", "Keep-Alive");
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", async_uuid );
}

void SwiftApiClientKuCoin::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString,QString> params;
    params["clientOid"] = j_params.value("local_id").toString();
    params["price"] = QString::number( j_params.value("rate").toString().toDouble(), 'f', 8 );
    params["side"] = j_params.value("type").toString();
    params["symbol"] = SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toUInt() );
    params["size"] = QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 );


    QJsonObject j_p;
    QString body = "";
    for( auto it = params.begin(); it != params.end(); it++ ) {
        j_p.insert( it.key(), it.value() );
    }
    body = QJsonDocument( j_p ).toJson( QJsonDocument::Compact );
    QString prehash( QString::number( async_uuid )+"POST"+"/api/v1/orders"+body );
    QString signature( QMessageAuthenticationCode::hash( prehash.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl("https://openapi-v2.kucoin.com/api/v1/orders") );
    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature.toUtf8());
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( async_uuid ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    QNetworkReply * rep = getManager()->post( req, body.toUtf8() );
    rep->setProperty("uuid", async_uuid );
}

void SwiftApiClientKuCoin::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{

    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString endpoint( "/api/v1/orders/"+j_params.value("remote_id").toString() );
    const QString what( QString::number( ts ).toUtf8()+"DELETE"+endpoint );
    const QByteArray signature(  QMessageAuthenticationCode::hash( what.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl( "https://api.kucoin.com/api/v1/orders/"+j_params.value("remote_id").toString())  );

    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature );
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( ts ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    QNetworkReply * rep = getManager()->deleteResource( req );
    rep->setProperty("uuid", async_uuid );
     Q_UNUSED(j_params)
}

void SwiftApiClientKuCoin::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString endpoint( "/api/v1/orders/"+j_params.value("remote_id").toString() );
    const QString what( QString::number( ts ).toUtf8()+"GET"+endpoint );
    const QByteArray signature(  QMessageAuthenticationCode::hash( what.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl( "https://api.kucoin.com/api/v1/orders/"+j_params.value("remote_id").toString())  );

    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", api_key.toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature );
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( ts ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", api_user.toUtf8() );
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", async_uuid );
     Q_UNUSED(j_params)
}

void SwiftApiClientKuCoin::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientKuCoin::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientKuCoin::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED( j_params )
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString endpoint( "/api/v1/withdrawals");
    const QString what( QString::number( ts ).toUtf8()+"GET"+endpoint );
    const QByteArray signature(  QMessageAuthenticationCode::hash( what.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl( "https://api.kucoin.com/api/v1/withdrawals" )  );

    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", api_key.toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature );
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( ts ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", api_user.toUtf8() );
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", async_uuid );
}

void SwiftApiClientKuCoin::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    withdrawList( j_params, async_uuid );
}

void SwiftApiClientKuCoin::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientKuCoin::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    const quint32 currency_id = j_params.value("currency_id").toString().toUInt();
    const double amount = j_params.value("amount").toString().toDouble();

    if ( getInnerCachedBalance( currency_id ) < amount ) {
        if ( getTotalCachedBalance( currency_id ) >= amount ) {
            // Предварительно перемещаем средства с торгового на основной аккаунт
            QJsonObject params;
            params["currency_id"] = QString::number( currency_id );
            params["amount"] = QString::number( amount, 'f', 8 );
            params["from"] = "trade";
            params["to"] = "main";
            createAsyncWaiter( SwiftApiClient::AsyncMethods::WithdrawInner, params );
            QEventLoop loop;
            QTimer::singleShot( 15000, &loop, &QEventLoop::quit );
            loop.exec();
        }
    }

    QMap<QString,QString> params;
    const quint32 currency = j_params.value("currency_id").toString().toUInt();
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString cname = SwiftLib::getAssets()->getCurrencyName( currency );
    params["currency"] = cname;
    params["address"] = j_params.value("address").toString();

    if ( cname == "USDT" && !j_params.contains("chain") ) {
        params["chain"] = "ERC20";
    } else {
        params["chain"] = j_params.value("chain").toString();
    }
    params["amount"] = QString::number( (j_params.value("amount").toString().toDouble() - (SwiftLib::getAssets()->getCurrencyCoin( currency ) == 1 ? 0.0005 : 5 ) ) ,'f', 8 );

    QJsonObject j_p;
    QString body = "";
    for( auto it = params.begin(); it != params.end(); it++ ) {
        j_p.insert( it.key(), it.value() );
    }
    body = QJsonDocument( j_p ).toJson( QJsonDocument::Compact );
    QString prehash( QString::number( ts )+"POST"+"/api/v1/withdrawals"+body );
    QString signature( QMessageAuthenticationCode::hash( prehash.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl("https://api.kucoin.com/api/v1/withdrawals") );
    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature.toUtf8());
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( ts ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    QNetworkReply * rep = getManager()->post( req, body.toUtf8() );
    rep->setProperty("uuid", async_uuid );
}

void SwiftApiClientKuCoin::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{
    QMap<QString, QString> params;
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    params["clientOid"] = QString::number( async_uuid ) + getExchangeName();
    params["currency"] = SwiftLib::getAssets()->getCurrencyName( j_params.value("currency_id").toString().toUInt() );
    params["amount"] = QString::number( j_params.value("amount").toString().toDouble(), 'f', 8 );
    const QString fromType( j_params.value("from").toString() );
    const QString toType( j_params.value("to").toString() );
    params["from"] = fromType;
    params["to"] = toType;
    QJsonObject j_p;
    QString body = "";
    for( auto it = params.begin(); it != params.end(); it++ ) {
        j_p.insert( it.key(), it.value() );
    }
    body = QJsonDocument( j_p ).toJson( QJsonDocument::Compact );
    QString prehash( QString::number( ts )+"POST"+"/api/v2/accounts/inner-transfer"+body );
    QString signature( QMessageAuthenticationCode::hash( prehash.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl("https://api.kucoin.com/api/v2/accounts/inner-transfer") );
    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature.toUtf8());
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( ts ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    QNetworkReply * rep = getManager()->post( req, body.toUtf8() );
    rep->setProperty("uuid", async_uuid );
}

void SwiftApiClientKuCoin::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["async_uuid"] = QString::number( async_uuid );
    QJsonArray ja;
    QJsonObject jo;
    jo["amount"] = QString::number( 0, 'f', 8 );
    jo["taker"] = "0.002";
    ja.push_back( jo );
    ret["fees"] = ja;
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientKuCoin::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED( j_params )
    const quint64 noncer = QDateTime::currentMSecsSinceEpoch();
    QString prehash( QString::number( noncer )+"GET"+"/api/v1/accounts" );
    QString signature( QMessageAuthenticationCode::hash( prehash.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl("https://api.kucoin.com/api/v1/accounts") );
    req.setRawHeader("Content-Type", "application/json; charset=utf-8");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature.toUtf8());
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( noncer ).toUtf8()  );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    // KC-API-KEY, KC-API-SIGN, KC-API-TIMESTAMP, KC-API-PASSPHRASE
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", QString::number( async_uuid ) );
    rep->setProperty("nonce", noncer );
}

void SwiftApiClientKuCoin::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid )
{
    Q_UNUSED( j_params )
    QString body = "";
    const quint64 noncer = QDateTime::currentMSecsSinceEpoch();
    QString prehash( QString::number( noncer )+"GET"+"/api/v1/deposits"+body );
    QString signature( QMessageAuthenticationCode::hash( prehash.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl("https://api.kucoin.com/api/v1/deposits") );
    req.setRawHeader("Content-Type", "application/json; charset=utf-8");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature.toUtf8());
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( noncer ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", async_uuid );
}

void SwiftApiClientKuCoin::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientKuCoin::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString pname( SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toUInt() ) );
    const QString endpoint( "/api/v1/orders?status=done&symbol="+pname );
    const QString what( QString::number( ts ).toUtf8()+"GET"+endpoint );
    const QByteArray signature(  QMessageAuthenticationCode::hash( what.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl( "https://api.kucoin.com/api/v1/orders?status=done&symbol="+pname)  );

    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature );
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( ts ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", async_uuid );
     Q_UNUSED(j_params)
}

void SwiftApiClientKuCoin::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{

    quint64 ts = QDateTime::currentMSecsSinceEpoch();
    const QString pname( SwiftLib::getAssets()->getMarketName( j_params.value("market_id").toString().toUInt() ) );
    const QString endpoint( "/api/v1/orders?status=active&symbol="+pname );
    const QString what( QString::number( ts ).toUtf8()+"GET"+endpoint );
    const QByteArray signature(  QMessageAuthenticationCode::hash( what.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toBase64() );
    QNetworkRequest req( QUrl( "https://api.kucoin.com/api/v1/orders?status=active&symbol="+pname)  );

    req.setRawHeader("Content-Type", "application/json");
    req.setRawHeader("KC-API-KEY", getExchangeApiKey().toUtf8() );
    req.setRawHeader("KC-API-SIGN", signature );
    req.setRawHeader("KC-API-TIMESTAMP", QString::number( ts ).toUtf8() );
    req.setRawHeader("KC-API-PASSPHRASE", getExchangeApiAdditional().toUtf8() );
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("uuid", async_uuid );
     Q_UNUSED(j_params)
}

void SwiftApiClientKuCoin::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
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

void SwiftApiClientKuCoin::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = false;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["error"] = "Method not implemented";
    registerAsyncResult( async_uuid, ret );
}

void SwiftApiClientKuCoin::transferToTrades() {
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
            j_inner_withdraw_params["from"] = "main";
            j_inner_withdraw_params["to"] = "trade";
            qWarning() << "Inner transfers" << j_inner_withdraw_params;
            withdrawInner( j_inner_withdraw_params, 50000 );
        }
    }
    QTimer::singleShot( 360000, this, &SwiftApiClientKuCoin::transferToTrades );
}

void SwiftApiClientKuCoin::parseCustomApiResponse(const QString &method, const QByteArray &data) {
    if ( method == "timesync" ) {
        QJsonObject j_data( QJsonDocument::fromJson( data ).object() );
        if ( j_data.value("code").toString() == "200000" ) {
            const quint64 servtime = j_data.value("data").toVariant().toULongLong();
            const quint64 localtime = QDateTime::currentMSecsSinceEpoch();
            timeDiff = localtime - servtime;
        }
    }
}

void SwiftApiClientKuCoin::syncTimestamps() {
    QNetworkRequest req( QUrl( "https://api.kucoin.com/api/v1/timestamp" ) );
    QNetworkReply * rep = getManager()->get( req );
    rep->setProperty("customRequest", "timesync");
}
