#ifndef ORDERBOOKSNODE_H
#define ORDERBOOKSNODE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QWebSocket>
#include <QMutex>
#define SUPPORTED_EXCHANGES "cexio,bittrex,lbank,kucoin,binance,kraken,livecoin,lakebtc,exmo,hitbtc,bitfinex,okcoin,zb,huobi,idcm"
#define RECORDS_LIMIT 20

class OrderbooksNode : public QObject
{
    Q_OBJECT
public:
    enum NodeErrors {
        UnknownExchange,
        WebsocketAuthError
    };

    enum NodeStates {
        NodeCreated,
        NodeStarting,
        NodeConnected,
        NodeError,
        NodeShutdown
    };

    explicit OrderbooksNode(QObject *parent = nullptr);
    void setExchange( const QString& exchange_name, const QString& target_pair, const quint32& target_pair_id );
    void setExchange( const QString& exchange_name, const QList<QPair<QString,quint32>>& pairs );

signals:
    void orderbooks( const QJsonArray& a, const QJsonArray& b );
    void nodeError( OrderbooksNode::NodeErrors e );
    void aboutToClose();

public slots:
    void stopNode();
    void startNode();
    void onRawOrderbooks( const QJsonArray&a, const QJsonArray&b, const quint32 &custom_pair_id = 0 );
private:
    quint64 last_pong;
    QMap<double, QJsonArray> getIndexedAsks() const;
    QMap<double, QJsonArray> getIndexedBids() const;
    bool rebuildIndexes();

    QJsonArray getAsks() const;
    QJsonArray getBids() const;

    void initLakebtcInterval();
    void initLivecoinInterval();

    void initExmoInterval();

    void initBitfinexInterval();
    void initLivecoinWs() {

    }
    void initKucoinInterval();
    void initZbInterval();
    void initLbankInterval();
    void initLbankWs();
    void initBittrexInterval();
    void initCexioInterval();
    void initCexioWs();
    void initOkCoinInterval();
    void initHuobiInterval();

    void initNodeExchange();

    void initHitbtcInterval();
    void initKrakenInterval();
    void initKrakenWs();
    void initBinanceInterval();
    void initBinanceWs();

    void initIdcmInterval();

    quint32 getPairId( const QString& pairname ) {
        if ( _target_pairs_rels.contains( pairname ) ) {
            return _target_pairs_rels.value( pairname );
        } else {
            qWarning() << pairname;
            return 0;
        }
    }

    QMutex mutex;
    QNetworkAccessManager * netmanager;
    QSharedPointer<QWebSocket> websocket;
    QTimer * _request_interval_timer;
    QTimer * _publish_interval_timer;
    quint64 req_noncer;
    quint32 _request_interval;
    quint32 _publish_interval;
    NodeStates _current_state;

    QString _exchange_name;
    QString _target_pair;
    quint32 _target_pair_id;
    QList<QPair<QString,quint32>> _target_pairs;
    QMap<QString,quint32> _target_pairs_rels;
    // Orderbooks storage
    QMap<double, QJsonArray> _asks_indexed;
    QMap<double, QJsonArray> _bids_indexed;

    QJsonArray _asks;
    QJsonArray _bids;

};

#endif // ORDERBOOKSNODE_H
