#ifndef BALANCESWORKER_H
#define BALANCESWORKER_H

#include <QObject>
#include "../swift-lib/apircpclient.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QQueue>

struct RateChange {
    quint32 pid;
    double prev_sell;
    double sell;
    double prev_buy;
    double buy;
    double diff_sell;
    double diff_buy;
    quint32 orders;
    QJsonObject toJson() const {
        QJsonObject j;
        AssetsStorage * assets = SwiftLib::getAssets();
        j["pair_id"] = QString::number( pid );
        j["prev_sell"] = QString::number( prev_sell, 'f', 8 );
        j["sell"] = QString::number( sell, 'f', 8 );
        j["prev_buy"] = QString::number(  prev_buy, 'f', 8  );
        j["buy"] = QString::number(  buy, 'f', 8  );
        j["diff_sell"] = QString::number(  diff_sell, 'f', 2  );
        j["change"] = QString::number(  diff_sell > diff_buy ? diff_sell : diff_buy, 'f', 2  );
        j["rate"] = QString::number( buy > sell ? buy : sell,'f',8 );
        j["diff_buy"] = QString::number(  diff_sell, 'f', 2  );
        j["orders_count"] = QString::number( orders );
        const quint32 eid =  assets->getMarketExchangeId(pid );
        j["exchange_id"] = QString::number( eid );
        j["exchange_name"] = assets->getExchangeName( eid );
        j["arbitrage_pair_id"] = QString::number( assets->getMarketArbitragePairId( pid ) );
        return j;
    }
};

struct ExchangeTradeStatus {
    quint32 exchange_id;

    quint32 sell_orders_cnt;
    double sell_order_size;
    double sell_amount;
    double sell_price;

    quint32 buy_orders_cnt;
    double buy_order_size;
    double buy_amount;
    double buy_price;
};

class BalancesWorker : public QObject
{
    Q_OBJECT
public:
    explicit BalancesWorker(QObject *parent = nullptr);
    QJsonArray getBalances() const;
    bool reserveInternal( const quint32& currency_id, const double& amount );
    bool reserveCancelInternal( const quint32& currency_id, const double& amount );
    QJsonObject getTradingSummary(const QVariantList&v);
    void reportBalancesCollected();
    void reportSmallBalances();
    QJsonArray last_loaded_deposits;
    QJsonArray last_loaded_withdraws;
    QJsonArray getDepositsHistory() ;

    QJsonArray getWithdrawHistory() ;

signals:
    void balancesEvent( const QJsonObject& event );
    void depositsEvent( const QJsonObject& event );
    void withdrawsEvent( const QJsonObject& event );

    void withdrawCeated( const QJsonObject& event );
    void withdrawHistory( const QJsonObject& event );
    void deposiHistory(const QJsonObject& event );
public slots:
    void updateRates();
    void onUpdateEvent( const QJsonObject& event );
    void onDepositsEvent(const QJsonObject& event);
    void onWithdrawsEvent(const QJsonObject& event);

    void onDepositHistory(const QJsonObject& event);
    void onWithdrawCeated( const QJsonObject& event );
    void onWithdrawHistory( const QJsonObject& event );

    void processQueue();
    void getDepositsStatuses();
    void getWithdrawStatuses();

    void beginCollect();
    void onWampSession( Wamp::Session * sess );
    void onBalance( const QJsonObject& res );
    void calculateSummaries();
    void saveBalanceHistory();
    void calculateBalancerTransfers();

    void loadAddresses();
    /**
     * @brief getRate
     * @deprecated
     */
    void getRate();

    void getExternalBtcBalance();
    void updateRatesCache();
    void getTodayRateDiff();
    void getTodayTradeProfit();

    void insertAddress( const quint32& exch, const quint32& coin, const QString& address );

    void calculatePeriods() {

        prtiodic_summary["daily_profit"] = QString::number( 0, 'f', 8);
        prtiodic_summary["weekly_profit"] = QString::number( 0, 'f', 8);
        prtiodic_summary["monthly_profit"] = QString::number( 0, 'f', 8);
        prtiodic_summary["avg_profit"] = QString::number( 0, 'f', 8);

    }

    QJsonArray getRatesDiff() {
        QMap<quint32, RateChange> _rs( _rates_stats );
        QJsonArray r;
        for( auto it = _rs.begin(); it != _rs.end(); it++ ) {
            r.push_back( it->toJson() );
        }
        return r;
    }
private:

    double last_profit_value;
    QMap<quint32, double> last_rate_diff_value;

    QMap<quint32,double> __cached_rates;
    QMap<quint32,double> __cached_rates_all;
    QStringList savedTransactionsHasheasWithdraw;
    QMap<QString,QString> savedTransactionsHasheasWithdrawStatus;
    QStringList savedTransactionsHasheasDeposit;
    double getSellRate( const quint32& pair_id );
    QJsonObject prtiodic_summary;



    double getBuyRate( const quint32& pair_id );
    QJsonArray getAsks() const;
    QJsonArray getBids() const;
    bool balancer_used;
    QHash<QPair<quint32,quint32>,double> _transfer_candidats;
    QHash<quint32,double> _btc_sources;
    QHash<quint32,double> _usd_sources;

    QHash<quint32,double> _btc_targets;
    QHash<quint32,double> _usd_targets;

    QNetworkAccessManager * netman;


    void saveReport();
    void addLog( const QString& message );
    void addError( const QString& message );
    QMutex mutex;
    AssetsStorage * assets;
    QTimer * queue_timer;
    QTimer * summaries_timer;

    quint64 _waiters;
    QList<quint64> _waiters_balances;
    QHash<quint64,quint32> _waiters_exchs;
    Wamp::Session * session;
    QHash<quint32, QJsonObject> _mapped;

    QHash<quint32,double> _bals_available;
    QHash<quint32,double> _bals_total;
    QHash<quint32,double> _bals_reserved;
    QHash<quint32,double> _bals_orders;
    QHash<quint32,double> _bals_requested;

    QHash<quint32,double> _coin_total;

    QHash<quint32, double> _internal_reserve;

    QQueue<quint32> _requests_queue;
    double _last_btc_usd_rate;



    double __total_btc;
    double __total_usd;

    double __in_orders_btc;
    double __in_orders_usd;

    double __reserved_btc;
    double __reserved_usd;

    double __available_btc;
    double __available_usd;


    double _total_btc;
    double _total_usd;

    double _in_orders_btc;
    double _in_orders_usd;

    double _reserved_btc;
    double _reserved_usd;

    double _available_btc;
    double _available_usd;

    double _ext_wallet_balance;

    double _current_profit;
    quint32 _current_orders;
    double _retransfer_reserved_kraken_usdt;

    QHash<quint32,double> __balances_sended;

    QHash<quint32,quint64> _errs_counter;
    QHash<quint32,quint64> _last_exchange_request_timings;

    QHash<quint32, quint64> _delayed_exchanges;
    QList<quint32> _broken_exchanges;
    QHash<quint32,QHash<quint32,double>> _balances_group_by_exchange;


    QMap<quint32,double> _last_arbitrage_pairs_rates;
    QMap<quint32,double> _last_pairs_rates;
    QHash<quint32,double> _active_withdraws;
    QHash<quint32,double> _active_deposits;
    QJsonArray asks;
    QJsonArray bids;
    bool balancer_enabled;
    QJsonArray _pending_deposits;
    QJsonArray _pending_withdraws;

    bool hasDepositAddress( const quint32& currency_id, const QString& token_type ) const;

    quint32 getWithdrawCurrencyId( const quint32& exchange_id, const quint32& coin_id ) const;

    quint32 getDepositCurrencyId( const quint32& exchange_id, const quint32& coin_id ) const;


    QMap<quint32, RateChange> _rates_stats;
    QString dbname;
    QJsonArray j_addresses;
    QJsonArray j_addresses_insert;
    QSqlDatabase _ch;
};

#endif // BALANCESWORKER_H
