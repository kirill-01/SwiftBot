#ifndef SWIFTAPICLIENT_H
#define SWIFTAPICLIENT_H

#include <QObject>
#include "wamp.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QMutex>
#include <QQueue>
#include <QTimer>
#include <QNetworkAccessManager>

#include <QMessageAuthenticationCode>
#include <QUrlQuery>
#include "swiftlib.h"
#include "swiftworker.h"

class SwiftApiClient : public SwiftWorker
{
    Q_OBJECT
public:
    SwiftApiClient(QObject *parent = nullptr);


    Q_ENUMS(AsyncMethods)
    Q_ENUMS(ApiErrors)

    enum ApiErrors {
        HttpResponseError
    };

    enum AsyncMethods {
        TimeSyncMethod,
        GetCurrencies,
        GetMarkets,
        GetOrderbooks,
        OrderPlace,
        OrderCancel,
        OrderGet,
        OrderReplace,
        WithdrawGetLimits,
        WithdrawList,
        WithdrawHistory,
        WithdrawCancel,
        WithdrawCreate,
        WithdrawInner,
        WithdrawGetFee,
        GetBalances,
        GetDeposits,
        GetDepositAddress,
        TradeHistory,
        TradeOpenOrders,
        TradeGetFee,
        TradeGetLimits
    };

    static QString getMethodName( const SwiftApiClient::AsyncMethods& method );
    quint64 _req_total_counter;
    quint64 _req_total_limit;
    QHash<AsyncMethods, quint64> _methods_counter;
    QHash<AsyncMethods, quint64> _methods_minimum_delay;
    // Interface
    quint32 getExchangeId();;
    virtual QString getExchangeName() const=0;

    virtual QString getApiVersionString();

    QString getExchangeApiKey() const;
    QString getExchangeApiSecret() const;
    QString getExchangeApiAdditional() const;

    virtual void getCurrencies( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void getMarkets( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void getOrderbooks( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void orderPlace( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void orderCancel( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void orderGet( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void orderReplace( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void withdrawGetLimits( const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void withdrawList(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0 )=0;
    virtual void withdrawHistory(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void withdrawCancel(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void withdrawCreate(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void withdrawInner(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void withdrawGetFee(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void getBalances(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void getDeposits(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void getDepositAddress(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void tradeHistory(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void tradeOpenOrders(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void tradeGetFee(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void tradeGetLimits(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0)=0;
    virtual void customParser( const SwiftApiClient::AsyncMethods& method, const QJsonObject& j_result );
    virtual void customMethod(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0) {
        Q_UNUSED(j_params)
        Q_UNUSED(async_uuid)
    }
    // Async methods
    quint64 createAsyncWaiter( const SwiftApiClient::AsyncMethods& method,  const QJsonObject& j_params );

    QNetworkAccessManager * getManager();
    void registerAsyncResult( const quint64& uuid, const QJsonObject& j_result );

    QHash<quint32, double> _withdraw_fees;
    QHash<quint32, double> _trade_fees;
    QHash<quint32, double> _min_trade_sizes;
    QJsonArray _cached_currencies;
    QJsonArray _cached_markets;
    QJsonArray _cached_balances;
    QJsonArray _cached_orders;
    QTimer *limits_timer;
    bool is_only_public;
    void addLog( const QString& message );
    void addError(const QString& message  );
    bool wamp_connected;
    QString api_key;
    QString api_secret;
    QString api_user;
    void registerOrder( const QJsonObject& j, const quint64& uuid );
    QJsonObject getRegisteredOrder( const quint64& uuid );
    bool isDebug() const;

    void setCachedBalance( const quint32& currency_id, const double& total, const double& inner );

    double getTotalCachedBalance( const quint32& currency_id ) const;

    double getInnerCachedBalance( const quint32& currency_id ) const;

public slots:
    void resetLimits();


    void unfreeze();
    void sendApiStateReport();
    void onWampSession_( Wamp::Session * session_ );

    void onNetworkReply( QNetworkReply * reply );

    void processAsyncQueueResult();

    void processAsyncQueue();

    void onApiResponseParsed( const quint64& uuid, const QJsonObject& j_result );

    void customMethodParsers(  const SwiftApiClient::AsyncMethods& method, const QJsonObject& j_result   );
signals:
    void pubWampResponse( const QString& topic, const QJsonObject& j_res, const quint64& uuid );
    void pubWamp( const QString& topic, const QJsonObject& j_res);
    void customRequestResult( const QString& reqname, const QByteArray& data );
    void apiParsedResponse( const SwiftApiClient::AsyncMethods& method, const QJsonObject& j_result );
    void parseApiResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods& method, const QByteArray& data );

protected:
    void unrealizedMethod(const QJsonObject &j_params, const quint64 &async_uuid);

    QString paramMapToStr(const QMap<QString, QString>& params) const;
    bool isParamExist(const QStringList& params, const QJsonObject& j_params, QString& error) const;
    bool isUintValid(quint32 id, QString& error, const QString& id_name) const;
    bool isDoublePlus(double val, QString& error, const QString& id_name) const;
    bool isSideValid(const QString& side, QString& error) const;
    bool isUintInRange(quint32 val, quint32 r_min, quint32 r_max, bool inclusive=true) const;

private:

    void methodState( const AsyncMethods& method, const bool& result );
    bool publishWamp( const SwiftLib::WampTopics& topic, const QJsonObject& obj, const quint64& async_uuid=0 );


    // Async helpers
    quint64 registerAsyncCall( const SwiftApiClient::AsyncMethods& method );

    quint64 getNextUuid();



    bool sendAsyncResult( const quint64& uuid  );

    // Private properties
    // Pointers


    QTimer * _queues_timer;
    QTimer * _state_timer;

    // Network, websockets
    QNetworkAccessManager * netman;

    // Storages
    QMutex mutex;
    quint64 _uuids_counter;
    QHash<quint64, QJsonObject> _async_results;
    QHash<quint64, SwiftApiClient::AsyncMethods> _async_dictionary;
    QHash<quint64, QJsonObject> _async_params;
    QDateTime _started_at;
    QDateTime _last_request_at;
    QJsonArray j_errors;
    QList<quint64> _asyncs;

    QHash<QString, QJsonObject> _orders_cache_local;
    QHash<QString, QJsonObject> _orders_cache_remote;

    QHash<quint64,QString> _orders_local_ids_cache;
    QHash<quint64,QString> _orders_remote_ids_cache;

    // Queues
    QQueue<quint64> _async_queue;
    QQueue<quint64> _async_results_queue;


    // Balances cached
    QHash<quint32, double> _balances_total_cached;
    QHash<quint32, double> _balances_inner_cached;
    QHash<AsyncMethods, Wamp::Endpoint> _endpoints;
    QHash<quint64, QString> _regitered_orders;
    QHash<QString, QJsonObject> _regitered_orders_objects;
    QHash<double, quint32> _wait_order_window;

    QHash<AsyncMethods, quint64> _last_methods_ts;

    bool debug;
    QHash<SwiftApiClient::AsyncMethods, QJsonObject> _cached_results;
    bool request_pause;

    // SwiftWorker interface
public:
    void initWorker(Wamp::Session *sess) override;
    QString getWorkerName() const override;

    // SwiftWorker interface
public:
    QStringList listenParams() override;
    void onParamChanged(const QString &param, const QString &value) override;
};

#endif // SWIFTAPICLIENT_H
