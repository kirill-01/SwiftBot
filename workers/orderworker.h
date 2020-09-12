#ifndef ORDERWORKER_H
#define ORDERWORKER_H

#include <QObject>
#include "swiftworker.h"
#include "ordersstorage.h"
#include <QQueue>

class OrderWorker : public SwiftWorker
{
    Q_OBJECT
public:
    OrderWorker( QObject * parent = nullptr );

    // SwiftWorker interface
public:
    QStringList listenParams() override;
    void onParamChanged(const QString &param, const QString &value) override;
    void initWorker(Wamp::Session *sess) override;
    QString getWorkerName() const override;
    QJsonObject getTradingSummary( const QVariantList&v);
signals:
    void errorOrderEvent( const QJsonObject& j_itm );
    void updatedOrderEvent( const QJsonObject& j_itm );
    void completedOrderEvent( const QJsonObject& j_itm );
    void placedOrderEvent( const QJsonObject& j_itm );
    void canceledOrderEvent( const QJsonObject& j_itm );
    void historyOrderEvent( const QJsonObject& j_itm );
    void activeOrderEvent( const QJsonObject& j_itm );
    void saveOrder( const QJsonObject& j_itm );
    void saveOrderState( const QJsonObject& j_itm );
public slots:
    void onOrdersFromDb(const QJsonArray &stored_);
    void checkUpdates() {
        const QJsonArray j_updated_orders( o_storage->getUpdatedOrders() );
        if ( !j_updated_orders.isEmpty() ) {
            for( auto it = j_updated_orders.begin(); it != j_updated_orders.end(); it++ ) {
                emit saveOrder(  it->toObject() );
            }
            o_storage->clearUpdated();
        }

        QTimer::singleShot( 45000, this, &OrderWorker::checkUpdates );
    }
    void startClient();
    void processActiveQueue();
    void processHistoryQueue();
    void requestActives();
    void requestHistory();
    void onErrorOrderEvent( const QJsonObject& j_itm );
    void onUpdatedOrderEvent( const QJsonObject& j_itm );
    void onCompletedOrderEvent( const QJsonObject& j_itm );
    void onPlacedOrderEvent( const QJsonObject& j_itm );
    void onCanceledOrderEvent( const QJsonObject& j_itm );
    void onHistoryOrderEvent( const QJsonObject& j_itm );
    void onActiveOrderEvent( const QJsonObject& j_itm );
private:
    QTcpSocket socket_in;
    QTcpSocket socket_out;
    OrdersStorage * o_storage;
    AssetsStorage * assets;
    QQueue<QPair<quint32,quint32>> _queued_active_get;
    QQueue<QPair<quint32,quint32>> _queued_history_get;
};

#endif // ORDERWORKER_H
