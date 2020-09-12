#ifndef ORDERBOOKSKEEPER_H
#define ORDERBOOKSKEEPER_H

#include <QObject>
#include <wampclient.h>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QTimer>
#include <QHash>
#include <QMap>
#include <QMutex>

class OrderbooksKeeper : public QObject
{
    Q_OBJECT
public:
    explicit OrderbooksKeeper(QObject *parent = nullptr);

signals:
    void orderbooks( const QHash<quint32, QMap<double,double>>& asks, const QHash<quint32, QMap<double,double>> & bids );
    void publishMessage( const QString& topic, const QJsonObject& j_val );
public slots:
    void onOrderBooks( const QJsonObject& j_books);
    void onRealmSession(Wamp::Session * session) {
        realm_session = session;
    }

    void flushOrderbooks();
    void onWampSession(Wamp::Session * session );
    void startKeeper();
private:
    QMutex mutex;
    QHash<quint32, QDateTime> _asks_timestamps;
    QHash<quint32, QDateTime> _bids_timestamps;
    QHash<quint32, QMap<double,double>> _asks;
    QHash<quint32, QMap<double,double>> _bids;
    WampClient * orderbooks_client;
    Wamp::Session * sess;
    Wamp::Session * realm_session;
    QTimer * flush_timer;
};



#endif // ORDERBOOKSKEEPER_H
