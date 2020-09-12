#ifndef ARBITRAGEPAIR_H
#define ARBITRAGEPAIR_H

#include <QObject>
#include <QMap>
#include <QPair>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "swiftlib.h"
#include <QQueue>
#include "wamp.h"
#include <QElapsedTimer>
#include "systemevent.h"
#include "orderparams.h"


class ArbitragePair;
class ArbitrageWindow;
class ArbitrageManager;

struct ArbitrageCandidat {
    explicit ArbitrageCandidat(
            const quint32& _arbitrage_pair_id = 0,
            const quint32& _sell_pair_id = 0,
            const quint32& _buy_pair_id = 0,
            const double& _sell_rate = 0,
            const double& _sell_price = 0,
            const double& _sell_fee = 0,
            const double& _buy_rate = 0,
            const double& _buy_price = 0,
            const double& _buy_fee = 0,
            const double& _amount = 0
            );

    ArbitrageCandidat( const ArbitrageCandidat& other );
    ArbitrageCandidat operator=(const ArbitrageCandidat& other);
    quint32 arbitrage_pair_id;
    quint32 sell_pair_id;
    quint32 buy_pair_id;
    double sell_rate;
    double sell_price;
    double sell_fee;
    double buy_rate;
    double buy_price;
    double buy_fee;
    double amount;
    double profit() const;
    QJsonObject toJson() const;
};




struct ArbitrageVariant {


    QJsonObject toJson();
    ArbitrageVariant operator=(const ArbitrageVariant& other);
    explicit ArbitrageVariant( const double& p=0, const double& a=0, const double& rs=0, const double& rb=0,const double& fs=0,const double& fb=0 );
    ArbitrageVariant( const ArbitrageVariant& other );

    double profit;
    double amount;

    double rate_sell;
    double rate_buy;

    double fee_sell;
    double fee_buy;

};



class ArbitragePair : public QObject
{
    Q_OBJECT
public:
    friend class ArbitrageManager;
    friend class ArbitrageWindow;
    QMap<quint32,double> _cached_reqs;
    QMap<quint32,double> _cached_bals;
    double getBalance( const quint32& cid );
    double getBalanceAvailable( const quint32& cid, const bool& force=false );
    void setCoins( const quint32& base_cn, const quint32& mkt_coin ) {
        sell_coin_id = base_cn;
        buy_coin_id = mkt_coin;
    }
    bool hasWindow( const quint32& id ) const;

    bool isLocked() const {
        return is_locked;
    }
    void lock() {
        lock_time = QDateTime::currentSecsSinceEpoch();
        is_locked = true;
    }
    void unlock() {
        is_locked = false;
    }

    void prepareCachedBals();

    QMap<quint32,QMap<quint32,double>> _required_summary;
    void setRequestedAmount( const quint32 & wid, const quint32& cid, const double& amount ) {
        _required_summary[wid][cid] = amount;
    }
    void updateCurrenciesRequirements();

    ArbitrageManager * getManager();
    ArbitrageWindow * getWindow( const quint32& id );

    QList<QPair<OrderParams*, OrderParams*>> _orders_candidats;

    void insertOrderCandidat( OrderParams* sell_order, OrderParams* buy_order );
    void placeOrders();

    QJsonObject toJson(const bool & show_active_only = true);
    bool sortWindows(const QJsonObject& a,const QJsonObject& b );
    explicit ArbitragePair(QObject *parent = nullptr);
    ArbitragePair( const QSqlRecord& rec, QObject *parent = nullptr );

    void loadDbWindow( const quint32& sell_pair, const quint32& buy_pair );

    void loadDbWindow( const quint32& window_id );


    void createNewWindows();
    double getSpreadSize() const;
    void update( const QSqlRecord& rec );

    quint32 createWindow( const quint32& arbitrage_pair_id,const  QPair<quint32,quint32>& trade_pairs );

    quint64 getTotalTicks();
    QList<QPair<quint32,quint32>> getTradePairs() const;
    Wamp::Session * session;
    QHash<QPair<quint32,quint32>, quint32> _windows_rels;
    QList<QVariantList> _classic_candidats;
    void addClassicCandidat( const quint32& wid, const double& size, const double&  sell_rate, const double&  buy_rate ) {
        _classic_candidats.push_back({wid, size, sell_rate, buy_rate });
    }
public slots:
    void loadDbWindows();
    void calculateOrderbooks( const QHash<quint32, QMap<double,double>>&asks, const QHash<quint32, QMap<double,double>>&bids );

    void reloadCache();
    void setWampSession( Wamp::Session * sess );
    void onErrorOrder( const QJsonObject& j_itm );
    void onCompletedOrder( const QJsonObject& j_itm );
    void onPlacedOrder( const QJsonObject& j_itm );
    void onCanceledOrder( const QJsonObject& j_itm );

    void checkCandidats();

    void sendLog( const QString& msg );
    void sendDebug( const QString& msg );
signals:
    void debug( const QString & msg );
    void log( const QString& msg );
    void tocalculateOrderbooks( const QHash<quint32, QMap<double,double>>&asks, const QHash<quint32, QMap<double,double>>&bids );
    void hasWampSession( Wamp::Session * sess );
    void errorOrder( const QJsonObject& j_itm );
    void completedOrder( const QJsonObject& j_itm );
    void placedOrder( const QJsonObject& j_itm );
    void canceledOrder( const QJsonObject& j_itm );
private:

    bool is_locked;
    qint64 lock_time;
    QList<quint32> _valid_pairs;
    QList<double> _avg_amount;
    quint64 all_ticks;
    quint32 id;
    QString base_currency_name;
    QString price_currency_name;

    quint32 sell_coin_id;
    quint32 buy_coin_id;

    double min_spread;
    double max_spread;

    double spread_percent;
    QMutex m;

    double ttl_funds;
    QQueue<QPair<quint32,quint32>> _new_windows;
    QMap<quint32,ArbitrageWindow*> _windows;

    double sellpct;
    QHash<quint32, QMap<double, double>> _a;
    QHash<quint32, QMap<double, double>> _b;
    quint32 amount_scale;
    quint32 rate_scale;
};



#endif // ARBITRAGEPAIR_H
