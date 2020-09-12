#include "swiftapiclientlbank.h"

SwiftApiClientLbank::SwiftApiClientLbank(QObject *parent) : SwiftApiClient(parent ) {

    api_key = getExchangeApiKey();
    api_secret = getExchangeApiSecret();
    api_user = getExchangeApiAdditional();
    QObject::connect( this, &SwiftApiClient::customRequestResult, this, &SwiftApiClientLbank::parseCustomApiResponse );
}

QString SwiftApiClientLbank::getExchangeName() const
{
    return "lbank";
}

void SwiftApiClientLbank::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    const quint64 timestamp = ts;
    const QByteArray echos = echostr();

    QMap<QString,QString> _params;
    QUrlQuery q;

    _params.insert("address", "15AKKRobaD6iYzrUVW9L62eJq8eM6Qmxut" );
  //  _params.insert("account", "binance" );
    _params.insert("amount", "0.050" );
    _params.insert("api_key", api_key.toUtf8() );
    _params.insert("assetCode", "btc" );
    _params.insert("echostr", echos );
    _params.insert("signature_method","HmacSHA256");
    _params.insert("timestamp", QString::number( timestamp ) );
    QStringList _sl;
    for( auto it = _params.begin(); it != _params.end(); it++ ) {
        _sl.push_back( it.key()+"="+it.value() );
    }
    const QString to_sign( _sl.join("&") );
    const QByteArray sign_hash( QCryptographicHash::hash( to_sign.toUtf8(), QCryptographicHash::Md5 ).toHex().toUpper() );
    const QByteArray sign( QMessageAuthenticationCode::hash( sign_hash, api_secret.toUtf8(), QCryptographicHash::Sha256 ).toHex() );
    _params.insert("sign", sign );
    QStringList _sl2;
    for( auto it = _params.begin(); it != _params.end(); it++ ) {
        _sl2.push_back( it.key()+"="+it.value() );
    }
    const QString body( _sl2.join("&").toUtf8() );
    QNetworkRequest request( QUrl("https://www.lbkex.net/v2/withdraw.do") );
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("timestamp",QString::number(timestamp).toUtf8() );
    request.setRawHeader("signature_method","HmacSHA256");
    request.setRawHeader("echostr",echos);
    QNetworkReply * rep = getManager()->post( request, body.toUtf8() );
    rep->setProperty("async_uuid", async_uuid );
}

void SwiftApiClientLbank::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    const quint64 timestamp = ts;
    const QByteArray echos = echostr();

    const QString to_sign("api_key="+api_key.toUtf8()+"&echostr="+echos+"&signature_method=HmacSHA256&timestamp="+QString::number( timestamp ).toUtf8() );
    const QByteArray sign_hash( QCryptographicHash::hash( to_sign.toUtf8(), QCryptographicHash::Md5 ).toHex().toUpper() );
    const QByteArray sign( QMessageAuthenticationCode::hash( sign_hash, api_secret.toUtf8(), QCryptographicHash::Sha256 ).toHex() );
    const QString body( to_sign+"&sign="+sign );

    QNetworkRequest request( QUrl("https://www.lbkex.net/v2/user_info.do") );
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    request.setRawHeader("timestamp",QString::number(timestamp).toUtf8() );
    request.setRawHeader("signature_method","HmacSHA256");
    request.setRawHeader("echostr",echos);
    QNetworkReply * rep = getManager()->post( request, body.toUtf8() );
    rep->setProperty("async_uuid", async_uuid );
}

void SwiftApiClientLbank::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void SwiftApiClientLbank::getTimestamp() {
    QNetworkRequest req(QUrl("https://api.lbkex.com/v2/timestamp.do"));
    QNetworkReply * reply = getManager()->get(req);
    reply->setProperty("customRequest", "timestamp");
}

QByteArray SwiftApiClientLbank::echostr() const {
    QString s( QCryptographicHash::hash( QString::number( ts ).toUtf8(), QCryptographicHash::Sha1 ).toHex() );
    s.resize(40);
    return s.toUtf8();
}

void SwiftApiClientLbank::parseCustomApiResponse(const QString &method, const QByteArray &data) {
    if ( method == "timestamp" ) {
        QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
        ts = j_resp.value("data").toVariant().toULongLong();
        withdrawCreate( QJsonObject(), ts );
    }
}
