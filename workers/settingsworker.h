#ifndef SETTINGSWORKER_H
#define SETTINGSWORKER_H
#include "swiftworker.h"
#include <QObject>

class SettingsWorker : public SwiftWorker
{
    Q_OBJECT
public:
    SettingsWorker( QObject * parent = nullptr ) : SwiftWorker( parent ), assets( SwiftLib::getAssets() ) {
        loadAssets();
    }

    // SwiftWorker interface
public:
    void initWorker(Wamp::Session *sess) override;
    QString getWorkerName() const override;
    QString getValue( const QString& param ) {
        if ( _cached_params.contains( param ) ) {
            return _cached_params.value( param );
        }
        return SwiftLib::getSettings()->value(param, "0").toString();
    }
    void setValue( const QString&p, const QString&v ) {
        if ( !_cached_params.contains( p ) || _cached_params.value( p ) != v ) {
            _cached_params[p] = v;
            QSqlQuery q("UPDATE settings_storage SET value='"+v+"' WHERE param='"+p+"'");
            if ( !q.exec() ) {
                qWarning() << q.lastError().text();
            } else {
                session->publish("swift.assets.feed.settings", {p,v});
            }
        }
    }
    QJsonObject getExchangesFees() {
        QJsonObject j_ret;
        const QStringList exchs_names( assets->getExchangesNames() );
        for( auto it = exchs_names.begin(); it != exchs_names.end(); it++ ) {
            const quint32 eid = assets->getExchangeId( *it );
            const double trade_fee = assets->getExchangeTradeFee( eid );
            j_ret[ QString::number( eid ) ] = QString::number( trade_fee, 'f', 4 );
        }
        return j_ret;
    }

    QJsonObject getCurrenciesFees() {
        QJsonObject j_ret;
        const QList<quint32> cids( assets->getAllCurrencies() );
        for( auto it = cids.begin(); it != cids.end(); it++ ) {
            const quint32 cid = *it;
            const double withdraw_fee = assets->getCurrencyWithdrawFee( cid );
            j_ret[ QString::number( cid ) ] = QString::number( withdraw_fee, 'f', 4 );
        }
        return j_ret;
    }
public slots:
    void watch_dog() {
        if ( !_wdogs.isEmpty() ) {
            for( auto it = _wdogs.begin(); it != _wdogs.end(); it++ ) {
                if ( QDateTime::currentSecsSinceEpoch() - it.value() > 80 ) {
                    const QString module( it.key() );
                    if ( module == "balance" ) {
                        QProcess::startDetached("systemctl restart swift-worker-balances");
                    } else if ( module == "arbitrage") {
                        QProcess::startDetached("systemctl restart swift-bot");
                    }
                }
            }
        }
    }
    void loadAssets() {
        QJsonObject j_assets;
        QJsonObject a_coins;
        QJsonObject a_exchanges;
        QJsonObject a_pairs;
        QJsonObject a_arb_pairs;
        QJsonObject a_currencies;

        QSqlQuery q("SELECT * FROM coins");
        if ( q.exec() ) {
            while( q.next() ) {
                QJsonObject jitm;
                jitm["id"] = QString::number( q.value("id").toUInt() );
                jitm["name"] = q.value("name").toString();
                jitm["full_name"] =  q.value("full_name").toString();
                jitm["is_enabled"] = QString::number( q.value("is_enabled").toUInt() );
                a_coins[ QString::number( q.value("id").toUInt() ) ] = jitm;
            }
        }
        q.finish();

        j_assets["coins"] = a_coins;
        QSqlQuery q2("SELECT * FROM pairs");
        if ( q2.exec() ) {
            while( q2.next() ) {
                QJsonObject jitm;
                jitm["id"] = QString::number( q2.value("id").toUInt() );
                jitm["name"] = q2.value("name").toString();
                jitm["exchange_id"] = QString::number( q2.value("exchange_id").toUInt() );
                jitm["arbitrage_pair_id"] = QString::number( q2.value("arbitrage_pair_id").toUInt() );
                jitm["is_enabled"] = QString::number( q2.value("is_enabled").toUInt() );
                jitm["base_currency_id"] = QString::number( q2.value("base_currency_id").toUInt() );
                jitm["market_currency_id"] = QString::number( q2.value("market_currency_id").toUInt() );
                a_pairs[ QString::number( q2.value("id").toUInt() ) ] = jitm;
            }
        }
        q2.finish();

        QSqlQuery q3("SELECT * FROM arbitrage_pairs");
        if ( q3.exec() ) {
            while( q3.next() ) {
                QJsonObject jitm;
                jitm["id"] = QString::number( q3.value("id").toUInt() );
                jitm["base_coin_id"] = QString::number( q3.value("base_coin_id").toUInt() );
                jitm["price_coin_id"] = QString::number( q3.value("price_coin_id").toUInt() );
                jitm["name"] = q3.value("name").toString();
                jitm["is_enabled"] = QString::number( q3.value("is_enabled").toUInt() );
                a_arb_pairs[QString::number( q3.value("id").toUInt() )]=jitm;
            }
        }
        q3.finish();


        QSqlQuery q4("SELECT * FROM exchanges");
        if ( q4.exec() ) {
            while( q4.next() ) {
                QJsonObject jitm;
                jitm["id"] = QString::number( q4.value("id").toUInt() );
                jitm["name"] = q4.value("name").toString();
                jitm["is_enabled"] = QString::number( q4.value("is_enabled").toUInt() );
                jitm["fee"] = QString::number( q4.value("trade_fee").toDouble(), 'f', 4 );
                a_exchanges[QString::number( q4.value("id").toUInt() )]=jitm;
            }
        }
        q4.finish();

        QSqlQuery q5("SELECT * FROM currencies");
        if ( q5.exec() ) {
            while( q5.next() ) {
                QJsonObject jitm;
                jitm["id"] = QString::number( q5.value("id").toUInt() );
                jitm["coin_id"] = QString::number( q5.value("coin_id").toUInt() );
                jitm["exchange_id"] = QString::number( q5.value("exchange_id").toUInt() );
                jitm["name"] = q5.value("name").toString();
                jitm["is_enabled"] = QString::number( q5.value("is_enabled").toUInt() );
                jitm["fee"] = QString::number( q5.value("withdraw_fee").toDouble(), 'f', 4 );
                a_currencies[QString::number( q5.value("id").toUInt() )]=jitm;
            }
        }
        q5.finish();

        j_assets["exchanges"] = a_exchanges;
        j_assets["pairs"] = a_pairs;
        j_assets["arbitrage_pairs"] = a_arb_pairs;
        j_assets["currencies"] = a_currencies;
        _assets_cached = j_assets;
        QTimer::singleShot( 520000, this, &SettingsWorker::loadAssets );
    }
    void loadParams() {
        QSqlQuery q;
        if ( q.exec("SELECT * FROM `settings_storage`") ) {
            while ( q.next() ) {
                _cached_params.insert( q.value("param").toString(), q.value("value").toString() );
            }
            for( auto it = _cached_params.begin(); it != _cached_params.end(); it++ ) {
                SwiftLib::getSettings()->setValue( it.key(), it.value() );
            }
        } else {
            qWarning() << "ON GET PARAM" << q.lastError().text();
        }
        QTimer::singleShot( 30000, this, &SettingsWorker::loadParams );
    }
private:
    AssetsStorage * assets;
    QHash<QString,qint64> _wdogs;
    QHash<QString,QString> _cached_params;
    QJsonObject _assets_cached;
};

#endif // SETTINGSWORKER_H
