#ifndef SWIFTAPICLIENTBINANCE_H
#define SWIFTAPICLIENTBINANCE_H

#include <QObject>
#include "swiftapiclient.h"

/**
 * @brief The SwiftApiClientBinance class
 * SIGNED endpoints require an additional parameter, signature, to be sent in the query string or request body.
Endpoints use HMAC SHA256 signatures. The HMAC SHA256 signature is a keyed HMAC SHA256 operation. Use your secretKey as the key and totalParams as the value for the HMAC operation.
The signature is not case sensitive.
totalParams is defined as the query string concatenated with the request body.
 */

class SwiftApiClientBinance : public SwiftApiClient
{
    Q_OBJECT
public:
    explicit SwiftApiClientBinance(QObject *parent) : SwiftApiClient( parent )
    {
        api_key = getExchangeApiKey();
        api_secret = getExchangeApiSecret();
        api_user = getExchangeApiAdditional();
        QObject::connect( this, &SwiftApiClient::customRequestResult, this, &SwiftApiClientBinance::parseCustomApiResponse );
        timeDiff = 0;
        _inner_noncer = QDateTime::currentSecsSinceEpoch();
        syncTimestamps();
        QTimer * t = new QTimer(this);
        t->setInterval( 30000 );
        connect( t, &QTimer::timeout, this, &SwiftApiClientBinance::syncTimestamps );
        t->start();

    }

    QByteArray sign( const QMap<QString,QString>& _params ) const {
        QStringList _parts;
        for( auto it = _params.begin(); it != _params.end(); it++ ) {
            _parts.push_back( it.key()+"="+it.value() );
        }
        const QString to_sign( _parts.join("&") );
        return QMessageAuthenticationCode::hash( to_sign.toUtf8(), getExchangeApiSecret().toUtf8(), QCryptographicHash::Sha256 ).toHex();
    }

signals:


    // SwiftApiClient interface
public:

    QString getExchangeName() const override;
    void getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getMarkets(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderPlace(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderCancel(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderGet(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderReplace(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawList(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getBalances(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getDeposits(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid) override;
public slots:
    void parseCustomApiResponse( const QString& method, const QByteArray& data ) {
        if ( method == "timesync" ) {
            QJsonObject j_data( QJsonDocument::fromJson( data ).object() );
                const quint64 servtime = j_data.value("serverTime").toVariant().toULongLong();
                const quint64 localtime = QDateTime::currentMSecsSinceEpoch();
                timeDiff = localtime - servtime;
        }
    }
    void syncTimestamps() {
        QNetworkRequest req( QUrl( "https://api.binance.com/api/v3/time" ) );
        QNetworkReply * rep = getManager()->get( req );
        rep->setProperty("customRequest", "timesync");
    }
private:
    quint64 getInnerNoncer();

    quint64 timeDiff;
    quint64 _inner_noncer;
    QMutex innermutex;
};

#endif // SWIFTAPICLIENTBINANCE_H
