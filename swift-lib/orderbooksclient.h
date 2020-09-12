#ifndef ORDERBOOKSCLIENT_H
#define ORDERBOOKSCLIENT_H

#include <QObject>
#include "wampclient.h"

class OrderbooksClient : public QObject
{
    Q_OBJECT
public:
    explicit OrderbooksClient(QObject *parent = nullptr);

signals:
    void orderbooks( const QHash<quint32, QMap<double,double>>& asks,  const QHash<quint32, QMap<double,double>>& bids );
public slots:
    void onOrderBooks(const QJsonObject& j_books);
    void onWampSession( Wamp::Session * sess );
    void stopClient();
    void startClient( const QList<quint32>& filter = QList<quint32>() );
private:
    QList<quint32> _filtered_pairs;
    WampClient * orderbooks_client;
    Wamp::Session * session;
};

#endif // ORDERBOOKSCLIENT_H
