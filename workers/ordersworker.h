#ifndef ORDERSWORKER_H
#define ORDERSWORKER_H

#include <QObject>
#include <QSharedDataPointer>
#include "swiftlib.h"
#include <QQueue>
#include <QMutex>
#include <ordersstorage.h>

class OrdersWorker : public QObject
{
    Q_OBJECT
public:
    explicit OrdersWorker(QObject *parent = nullptr);
    OrdersWorker(const OrdersWorker &);
    OrdersWorker &operator=(const OrdersWorker &);
    ~OrdersWorker();
    static QString createOrderHash(const QJsonObject& j);
    void register_uuid_watch( const QString& uuid );

signals:
    void deleteOrderRec( const QJsonObject& j);
    void publishMessage( const QString& topic, const QJsonObject& msg );
    void saveOrder( const QJsonObject& j_order );
    void saveOrderState( const QJsonObject& j_order );
    void processApiHistory(  const QJsonArray& j_items );
    void processApiCurrent(  const QJsonArray& j_items, const quint32& market_privided=0 );

    void errorOrderEvent( const QJsonObject& j_itm );
    void completedOrderEvent( const QJsonObject& j_itm );
    void placedOrderEvent( const QJsonObject& j_itm );
    void canceledOrderEvent( const QJsonObject& j_itm );
public slots:
    void fixWinRelations();
    void checkUpdates();
    void updateConfig(const QString& conf );
    void startClient();
    void checkIndividualOpenState();
    void onProcessApiCurrent( const QJsonArray& j_items, const quint32& market_privided=0 );
    void onProcessApiHistory( const QJsonArray& j_items );

    void onErrorOrderEvent( const QJsonObject& j_itm );
    void onCompletedOrderEvent( const QJsonObject& j_itm );
    void onPlacedOrderEvent( const QJsonObject& j_itm );
    void onCanceledOrderEvent( const QJsonObject& j_itm );

    void onOrdersFromDb( const QJsonArray& stored_ );

    void onWampConnected(quint64);
    void checkAwaitingUpdates();


    void getOpenOrders();
    void getCompletedOrders();
private:
    QHash<quint32, quint64> _exch_last_requested_all;
    QStringList _loaded_history_remote_ids;
    QJsonObject checkState( const QJsonObject& j_obj, const quint32& state ) const;
    AssetsStorage * assets;
    QMutex mutex;

    // Таймеры
    QTimer * _update_markets_open_orders_timer;
    QTimer * _update_today_trade_history_timer;



    // WAMP
    Wamp::Session * session;
    QTcpSocket socket_in;
    QTcpSocket socket_out;
    QHash<QString, quint64> _close_timers;

    OrdersStorage * o_storage;
};

#endif // ORDERSWORKER_H
