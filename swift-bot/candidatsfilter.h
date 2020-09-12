#ifndef CANDIDATSFILTER_H
#define CANDIDATSFILTER_H

#include <QObject>
#include <QHash>
#include <QMap>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlError>
#include "../swift-lib/swiftlib.h"
#include "../swift-lib/arbitragepair.h"
#include <wampclient.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QTimer>
#include <QMutex>

class ArbitrageManager;

struct ArbitrageWindiowStats {
    ArbitrageWindiowStats();

    ArbitrageWindiowStats operator()( const ArbitrageWindiowStats& other );

    ArbitrageWindiowStats operator=( const ArbitrageWindiowStats& other );

    ArbitrageWindiowStats( const ArbitrageWindiowStats& other );
    void addTick();
    double sell_rate;
    double buy_rate;
    double spread_size;
    quint64 ticks;
};

struct ArbitrageWindowDefinition {
    ArbitrageWindowDefinition( const ArbitrageWindowDefinition& other );
    ArbitrageWindowDefinition operator()( const ArbitrageWindowDefinition& other );
    ArbitrageWindowDefinition operator=( const ArbitrageWindowDefinition& other );
    ArbitrageWindowDefinition();
    ArbitrageWindowDefinition(const quint32& _id, const quint32& _sell_exchange, const quint32& _sell_pair, const quint32& _buy_exchange, const quint32& _buy_pair );
    quint32 id;

    quint32 sell_exchange;
    quint32 sell_pair;
    quint32 buy_exchange;
    quint32 buy_pair;
    ArbitrageWindiowStats stats;
};


typedef QPair<quint32,quint32> window_key;
typedef QHash<window_key,ArbitrageWindowDefinition> ArbitrageWindows;




class CandidatsFilter : public QObject
{
    Q_OBJECT
public:
    explicit CandidatsFilter(QObject *parent = nullptr);
    quint32 getSystemMode() const;
signals:
    void readyCandidats( const QList<ArbitrageCandidat>& candidats );
    void publishMessage( const QString& topic, const QJsonObject& msg );
    void placeShiftOrder(  const quint32& pair_id, const double& amount, const double& rate, const quint32& type );

public slots:
    void sendArbitrageSnapshow() {
        // const QJsonObject j_msg(  arbitrage_manager->toJson( ) );
        // emit publishMessage("swiftbot.arbitrage.snapshot", j_msg );
    }
    void reportWindowsCollected();
    void onWampSession(Wamp::Session *session);
    void onOrderBooks( const QHash<quint32, QMap<double,double>> &asks, const QHash<quint32, QMap<double,double>> &bids );

    void onMoneyShiftExists( const double& amount, const double& rate, const bool& is_positive );
    void onMoneyZeroShift();
private:

    QList<ArbitrageCandidat> filterPair( const quint32& sell_pair, const quint32& buy_pair );
    ArbitrageWindows _windows;
    double getExchangeFee( const quint32& pair_id ) const;
    void loadPairPresets();
    void filterCandidats( const QList<quint32>& awins = QList<quint32>() );
    AssetsStorage * assets;
    // Ask - цена покупки, тут нас интересует минимальная цена, фильтруем с головы
    QMap<double,double> getAsks(const quint32& pair_id ) const;
    double getAsksAmount(const quint32& pair_id ) const {
        QList<double> a( getAsks( pair_id ).keys() );
        return std::accumulate( a.begin(), a.end(), 0.0 );
    }
    // Bid - цена продажи, нас интересует максимальная, фильтруем с конца
    QMap<double,double> getBids(const quint32& pair_id ) const;
    double getBidsAmount(const quint32& pair_id ) const {
        QList<double> a( getBids( pair_id ).keys() );
        return std::accumulate( a.begin(), a.end(), 0.0 );
    }
    QHash<quint32, QList<quint32>> _arbitrage_pairs;
    QHash<QPair<quint32,quint32>, quint32> _windows_rels;
    QHash<quint32,quint32> _pairs_exchange;
    QHash<quint32, QMap<double,double>> _asks;
    QHash<quint32, QMap<double,double>> _bids;
    QHash<quint32, double> _mid_rates;
    QMutex m;
    bool hasMoneyShift;
    bool hasPositiveMoneyShift;
    double money_shift_amount;
    double money_shift_rate;
    Wamp::Session * sess;
    ArbitrageManager* arbitrage_manager;
};

#endif // CANDIDATSFILTER_H
