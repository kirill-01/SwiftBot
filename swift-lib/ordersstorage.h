#ifndef ORDERSSTORAGE_H
#define ORDERSSTORAGE_H

#include <QObject>
#include <QSharedDataPointer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMutex>
#include <QDateTime>
#include <QMap>

class OrdersStorageData;

class OrdersStorage : public QObject
{
    Q_OBJECT
public:
    QJsonObject prepareOrderData( const QJsonObject& j_order );
    explicit OrdersStorage(QObject *parent = nullptr);
    OrdersStorage(const OrdersStorage &);
    OrdersStorage &operator=(const OrdersStorage &);
    ~OrdersStorage();
    void addOrder( const QJsonObject& order_json );
    void updateOrder( const QJsonObject& order_json );
    bool isExistsRemoteId( const QString& uid ) const {
        return _index_ext_id.contains( uid );
    }
    bool isExistsLocalId( const QString& uid ) const {
        return _index_ext_id.contains( uid );
    }
    QJsonObject getOrder( const QString& remote_uid, const QString& local_uid = "" ) const;
    QJsonArray getOpenOrders();

    QJsonArray getClosedOrders();
    QJsonArray getClosedOrdersByPair( const quint32 pair_id );
    QJsonArray getClosedOrdersByPair( const quint32 pair_id, const QString& date );
    QJsonArray getOopenOrdersByPair( const quint32 pair_id );
    QJsonArray getUpdatedOrders();

    void clearUpdated() {
        _index_updated_orders.clear();
    }

    QString uniTime( const quint64& ts ) {
        if ( ts > static_cast<quint64>( QDateTime::currentSecsSinceEpoch() ) ) {
            return QString::number( QDateTime::fromSecsSinceEpoch( ts / 1000, Qt::UTC ).toSecsSinceEpoch() );
        } else {
            return QString::number( QDateTime::fromSecsSinceEpoch( ts, Qt::UTC ).toSecsSinceEpoch() );
        }
    }

    QString getLocalIdByRemote( const QString& remote_id, const quint32& exchange_id );
signals:
public slots:
    // Обслуживание хранилища
    void provision();


private:
    QMutex m;
    QHash<QString, int> _index_ext_id;
    QHash<QString, int> _index_loc_id;
    QMap<QString, int> _index_open_orders;
    QMap<QString, int> _index_closed_orders;
    QList<quint32> _index_updated_orders;
    QSharedDataPointer<OrdersStorageData> data;
};

#endif // ORDERSSTORAGE_H
