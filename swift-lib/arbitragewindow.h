#ifndef ARBITRAGEWINDOW_H
#define ARBITRAGEWINDOW_H

#include "arbitragepair.h"


class ArbitrageWindow : public QObject
{
    Q_OBJECT
public:

    double formatAmount( const double& a ) const {
        return QString::number( a, 'f', _amount_precision ).toDouble();
    }
    double formatPrice( const double& p ) const {
        return QString::number( p, 'f', _price_precision ).toDouble();
    }
    bool isLocked() const;
    void lock ();
    void unlock();
    void placeOrders();
    quint32 getSellExchangeId() const;

    bool is_window_order( const QString& uid ) const;
    void addWindowOrderUid( const QString& uid );
    quint32 getSellCurrency() const;

    quint32 getBuyCurrency() const;

    void transferFunds();
    bool conversionStartStepRequired();
    bool manualStartTransferRequired();
    bool hasSprint();
    bool createSprint( const double& amount );
    QJsonObject createStep( const double& amount, const bool& is_sell, const quint32& num ) const;
    QJsonObject createTransferStep( const double& amount, const quint32& from, const quint32& to, const quint32& num );
    QJsonObject createConversionStep( const double& amount, const quint32& from, const quint32& num );
    QJsonArray explain(const double& amount );
    void calculateOrderbooks(const QHash<quint32, QMap<double,double>>&asks, const QHash<quint32, QMap<double,double>>&bids );
    explicit ArbitrageWindow(QObject *parent = nullptr);

    bool open();

    bool close();

    ArbitrageWindow( const QSqlRecord& rec, QObject *parent = nullptr );
    void update( const QSqlRecord& rec );
    bool enable();
    bool disable();
    bool check( const quint32& method );
    quint64 getTicksCount( ) const;
    double getFiatConversionRate( const quint32& exchange_id, const double& amount );


    QJsonObject toJson( const bool& short_version = false );

    QJsonObject j_prepared;

    double getRequestedBalance( const quint32& currency );
    double amountsumm() const;
    double sell_rate;
    double buy_rate;
    double sell_rate_avg() const;
    double buy_rate_avg() const;

    double getBuyBalance();
    double getSellBalance();

    double getOrderSize();

    double weight;
    bool isAvailable() const;
    bool isEnabled() const;
    bool isValid() const;
    bool isPositive() const;
    double getMinSize() const;

    double getDryProfitPercent();
    double getStatProfitPercent() const;
    double getRealProfitPercent() const;
    double getRealProfit( const double& amount ) const;
    double getSpread() const;
    double getSpreadPercent() const;

    void setAsClassicCandidat();
    void onErrorOrder( const QJsonObject& j_itm );
    quint32 sell_pair_id;
    quint32 buy_pair_id;
signals:



public slots:
    void onCompletedOrder( const QJsonObject& j_itm );
    void onPlacedOrder( const QJsonObject& j_itm );
    void onCanceledOrder( const QJsonObject& j_itm );
    void checkIsActive();
    void saveWindow();
    void onWampSession( Wamp::Session * sess );
    void publishData() {
        if ( session != nullptr && session->isJoined() ) {
          //  session->publish("swift.orders.feed.windows", {id, toJson(true)});
        }
    }
private:
    double min_spread;
    QStringList _placed_orders_local_ids;
    QHash<QString,QString> _window_params;
    QHash<QString,quint32> _window_params_index;
    Wamp::Session * session;
    QMap<double, double> asks_last;
    QMap<double, double> bids_last;
    bool sell_base_is_token;
    bool sell_price_is_token;
    bool buy_base_is_token;
    bool buy_price_is_token;

    bool _valid;
    QList<double> _spread_avg;
    QList<double> _sell_rate_avg;
    QList<double> _buy_rate_avg;
    QList<double> _amount_avg;

    double sell_fee;
    double buy_fee;

    QList<ArbitrageVariant> _candidats;
    quint32 id;
    quint64 ticks_cnt;
    quint32 candidats_cnt;



    quint32 sell_base_currency_id;
    quint32 sell_price_currency_id;

    quint32 buy_base_currency_id;
    quint32 buy_price_icurrency_d;

    double min_size;
    double max_size;
    double amount_summ;


    quint32 sell_exchange_id;
    quint32 buy_exchange_id;

    quint64 last_available_time;
    quint64 available_since;
    bool is_available;

    // State settings
    bool is_enabled;
    bool is_locked;
    bool sell_order_tested;
    bool buy_order_tested;
    bool base_deposit_tested;
    bool market_deposit_tested;
    bool base_withdraw_tested;
    bool market_withdraw_tested;

    double first_send_conversion_fee;
    double first_send_transaction_fee;
    double back_send_conversion_fee;
    double back_send_transaction_fee;
    double required_balance_sell;
    double required_balance_buy;
    ArbitragePair* getArbitragePair();
    QMutex mutex;
    QString bname;
    QString pname;
    double minspread;
    double bb;
    double sb;
    quint32 amount_scale;
    quint32 rate_scale;
    quint32 _amount_precision;
    quint32 _price_precision;

    double min_order_size;
    double max_order_size;
    double order_size_step;

};



#endif // ARBITRAGEWINDOW_H
