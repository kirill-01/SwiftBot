#ifndef ARBITRAGEMANAGER_H
#define ARBITRAGEMANAGER_H

#include <QObject>
#include <QTimer>
#include "wamp.h"
#include "swiftlib.h"
#include <QMutex>
#include <QFile>
#define ORDERS_REL_FILENAME "/opt/swiftbot/orders_wins_rels.dat"

class ArbitrageChains;
class ArbitrageWindow;
class ArbitragePair;

class ArbitrageManager : public QObject
{
    friend class ArbitragePair;
    friend class ArbitrageWindow;
    Q_OBJECT
public:
    static QStringList getWindowOrdersUids( const quint32& wid );
    static QMap<quint32,QStringList> getWindowOrdersRels();
    explicit ArbitrageManager(QObject *parent = nullptr);
    QHash<quint32, QList<quint32>> getPairsRels() const;
    QJsonObject toJson(const bool & show_active_only = true);
    QJsonArray getWindowHistory( const quint32& window_id );
    QJsonArray getWindowTodayHistory( const quint32& window_id );
    QJsonArray explainWindow( const quint32& id, const double& amount );
    ArbitrageWindow *getWindow( const quint32& wid );
    bool hasWindow(const quint32& wid ) const;
    QMap<quint32, double> _aggregated_requests;
    QMap<quint32,QMap<quint32, double>> _requested_separated;
    void updateRequestedRecord( const quint32& cid, const double& am );
    void setRequested( const quint32& pair_id, const QMap<quint32, double>& part );

    static void linkOrders( const QString& first, const QString& second, const quint32& wid );
    void linkOrder( const QString& local_id, const quint32& wid, const double& am );

    void updateParam(const QString &param, const QString &value);

    QString getParam( const QString& p );
    QHash<QString,QString> _cached_params;
    quint32 getWindowOrdersCount(const quint32& wid ) const;

    QMap<quint32, QStringList> _linked_orders;
    double getPairMinRate( const quint32& pid ) const;
    double getPairMaxRate( const quint32& pid ) const;
    double getPairAvgRate( const quint32& pid ) const;
signals:
    void sessionConnected( Wamp::Session * s );
    void sendWampMessage( const QString& topic, const QString& message );
    void errorOrderEvent( const QJsonObject& j_itm );
    void completedOrderEvent( const QJsonObject& j_itm );
    void placedOrderEvent( const QJsonObject& j_itm );
    void canceledOrderEvent( const QJsonObject& j_itm );
    void checkOrderbooks(const QHash<quint32, QMap<double, double> > &asks, const QHash<quint32, QMap<double, double> > &bids);
    void tocalculateOrderbooks( const QHash<quint32, QMap<double,double>>&asks, const QHash<quint32, QMap<double,double>>&bids );
public slots:
    void updateRequested();
    void calculateOrderbooks( const QHash<quint32, QMap<double,double>>&asks, const QHash<quint32, QMap<double,double>>&bids );
    void checkNewWindows();

    void onWampSession( Wamp::Session * session );
    void loadPairSettings();
    void loadDbPairs();
    void loadPairRatesHistory();
    void saveOrdersRelsToDisk();
    void loadOrdersRelFromDisk();

    void sendSnapshot();
private:
    QMap<quint32,double> _avg_rates;
    QMap<quint32,double> _min_rates;
    QMap<quint32,double> _max_rates;
    QList<quint32> _en_exchanges;
    QList<quint32> _en_pairs;
    QList<quint32> _en_currencies;
    QString ethusd;
    QString btcusd;
    QMap<quint32, ArbitrageChains*> _chains;
    QMutex mut;
    quint64 ticks;
    QTimer * create_db_windows_timer;

    QMap<quint32,ArbitragePair*> _arbitrage_pairs;
    QMap<quint32,quint32> _ap;
    QHash<QPair<quint32,quint32>, quint32> _pairs_rels;
    Wamp::Session * sess;
    QString dbname;
    QMap<quint32,QList<double>> _window_amounts;
};

#endif // ARBITRAGEMANAGER_H
