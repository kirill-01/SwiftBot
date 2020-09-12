#ifndef BALANCEWORKER_H
#define BALANCEWORKER_H

#include <QObject>
#include "swiftworker.h"
#include <QTimeZone>
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


struct balancesStorage {
    quint32 currency_id;
    quint32 coin_id;
    quint32 exchange_id;

    double available;
    double total;
    double usd;
    double required;
};

struct transactionsStorage {
    quint32 id;
    QString remote_id;
    QString local_id;
    quint32 currency_id;
    quint32 exchange_id;
    double amount;
    QString address;
    QString tx;
    QString status;

    double fee;
    QDateTime created_at;
    QJsonObject toJson() {
        QJsonObject j({
          {"id", QString::number( id ) },
          {"remote_id", remote_id },
          {"local_id", local_id },
          {"currency_id", QString::number( currency_id ) },
          {"exchange_id", QString::number( exchange_id ) },
          {"amount", QString::number( amount,'f',8 ) },
          {"address",address },
          {"tx",  tx },
          {"status",  status  },
          {"fee", QString::number( fee, 'f',4) },
          {"created_at", QString::number( created_at.toSecsSinceEpoch() ) }
        });
        return j;
    }
};

class BalanceWorker : public SwiftWorker
{
    Q_OBJECT
public:
    BalanceWorker( QObject * parent = nullptr );
signals:
    void eventUpdate( const QJsonObject& data );
    void eventDeposit( const QJsonObject& data );
    void eventDeposits( const QJsonObject& data );
    void eventWithdraw( const QJsonObject& data );
    void eventWithdraws( const QJsonObject& data );
    // SwiftWorker interface
public:
    void initWorker(Wamp::Session *sess) override;
    QString getWorkerName() const override;
    QJsonArray getRatesDiff() {
        QMap<quint32, RateChange> _rs( _rates_stats );
        QJsonArray r;
        for( auto it = _rs.begin(); it != _rs.end(); it++ ) {
            r.push_back( it->toJson() );
        }
        return r;
    }
public slots:
    void requestBalances();
    void processWithdrawsQueue();
    void processBalancesQueue();
    void processDepositsQueue();
    void requestWithdraws();
    void requestDeposits();
    void onUpdate( const QJsonObject& data );

    void updateRates();
    void updateRequested() {
        QSqlQuery q("SELECT currency_id, AVG(requested) as r FROM currency_requested WHERE date(ts)=curdate() GROUP BY currency_id");
        if ( q.exec() ) {
            while( q.next() ) {
                _storage[ q.value("currency_id").toUInt() ].required = q.value("r").toDouble();
            }
        }
        QTimer::singleShot( 120000, this, &BalanceWorker::updateRequested );
    }

    void onDeposit( const QJsonObject& data );
    void onDeposits( const QJsonObject& data );
    void onWithdraw( const QJsonObject& data );
    void onWithdraws( const QJsonObject& data );
private:
    QMap<quint32, balancesStorage> storage() const { return _storage; }
    AssetsStorage * assets;
    QMap<quint32, balancesStorage> _storage;
    QMap<quint32, double> __apair_rates;
    QMap<quint32, double> __pair_rates;
    QHash<QString,transactionsStorage> _transactions_deposits;
    QQueue<QPair<quint32,quint32>> _queued_withdraws_get;
    QQueue<QPair<quint32,quint32>> _queued_deposits_get;
    QQueue<QPair<quint32,quint32>> _queued_balances_get;
    QHash<QString,transactionsStorage> _transactions_withdraws;
    QMap<quint32, RateChange> _rates_stats;

};

#endif // BALANCEWORKER_H
