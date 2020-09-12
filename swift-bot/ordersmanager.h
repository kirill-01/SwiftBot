#ifndef ORDERSMANAGER_H
#define ORDERSMANAGER_H

#include <QObject>
#include <QDebug>
#include "../swift-lib/arbitragepair.h"
#include "wampclient.h"
#include "swiftlib.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QMutex>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>



class OrdersManager : public QObject
{
    Q_OBJECT
public:
    explicit OrdersManager(QObject *parent = nullptr);
signals:
    void currentRate( const double& rate );
    void orderPlaced();
    void requestExchangeBalances( const quint32& exchange_id );
    void publishMessage( const QString& topic, const QJsonObject& message );
    void asyncOrderPlaced();
    void asyncTaskFinished( const quint64& uuid );
public slots:
    void clearLocks();
    void onPlaceOrder( const quint32& pair_id, const double& amount, const double& rate, const quint32& type  );
    void onWampSession( Wamp::Session * sess );
    void onCandidats( const QList<ArbitrageCandidat>& candidats );

    void loadCurrencyTotalUsage();
    void getAllOpenOrders();

private:
    quint32 system_mode;
    quint32 getMode() const {
        return system_mode;
    }
    QHash<quint32, quint64> _lock_market_time;
    QHash<quint32,double> createMoneyRefillRequests(const QList<ArbitrageCandidat>& candidats, const QHash<quint32,double>& _balances );

    QList<ArbitrageCandidat> createOrdersQueue( const QList<ArbitrageCandidat>& candidats);

    QList<quint32> _locked_markets;

    QList<QPair<quint32,double>> _used_virtual_asks;
    QList<QPair<quint32,double>> _used_virtual_bids;
    QList<quint32> _used_pairs;
    bool placeOrder( const ArbitrageCandidat& candidat );
    QList<quint64> _waitings;
    QHash<quint64,QJsonObject> _async_results;
    QJsonArray _open_orders;
    QHash<quint32, double> _balances;
    QHash<quint32, double> _balances_reserved;
    QHash<quint32, quint32> _requests_counter;
    QHash<quint32, double> _balances_requested;
    QHash<quint32, double> _currency_in_orders;
    double _rate;
    double _rate_trend;
    double _rate24;
    double _rate1;
    QStringList _sell_orders_hashes;
    QStringList _buy_orders_hashes;
    Wamp::Session * session;
    QTimer * _rates_timer;
    QTimer * _open_orders_timer;
    QMutex m;
    QVector<double> _rates;
    AssetsStorage* assets;
    bool _is_candidats_placing;


};

#endif // ORDERSMANAGER_H
