#ifndef STRATEGYWORKER_H
#define STRATEGYWORKER_H

#include <QObject>
#include "../swift-lib/apircpclient.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

class StrategyWorker : public QObject
{
    Q_OBJECT
public:
    explicit StrategyWorker(QObject *parent = nullptr);
    void getBalancesInfo() {
        QList<quint32> _target_currencies( assets->getAllCurrencies() );
        const QString msg = session->call("swiftbot.balances.get.all").toString();
        const QJsonArray _bals( QJsonDocument::fromJson( msg.toUtf8() ).array() );
        //const QJsonObject j_bals( QJsonDocument::fromJson(  ).object() );
        QMap<quint32,double> _bal_summary;
        for( auto it = _bals.begin(); it != _bals.end(); it++ ) {
            const quint32 coin_id = it->toObject().value("coin_id").toString().toUInt();
            _bal_summary[ coin_id ] += it->toObject().value("usd").toString().toDouble();
        }

        if ( target_arbitrage_pair == 2 ) {
            const double base_pcnt = _bal_summary.value(7) / ( _bal_summary.value(7) + _bal_summary.value(2) ) * 100;
            const double price_pcnt = _bal_summary.value(2) / ( _bal_summary.value(7) + _bal_summary.value(2) ) * 100;
        }
        const QString str_win( session->call("swiftbot.arbitrage.window.get", {QString::number(target_window_id)}).toString() );
        const QJsonObject j_window( QJsonDocument::fromJson( str_win.toUtf8() ).object() );
    }
signals:

public slots:
    void onWampSession(Wamp::Session *s);
private:
    AssetsStorage * assets;
    Wamp::Session * session;
    double max_finance_percent_to_use;
    quint32 target_arbitrage_pair;
    quint32 target_window_id;
    double total_base_balance;
    double total_price_balance;
};

#endif // STRATEGYWORKER_H
