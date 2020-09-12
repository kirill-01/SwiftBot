#include "balancesworker.h"
#include "systemevent.h"
#include <QNetworkInterface>
#include <QProcess>
BalancesWorker::BalancesWorker(QObject *parent) :
    QObject(parent), last_profit_value(0), balancer_used(true), netman( new QNetworkAccessManager ), assets( SwiftLib::getAssets() ),
    queue_timer( new QTimer(this ) ),
    summaries_timer( new QTimer(this ) ),
    session( nullptr ),
    _ext_wallet_balance(0),
    _retransfer_reserved_kraken_usdt(0)

{

    dbname = SwiftLib::getSettings()->value("MYSQL_DBNAME", "swiftbot").toString();
    connect( this, &BalancesWorker::balancesEvent, this, &BalancesWorker::onUpdateEvent );
    connect( this, &BalancesWorker::depositsEvent, this, &BalancesWorker::onDepositsEvent );
    connect( this, &BalancesWorker::withdrawsEvent, this, &BalancesWorker::onWithdrawsEvent );
    connect( this, &BalancesWorker::withdrawCeated, this, &BalancesWorker::onWithdrawCeated );
    connect( this, &BalancesWorker::withdrawHistory, this, &BalancesWorker::onWithdrawHistory );
    connect( this, &BalancesWorker::deposiHistory, this, &BalancesWorker::onDepositHistory );
    queue_timer->setInterval( SwiftLib::getSettings()->value("BALANCES_GET_INTERVAL", 10000).toUInt() );



    last_loaded_deposits = getDepositsHistory();
    last_loaded_withdraws = getWithdrawHistory();
    _ch = QSqlDatabase::addDatabase("QMYSQL", "clickhouse-def");
    _ch.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
    _ch.setPort( 9004 );

    _ch.setUserName( "default" );
    _ch.setPassword( "clickhouse" );
    _ch.setDatabaseName( "default" );
    if ( !_ch.open() ) {
        qWarning() << _ch.lastError().text();
    }
    updateRates();

}

QJsonArray BalancesWorker::getBalances() const {
    QJsonArray r;
    const QHash<quint32, QJsonObject> m( _mapped );
    for( auto it = m.begin(); it != m.end(); it++ ) {
        r.push_back( it.value() );
    }
    return r;
}

bool BalancesWorker::reserveInternal(const quint32 &currency_id, const double &amount) {
        _internal_reserve[currency_id] += amount;
        return true;
}


bool BalancesWorker::reserveCancelInternal(const quint32 &currency_id, const double &amount) {
    if ( _internal_reserve.value( currency_id ) >= amount ) {
        _internal_reserve[currency_id] -= amount;
        return true;
    } else {
        _internal_reserve.remove( currency_id );
        return false;
    }
}

QJsonObject BalancesWorker::getTradingSummary( const QVariantList&v)  {
    //QMutexLocker lock( &mutex );
    quint32 orders_total=0;

    QMap<quint32,double> _sold;
    QMap<quint32,double> _buy;
    QMap<quint32,double> _spend;
    QMap<quint32,double> _received;
    double _fees = 0;

    QJsonObject j_ret;
    QString dt;
    if ( v.isEmpty() ) {
         dt = "curdate()";
    } else {
        dt = "STR_TO_DATE('"+v.at(0).toString()+"', '%m/%d/%Y')";
    }
    QSqlQuery q("SELECT count(*) as orders_cnt, sum(`o`.`amount`) as trade_amount, sum(`o`.`price`) as trade_summ, sum(`o`.`fee`) as fees_paid, `o`.`type`, `p`.`arbitrage_pair_id` as `pair_id` FROM `orders` `o` LEFT JOIN `pairs` `p` ON `p`.`id`=`o`.`pair_id` WHERE date(`o`.`ts`)="+dt+" AND `o`.`status`<=2 AND `p`.`arbitrage_pair_id`>0 GROUP BY `o`.`type`, `p`.`arbitrage_pair_id`");
    if ( q.exec() ) {
        while ( q.next() ) {
            const quint32 pid = q.value("pair_id").toUInt();
            const quint32 type = q.value("type").toUInt();
            if ( type == 0 ) {
                _sold[ pid ] = q.value("trade_amount").toDouble();
                _received[ pid ] = q.value("trade_summ").toDouble();
            } else {
                _buy[ pid ] = q.value("trade_amount").toDouble();
                _spend[ pid ] = q.value("trade_summ").toDouble();
            }
            orders_total += q.value("orders_cnt").toUInt();
            _fees += q.value("fees_paid").toDouble();
        }
        q.finish();
    } else {
        qWarning() << "Error receive orders stats";
        qApp->exit( 1 );
    }
    QJsonArray j_pairs_summary;
    const QList<quint32> _arb_pairs({1,2});
    double tradeamount = 0;
    for( auto it = _received.begin(); it != _received.end(); it++ ) {
        tradeamount += it.value();
    }
    for( auto it = _spend.begin(); it != _spend.end(); it++ ) {
        tradeamount += it.value();
    }
    QJsonArray _j_pairs_stats;
    for ( auto it = _arb_pairs.begin(); it != _arb_pairs.end(); it++ ) {
        QJsonObject j_rec;
        j_rec["sold"] = _sold.value( *it );
        j_rec["buy"] = _buy.value( *it );
        j_rec["spend"] = _spend.value( *it );
        j_rec["received"] = _received.value( *it );
        j_rec["arbitrage_pair_id"] = QString::number( *it );
        j_rec["base_coin_name"] = assets->getArbitragePairBaseCurrencyName( *it );
        j_rec["market_coin_name"] = assets->getArbitragePairMarketCurrencyName( *it );
        j_rec["pair_name"] = assets->getArbitragePairName( *it );
        if ( _sold.value( *it ) > 0 || _buy.value( *it ) > 0 ) {
            _j_pairs_stats.push_back( j_rec );
        }
    }
    j_ret["pairs"] = _j_pairs_stats;
    j_ret["trade_fee"] = QString::number( _fees, 'f', 8 );
    j_ret["total_trade"] = QString::number( tradeamount, 'f', 8 );
    j_ret["orders_cnt"] = QString::number( orders_total );
    return j_ret;
}

void BalancesWorker::reportBalancesCollected() {
    if ( session != nullptr && session->isJoined() ) {
        session->call("swiftbot.system.set.hasbalances");
    }
}

void BalancesWorker::reportSmallBalances() {
    if ( session != nullptr && session->isJoined() ) {
        session->call("swiftbot.system.set.smallbalances");
    }
}

QJsonArray BalancesWorker::getDepositsHistory() {
    QMutexLocker lock( &mutex );
    QJsonArray j_it;
    if ( last_loaded_deposits.isEmpty() ) {
        QSqlQuery q;
        if ( q.exec("SELECT id,status,created_at,amount,currency_id FROM "+dbname+".deposits ORDER BY created_at DESC") ) {
            while( q.next() ) {
                QJsonObject j_itm;
                j_itm["id"] = QString::number( q.value("id").toUInt() );
                j_itm["type"] = "deposit";
               // j_itm["tx"] = q.value("txhash").toString();
                j_itm["currency_id"] = QString::number( q.value("currency_id").toUInt() );
               // j_itm["exchange_id"] = QString::number( assets->getCurrencyExchangeId( q.value("currency_id").toUInt() ) );
                j_itm["currency_name"] = assets->getCurrencyName(q.value("currency_id").toUInt()).replace("xbt","BTC");
                j_itm["exchange_name"] = assets->getCurrencyExchangeName( q.value("currency_id").toUInt() );
                j_itm["amount"] = QString::number( q.value("amount").toDouble(),'f',8 );
                j_itm["created_at"] = QString::number( q.value("created_at").toDateTime().toSecsSinceEpoch() );
                j_itm["status"] = q.value("status").toString();
                //j_itm["address"] = q.value("address").toString();
                j_it.push_back( j_itm );
            }
            last_loaded_deposits = j_it;
        } else {
            qWarning() << q.lastError().text();
            qApp->exit( 1 );
        }
        q.finish();
    } else {
        j_it = last_loaded_deposits;
    }
    return j_it;

}

QJsonArray BalancesWorker::getWithdrawHistory() {
    QMutexLocker lock( &mutex );
    QJsonArray j_items;
    if ( last_loaded_withdraws.isEmpty() ) {
        QSqlQuery q;
        if ( q.exec("SELECT id,status,created_at,amount,txhash,currency_id,address FROM "+dbname+".withdraws ORDER BY created_at DESC") ) {
            while( q.next() ) {
                QJsonObject j_itm;
                j_itm["id"] = QString::number( q.value("id").toUInt() );
                j_itm["type"] = "withdraw";
                j_itm["tx"] = q.value("txhash").toString();
                j_itm["currency_id"] = QString::number( q.value("currency_id").toUInt() );
                j_itm["exchange_id"] = QString::number( assets->getCurrencyExchangeId( q.value("currency_id").toUInt() ) );
                j_itm["currency_name"] = assets->getCurrencyName(q.value("currency_id").toUInt()).replace("xbt","BTC");
                j_itm["exchange_name"] = assets->getCurrencyExchangeName( q.value("currency_id").toUInt() );
                j_itm["amount"] = QString::number( q.value("amount").toDouble(),'f',8 );
                j_itm["created_at"] = QString::number( q.value("created_at").toDateTime().toSecsSinceEpoch() );
                j_itm["status"] = q.value("status").toString();
                j_itm["address"] = q.value("address").toString();
                j_itm["target_exchange"] = assets->getAddressExchange( q.value("address").toString() );
                j_items.push_back( j_itm );
            }
            last_loaded_withdraws = j_items;
        } else {
            qWarning() << q.lastError().text();
            qApp->exit( 1 );
        }
    } else {
        j_items = last_loaded_withdraws;
    }
    return j_items;
}

void BalancesWorker::updateRates() {
    QMap<quint32, QMap<quint32,double>> _asks;
    QMap<quint32, QMap<quint32,double>> _bids;
    QSqlQuery q(_ch);
    if ( q.exec("select d,toHour(ts) as h,avg(rate) as r,pair from asks WHERE ts>=toStartOfInterval(now(), INTERVAL "+SwiftLib::getSettings()->value("RATES_INTERVAL","3").toString()+" hour)  group by d,pair,toHour(ts)") ) {
        while( q.next() ) {
            const quint32 pid( q.value("pair").toUInt() );
            const quint32 h( q.value("h").toUInt() );
            const double r( q.value("r").toDouble());
            _asks[pid][h] = r;
        }
    }
    if ( q.exec("select d,toHour(ts) as h,avg(rate) as r,pair from bids WHERE ts>=toStartOfInterval(now(), INTERVAL "+SwiftLib::getSettings()->value("RATES_INTERVAL","3").toString()+" hour)  group by d,pair,toHour(ts)") ) {
        while( q.next() ) {
            const quint32 pid( q.value("pair").toUInt() );
            const quint32 h( q.value("h").toUInt() );
            const double r( q.value("r").toDouble());
            _bids[pid][h] = r;
        }
    }
    for( auto it = _asks.begin(); it != _asks.end(); it++ ) {
        const quint32 pid = it.key();
        const double current_buy_rate = _asks[pid].last();
        const double prev_buy_rate = _asks[pid].first();
        const double buy_change = ( current_buy_rate - prev_buy_rate ) / prev_buy_rate * 100;
        const double current_sell_rate = _bids[pid].last();
        const double prev_sell_rate = _bids[pid].first();
        const double sell_change = ( current_sell_rate - prev_sell_rate ) / prev_sell_rate * 100;
        _rates_stats[ pid ].buy = current_buy_rate;
        _rates_stats[ pid ].sell = current_sell_rate;
        _rates_stats[ pid ].prev_buy = prev_buy_rate;
        _rates_stats[ pid ].prev_sell = prev_sell_rate;
        _rates_stats[ pid ].diff_sell = sell_change;
        _rates_stats[ pid ].diff_buy = buy_change;
        _rates_stats[ pid ].pid = pid;
        _rates_stats[ pid ].orders = 0;
    }
    QSqlQuery q2("SELECT count(*) as cnt, pair_id FROM orders WHERE date(ts)=curdate() GROUP by pair_id");
    if ( q2.exec() ) {
        while( q2.next() ) {
            const quint32 pid = q2.value("pair_id").toUInt();
            const quint32 ocnt = q2.value("cnt").toUInt();
            _rates_stats[ pid ].orders = ocnt;
        }
    }
    QTimer::singleShot( 15000, this, &BalancesWorker::updateRates );
}

void BalancesWorker::onUpdateEvent(const QJsonObject &event) {
    bool changed = false;
    const quint32 exchid = event.value("exchange_id").toString().toUInt();
    QList<quint32> curs( assets->getExchangeCurrencies( exchid ) );
    QJsonArray j_arr( event.value("balances").toArray() );
    QList<quint32> _exsts;
    for( auto it = j_arr.begin(); it != j_arr.end(); it++ ) {
        _exsts.push_back( it->toObject().value("currency_id").toString().toUInt() );
    }
    for ( auto it = curs.begin(); it != curs.end(); it++ ) {
        if ( !_exsts.contains( *it ) ) {
            QJsonObject j_itm;
            j_itm["currency_id"] = QString::number( *it );
            j_itm["exchange_id"] = QString::number( exchid );
            j_itm["total"] = QString::number( 0, 'f', 8 );
            j_itm["available"] = QString::number( 0, 'f', 8 );
            j_arr.push_back( j_itm );
        }
    }
    QList<quint32> _ps;
    for( auto it = j_arr.begin(); it != j_arr.end(); it++ ) {
        const quint32 curr( it->toObject().value("currency_id").toString().toUInt() );
        if( curr > 0 ) {
            changed = true;
            QJsonObject j_currency( it->toObject() );
            if ( !j_currency.contains( "coin_id" ) ) {
                quint32 cid = j_currency.value("currency_id").toString().toUInt();
                j_currency["coin_id"] = QString::number( assets->getCurrencyCoin( cid ) );
                j_currency["is_token"] = assets->isToken( cid );
                j_currency["exchange_name"] = assets->getCurrencyExchangeName( cid );
                const quint32 _cid = assets->getCurrencyCoin( cid );
                j_currency["usd"] = QString::number( j_currency.value("total").toString().toDouble() * __cached_rates.value( _cid, 1 ), 'f', 8 );
            }
            _mapped[ curr ] = j_currency;
        }
    }

    if ( changed ) {
        calculateSummaries();
    }
}

void BalancesWorker::onDepositsEvent(const QJsonObject &event) {
    Q_UNUSED(event)
}

void BalancesWorker::onWithdrawsEvent(const QJsonObject &event) {
    Q_UNUSED(event)
}


void BalancesWorker::onDepositHistory(const QJsonObject &event) {
    const QJsonArray records( event.value("deposits").toArray() );
    QSqlQuery q;
    q.prepare("INSERT INTO `"+dbname+"`.`deposits` (`local_id`,`remote_id`,`currency_id`,`amount`,`address`,`created_at`,`txhash`,`status`,`fee`) VALUES (:local_id,:remote_id,:currency_id,:amount,:address,:created_at,:txhash,:status,:fee) ON DUPLICATE KEY UPDATE local_id=:local_id, status=:status,txhash=:txhash,created_at=:created_at");
    for ( auto it = records.begin(); it != records.end(); it++ ) {
            const QString remote_uuid_generated( QCryptographicHash::hash( QJsonDocument(it->toObject() ).toJson( QJsonDocument::Compact ), QCryptographicHash::Sha512 ).toHex() );
            const QString txhash( it->toObject().value("tx").toString() );
            const QString remote_uuid( it->toObject().contains("remote_id") ? it->toObject().value("remote_id").toString() : it->toObject().contains("tx") ? txhash : remote_uuid_generated );

            QString uuid( remote_uuid.isEmpty() ? remote_uuid_generated : remote_uuid );
            if ( uuid.length() > 250 ) {
                uuid = uuid.mid(0, 250 );
            }

            if ( savedTransactionsHasheasDeposit.contains( uuid ) ) {
            } else {
                if ( !uuid.isEmpty() ) {
                    q.bindValue(":remote_id",uuid );
                    q.bindValue(":local_id", uuid );
                    q.bindValue(":currency_id",it->toObject().value("currency_id").toString("0").toUInt(0) );
                    q.bindValue(":amount",it->toObject().value("amount").toString("0.0").toDouble(0) );
                    q.bindValue(":address",it->toObject().value("deposit_address").toString("???") );
                    q.bindValue(":created_at", QDateTime::fromSecsSinceEpoch( it->toObject().value("created_at").toString("0").toUInt(0) ) );
                    q.bindValue(":txhash",it->toObject().value("tx").toString("#") );
                    q.bindValue(":status",it->toObject().value("status").toString("complete") );
                    q.bindValue(":fee", 0 );
                    if ( !q.exec() ) {
                        qWarning() << "DEPOSIT HISTORY ERROR SAVE" << q.executedQuery() << it->toObject() << uuid;
                    }
                    savedTransactionsHasheasDeposit.push_back( uuid );
                }
            }
    }
    q.finish();

}

void BalancesWorker::onWithdrawCeated(const QJsonObject &event){
    Q_UNUSED(event)
}

void BalancesWorker::onWithdrawHistory(const QJsonObject &event){
    const QJsonArray records( event.value("withdraws").toArray() );
    QSqlQuery q;
    q.prepare("INSERT INTO `"+dbname+"`.`withdraws` (`local_id`,`remote_id`,`currency_id`,`amount`,`address`,`created_at`,`txhash`,`status`,`fee`) VALUES (:local_id,:remote_id,:currency_id,:amount,:address,:created_at,:txhash,:status,:fee) ON DUPLICATE KEY UPDATE local_id=:local_id, status=:status,txhash=:txhash,created_at=:created_at");
    for ( auto it = records.begin(); it != records.end(); it++ ) {
        if ( !it->toObject().value("deposit_address").isNull() ) {
        const QString remote_uuid_generated( QCryptographicHash::hash( QJsonDocument(it->toObject() ).toJson( QJsonDocument::Compact ), QCryptographicHash::Sha512 ).toHex() );
        const QString txhash( it->toObject().value("tx").toString() );
        const QString remote_uuid( it->toObject().contains("remote_id") ? it->toObject().value("remote_id").toString() : it->toObject().contains("tx") ? txhash : remote_uuid_generated );
        QString uuid( remote_uuid.isEmpty() ? remote_uuid_generated : remote_uuid );
        if ( uuid.length() > 250 ) {
            uuid = uuid.mid(0, 250 );
        }
        if ( savedTransactionsHasheasWithdraw.contains( uuid ) ) {
        } else {
            if ( !uuid.isEmpty() ) {
                q.bindValue(":remote_id",uuid );
                q.bindValue(":local_id", uuid );
                q.bindValue(":currency_id",it->toObject().value("currency_id").toString("0").toUInt() );
                q.bindValue(":amount",it->toObject().value("amount").toString("0.0").toDouble() );
                q.bindValue(":address",it->toObject().value("deposit_address").toString("@#") );
                q.bindValue(":created_at", QDateTime::fromSecsSinceEpoch( it->toObject().value("created_at").toVariant().toULongLong() ) );
                q.bindValue(":txhash",it->toObject().value("tx").toString("#") );
                q.bindValue(":status",it->toObject().value("status").toString("complete") );
                q.bindValue(":fee", assets->getCurrencyWithdrawFee( it->toObject().value("currency_id").toString("0").toUInt(0) ) );
                if ( !q.exec() ) {
                    qWarning() << "WITHDRAW HISTORY ERROR SAVE" << q.executedQuery() << it->toObject() << uuid;
                }
            }
            savedTransactionsHasheasWithdraw.push_back( uuid );
        }
        }
    }
    q.finish();

}

void BalancesWorker::processQueue() {
    if ( session != nullptr && session->isJoined() ) {
        const QStringList exch( assets->getExchangesNames() );
        const QStringList enabled( SwiftLib::getSettings()->value("EXCHANGES").toString().split(","));
        for( auto it = exch.begin(); it != exch.end(); it++ ) {
            if ( enabled.contains( *it ) ) {
                const quint32 exchid = assets->getExchangeId( *it );
                const QString top( SwiftLib::getRpcPath( exchid, SwiftLib::WampExchangesRpcMethods::GetBalances )  );
                const quint64 uid = session->call( top, QVariantList({false}) ).toString().toULongLong();
                Q_UNUSED(uid);
            }
        }
    }

}

void BalancesWorker::getDepositsStatuses() {
    if ( session != nullptr && session->isJoined() ) {
        const QStringList exch( assets->getExchangesNames() );
        const QStringList enabled( SwiftLib::getSettings()->value("EXCHANGES").toString().split(","));
        for( auto it = exch.begin(); it != exch.end(); it++ ) {
            if ( enabled.contains( *it ) ) {
                const quint32 exchid = assets->getExchangeId( *it );
                if ( assets->isSeparatedApi( exchid ) ) {
                    QList<quint32> _curs( assets->getExchangeCurrencies( exchid ) );
                    for( auto itc = _curs.begin(); itc != _curs.end(); itc++ ) {
                        QJsonObject j_p;
                        j_p["currency_id"] = QString::number( *itc );
                        const QString strp( QJsonDocument( j_p ).toJson( QJsonDocument::Compact ) );
                        const QString top( SwiftLib::getRpcPath( exchid, SwiftLib::WampExchangesRpcMethods::GetDeposits)  );
                        const quint64 uid = session->call( top, QVariantList({strp} ) ).toString().toULongLong();
                        if ( uid > 0 ) {
                            // Correct request ?
                        }
                    }
                } else {
                    const QString top( SwiftLib::getRpcPath( exchid, SwiftLib::WampExchangesRpcMethods::GetDeposits )  );
                    const quint64 uid = session->call( top, QVariantList({false}) ).toString().toULongLong();
                    if ( uid > 0 ) {
                        // Correct request ?
                    }
                }
            }
        }
    }
    QTimer::singleShot(235000, this, &BalancesWorker::getDepositsStatuses );
}

void BalancesWorker::getWithdrawStatuses()
{
    if ( session != nullptr && session->isJoined() ) {
        const QStringList exch( assets->getExchangesNames() );
        const QStringList enabled( SwiftLib::getSettings()->value("EXCHANGES").toString().split(","));
        for( auto it = exch.begin(); it != exch.end(); it++ ) {
            if ( enabled.contains( *it ) ) {
                const quint32 exchid = assets->getExchangeId( *it );
                if ( assets->isSeparatedApi( exchid ) ) {
                    QList<quint32> _curs( assets->getExchangeCurrencies( exchid ) );
                    for( auto itc = _curs.begin(); itc != _curs.end(); itc++ ) {
                        QJsonObject j_p;
                        j_p["currency_id"] = QString::number( *itc );
                        const QString strp( QJsonDocument( j_p ).toJson( QJsonDocument::Compact ) );
                        const QString top( SwiftLib::getRpcPath( exchid, SwiftLib::WampExchangesRpcMethods::WithdrawHistory)  );
                        const quint64 uid = session->call( top, QVariantList({strp} ) ).toString().toULongLong();
                        if ( uid > 0 ) {
                            // Correct request ?
                        }
                    }
                } else {
                    const QString top( SwiftLib::getRpcPath( exchid, SwiftLib::WampExchangesRpcMethods::WithdrawHistory )  );
                    const quint64 uid = session->call( top, QVariantList({false}) ).toString().toULongLong();
                    if ( uid > 0 ) {
                        // Correct request ?
                    }
                }
            }
        }
    }
    QTimer::singleShot(335000, this, &BalancesWorker::getWithdrawStatuses );
}

void BalancesWorker::beginCollect() {

}

void BalancesWorker::onWampSession(Wamp::Session *sess) {
    session = sess;
    QJsonObject msg;
    msg["text"] = "BALANCES_MODULE_STARTED";
    msg["cssClass"] = "ordersuccess";
    msg["closeTimeout"] = "1000";
    msg["position"] = "top";
    session->publishMessage("swiftbot.system.messages", msg);

    session->provide("swiftbot.components.balances.status", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        Q_UNUSED(v)
        QString hostip;
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost && address != QHostAddress("10.9.0.1") )
                 hostip = address.toString();
        }
        QJsonObject j_ret;
        j_ret["status"] = "running";
        j_ret["name"] = "worker_balances";
        j_ret["location"] = hostip;
        QJsonObject options;
        j_ret["options"] = options;
        const QString str(QJsonDocument(j_ret).toJson( QJsonDocument::Compact ) );
        return str;
    });

    session->provide("swiftbot.components.balances.command", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        const QString cmd = v.at(0).toString();
        if ( cmd == "restart") {
            qApp->exit();
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        }
        return true;
    });

    session->subscribe("swiftbot.system.events.balance",[&](const QVariantList&v, const QVariantMap&m) {
        const QJsonObject j_event( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        SwiftEvent event( j_event );
        if ( event.getEventName() == "update" ) {
            const QJsonObject targ( event.getEventTarget() );
            emit balancesEvent( event.getEventTarget() );
        }
        Q_UNUSED(m);
    });

    session->subscribe("swiftbot.system.events.withdraws",[&](const QVariantList&v, const QVariantMap&m) {
        const QJsonObject j_event( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        WithdrawEvent event( j_event );
        if ( event.getEventName() == "created" ) {
            const QJsonObject targ( event.getEventTarget() );
            emit withdrawCeated( event.getEventTarget() );
        } else if ( event.getEventName() == "history" ) {
            const QJsonObject targ( event.getEventTarget() );
            emit withdrawHistory( event.getEventTarget() );
        }
        Q_UNUSED(m);
    });

    /*
    session->provide("swiftbot.balance.external.btc.wallet",[&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(v)
        Q_UNUSED(m)
        return _ext_wallet_balance;
    });
*/
    session->subscribe("swiftbot.system.events.deposits",[&](const QVariantList&v, const QVariantMap&m) {
        const QJsonObject j_event( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );

        DepositEvent event( j_event );
        if ( event.getEventName() == "history" ) {
            const QJsonObject targ( event.getEventTarget() );
            emit deposiHistory( event.getEventTarget() );
        }
        Q_UNUSED(m);
    });

    session->provide( "swiftbot.balances.usd", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        double bals = 0;
        for ( auto i = _bals_total.begin(); i != _bals_total.end(); i++ ) {
            if ( i.value() > 0 ) {
                const quint32 _cid = assets->getCurrencyCoin( i.key() );
                bals += i.value() * __cached_rates.value( _cid, 1 );
            }
        }
        return bals;
    } );
    session->provide( "swiftbot.balances.get.all", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QString r( QJsonDocument( getBalances() ).toJson( QJsonDocument::Compact ) );
        return r;
    } );
    session->provide( "swiftbot.balances.get.available", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        const double result = _bals_available.value( currency_id, 0 ) > 0 ? _bals_available.value( currency_id, 0 ) >= _internal_reserve.value( currency_id, 0 ) ? _bals_available.value(currency_id,0) - _internal_reserve.value(currency_id,0) : 0 : 0;
        return result;
    });
    session->provide( "swiftbot.balances.get.total", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        const double totl = _bals_total.value( currency_id, 0 );
        return totl;
    } );



    session->provide( "swiftbot.balances.get.requested.stats", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        static QString cached_res("");
        const QStringList exchanges_using( SwiftLib::getSettings()->value("EXCHANGES").toString().split(",") );
        static qint64 last_ts(0);
        if ( QDateTime::currentSecsSinceEpoch() - last_ts >= 30 ) {
            QSqlQuery q("SELECT currency_id, min(requested) as min, max(requested) as max, avg(requested) as avg FROM currency_requested LEFT JOIN currencies c ON c.id=currency_requested.currency_id LEFT JOIN exchanges e ON e.id=c.exchange_id WHERE e.is_enabled=1 AND c.is_enabled=1 AND date(ts)=curdate() group by currency_id");
            QJsonArray j_ret;
            if ( q.exec() ) {
                while( q.next() ) {
                    QJsonObject j_it;
                    const quint32 cid = q.value("currency_id").toUInt();
                    const quint32 coinid =  assets->getCurrencyCoin( cid );
                    const QString ename( assets->getExchangeName( assets->getCurrencyExchangeId( cid ) ) );
                    j_it["currency_id"] = QString::number( cid );
                    j_it["currency_name"] = assets->getCoinName( coinid );
                    j_it["exchange_id"] = QString::number(assets->getCurrencyExchangeId( cid ) );
                    j_it["exchange_name"] = ename;
                    j_it["coin_id"] = QString::number( coinid );
                    double usdam = 0;
                    if ( coinid == 1 ) {
                        usdam = __cached_rates.value(1) * q.value("avg").toDouble();
                    } else if ( coinid == 2 ) {
                        usdam = __cached_rates.value(2) * q.value("avg").toDouble();
                    } else {
                        usdam = q.value("avg").toDouble();
                    }
                    j_it["avg_usd"] = QString::number( usdam, 'f', 8 );
                    j_it["min"] = QString::number( q.value("min").toDouble(), 'f', 4 );
                    j_it["max"] = QString::number( q.value("max").toDouble(), 'f', 4 );
                    j_it["avg"] = QString::number( q.value("avg").toDouble(), 'f', 4 );
                    if ( exchanges_using.contains( ename ) ) {
                        j_ret.push_back( j_it );
                    }
                }
            }
            q.finish();
            cached_res =  QJsonDocument( j_ret ).toJson( QJsonDocument::Compact );
            last_ts = QDateTime::currentSecsSinceEpoch();
        }
        return cached_res;
    } );

    session->provide( "swiftbot.balances.get.coin.total", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        if ( !v.isEmpty() ) {
            const quint32 coin_id = v.at(0).toUInt();
            const double totl = _coin_total.value( coin_id, 0 );
            return totl;
        } else {
            return 0.0;
        }
    } );
    session->provide( "swiftbot.balances.get.orders", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        return _bals_orders.value( currency_id, 0 );
    });
    session->provide( "swiftbot.balances.get.usd.orders", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const double ret = __in_orders_usd;
        return ret;
    });
    session->provide( "swiftbot.balances.get.btc.orders", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const double ret = __in_orders_btc;
        return ret;
    });
    session->provide( "swiftbot.balances.set.requested", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        const double amount = v.at(1).toDouble();
        _bals_requested[currency_id] =amount;
        if ( assets->getCurrencyName( currency_id ) != "???" ) {
        QJsonObject j_itm(_mapped.value( currency_id ) );
        if ( j_itm.isEmpty() ) {
            j_itm["currency_id"] = QString::number( currency_id );
            j_itm["exchange_id"] = QString::number( assets->getCurrencyExchangeId( currency_id ) );
            j_itm["name"] = assets->getCurrencyName( currency_id );
            j_itm["coin_id"] = QString::number( assets->getCurrencyCoin( currency_id ) );
            j_itm["available"] = QString::number( 0 );
            j_itm["required"] = QString::number( 0 );
            j_itm["total"] = QString::number( 0 );
            _mapped[currency_id] = j_itm;

        }

        _mapped[currency_id]["required"] = QString::number( amount, 'f', 8 );
        }
        return 1;
    });
    session->provide( "swiftbot.balances.add.requested", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        const double amount = v.at(1).toDouble();
        _bals_requested[currency_id]+=amount;
        return 1;
    });
    session->provide( "swiftbot.balances.get.requested", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        return _bals_requested.value(currency_id,0);
    });


    session->provide( "swiftbot.balances.get.reserved", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        return _bals_reserved.value( currency_id, 0 );
    });


    session->provide( "swiftbot.balances.reserve.create", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        const double amount = v.at(1).toDouble();
        const bool reserveAmount = reserveInternal( currency_id, amount );
        return reserveAmount ? 1 : 0;
    });

    session->provide( "swiftbot.balances.reserve.cancel", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 currency_id = v.at(0).toUInt();
        const double amount = v.at(1).toDouble();
        const bool reserveAmount = reserveCancelInternal( currency_id, amount );
        return reserveAmount ? 1 : 0;
    });
    session->provide( "swiftbot.balances.reserve.cancel.all", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        _internal_reserve.clear();
        return 1;
    });
    session->provide( "swiftbot.balances.get.rate.btc.usd", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const double rate = _last_btc_usd_rate;
        return rate;
    });
    session->provide( "swiftbot.balances.get.rates", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v);
        QJsonArray _ret( getRatesDiff() );

        const QString r( QJsonDocument( _ret ).toJson( QJsonDocument::Compact ) );
        return r;
    });


    session->provide( "swiftbot.balances.get.tradestats", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const QJsonObject j_ret( getTradingSummary(v) );
        const QString r( QJsonDocument( j_ret ).toJson( QJsonDocument::Compact ) );
        return r;
    });

    session->provide( "swiftbot.balances.get.todayprofit", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const double r = last_profit_value;
        return r;
    });

    session->provide( "swiftbot.balances.get.yesterdayrate", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const double cr = __cached_rates.value( 1, 1 );
        const double pr = last_rate_diff_value.value( 1, 1);
        const double r = cr - pr;
        return r;
    });

    session->provide( "swiftbot.balances.get.history", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        QSqlQuery q("SELECT avg(profit) as profit, avg(byrate) as byrate, avg(total) as total, date(ts) as tss FROM `balance_history` where date(ts) > SUBDATE(curdate(),INTERVAL 15 DAY) GROUP BY date(ts)");
        QJsonArray j_records;
        if ( q.exec() ) {
            while ( q.next() ) {
                QJsonObject jit;
                jit["date"] = q.value("tss").toDate().toString( Qt::ISODate );
                jit["profit"] = QString::number( q.value("profit").toDouble(), 'f', 8 );
                jit["byrate"] = QString::number( q.value("byrate").toDouble(), 'f', 8 );
                jit["total"] = QString::number( q.value("total").toDouble(), 'f', 8 );
                j_records.push_back( jit );
            }
        } else {
            qWarning() << q.lastError().text();
        }
        const QString ret( QJsonDocument( j_records ).toJson( QJsonDocument::Compact ) );
        return ret;
    });





    session->provide( "swiftbot.balances.get.addresses", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QString ret( QJsonDocument( j_addresses ).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->provide("set.config.newaddress", [&](const QVariantList&v, const QVariantMap&m ){
        Q_UNUSED(m)
        if ( !v.isEmpty() ) {
            const quint32 exch = v.at(0).toString().toUInt();
            const quint32 coin = v.at(1).toString().toUInt();
            const QString addr = v.at(2).toString();
            insertAddress( exch, coin, addr );
        }
        return QString("");
    });

    session->provide( "swiftbot.balances.current.get.address", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const QString address( v.at(0).toString() );
        const QString ret( QJsonDocument( assets->getAddressInfo( address ) ).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->subscribe("swiftbot.events.settings.valuechanged", [&](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        if ( v.size() == 2 ) {
            SwiftLib::getSettings()->setValue( v.at(0).toString(), v.at(1).toString() );
        }
    });

    session->provide( "swiftbot.balances.get.history.profit", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        QSqlQuery q;
        QHash<QString, double> _res;
        if ( !q.exec("SELECT TRUNCATE(amount,4) as am, TRUNCATE(sum(if(type=0,TRUNCATE(-amount,4),TRUNCATE(amount,4))),4) as a, sum(if(type=0,price,-price)) as s, date(ts) as tts FROM `"+dbname+"`.`orders` where status=2 group by TRUNCATE(amount,4), date(ts)") ) {
            qWarning() << "Get user profit error: " << q.lastError().text();
        } else {
            while ( q.next() ) {
                 if ( q.value("a").toDouble() == 0 ) {
                     _res[ q.value("tts").toString() ] += q.value("s").toDouble();
                 }
            }
        }
        QJsonArray j_arr;
        for( auto it = _res.begin(); it != _res.end(); it++ ) {
            QJsonObject j_obj;
            j_obj["date"] = it.key();
            j_obj["profit"] = QString::number( it.value(), 'f', 8 );
            j_arr.push_back( j_obj );
        }
        q.finish();
        const QString ret( QJsonDocument( j_arr ).toJson( QJsonDocument::Compact ) );
        return ret;
    });



    session->provide( "swiftbot.balances.current.withdraws", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
      QJsonArray j_items;
      QSqlQuery q;

      if ( q.exec("SELECT * FROM `"+dbname+"`.`withdraws` WHERE date(created_at)=curdate() ORDER BY created_at DESC") ) {
          while( q.next() ) {
            QJsonObject j_itm;
            j_itm["id"] = QString::number( q.value("id").toUInt() );
            j_itm["type"] = "withdraw";
            j_itm["currency_id"] = QString::number( q.value("currency_id").toUInt() );
            j_itm["exchange_id"] = QString::number( assets->getCurrencyExchangeId( q.value("currency_id").toUInt() ) );
            j_itm["amount"] = QString::number( q.value("amount").toDouble(),'f',8 );
            j_itm["currency_name"] = assets->getCurrencyName(q.value("currency_id").toUInt()).replace("xbt","BTC");
            j_itm["exchange_name"] = assets->getCurrencyExchangeName( q.value("currency_id").toUInt() );
            j_itm["created_at"] = QString::number( q.value("created_at").toDateTime().toSecsSinceEpoch() );
            j_itm["status"] = q.value("status").toString();
            j_itm["address"] = q.value("address").toString();
            j_items.push_back( j_itm );
          }
      }
      q.finish();
      const QJsonArray _d( j_items );
      const QString str( QJsonDocument( _d ).toJson( QJsonDocument::Compact ) );
      return str;
    });

    session->provide( "swiftbot.balances.current.withdraws.fees", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
      QSqlQuery q;
      double ttl = 0;
      if ( q.exec("SELECT sum(w.fee) as sm, c.coin_id FROM `"+dbname+"`.`withdraws` w left join currencies c on c.id=w.currency_id WHERE date(w.created_at)=curdate() group by c.coin_id") ) {
          while( q.next() ) {
             ttl += q.value("sm").toDouble() * __cached_rates.value(  q.value("coin_id").toUInt(), 1 );
          }
      }
      q.finish();
      const QString str( QString::number( ttl, 'f', 8 ));
      return str;
    });

    session->provide("swiftbot.balances.transactions.withdraw.get", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        QJsonArray j_items;
        QString ret("");
        if ( !v.isEmpty() ) {
            const quint32 trid=v.at(0).toUInt();
            QSqlQuery q;
            if ( q.exec("SELECT * FROM `"+dbname+"`.`withdraws` WHERE id="+QString::number( trid ) ) ) {
                if( q.next() ) {
                  QJsonObject j_itm;
                  j_itm["id"] = QString::number( q.value("id").toUInt() );
                  j_itm["type"] = "withdraw";
                  j_itm["currency_id"] = QString::number( q.value("currency_id").toUInt() );
                  j_itm["exchange_id"] = QString::number( assets->getCurrencyExchangeId( q.value("currency_id").toUInt() ) );
                  j_itm["currency_name"] = assets->getCurrencyName(q.value("currency_id").toUInt()).replace("xbt","BTC");
                  j_itm["exchange_name"] = assets->getCurrencyExchangeName( q.value("currency_id").toUInt() );
                  j_itm["amount"] = QString::number( q.value("amount").toDouble(),'f',8 );
                  j_itm["created_at"] = QString::number( q.value("created_at").toDateTime().toSecsSinceEpoch() );
                  j_itm["status"] = q.value("status").toString();
                  j_itm["address"] = q.value("address").toString();
                  ret = QJsonDocument(j_itm ).toJson( QJsonDocument::Compact );

                }
            }
            q.finish();
        }
        return ret;
    });
    session->provide( "swiftbot.balances.transactions.deposit.get", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        QJsonArray j_items;
        if ( !v.isEmpty() ) {
            const quint32 trid=v.at(0).toUInt();
            QSqlQuery q;
            if ( q.exec("SELECT * FROM `"+dbname+"`.`deposits` WHERE id="+QString::number( trid ) ) ) {
                if( q.next() ) {
                  QJsonObject j_itm;
                  j_itm["id"] = QString::number( q.value("id").toUInt() );
                  j_itm["type"] = "deposit";
                  j_itm["tx"] = q.value("txhash").toString();
                  j_itm["currency_id"] = QString::number( q.value("currency_id").toUInt() );
                  j_itm["exchange_id"] = QString::number( assets->getCurrencyExchangeId( q.value("currency_id").toUInt() ) );
                  j_itm["currency_name"] = assets->getCurrencyName(q.value("currency_id").toUInt()).replace("xbt","BTC");
                  j_itm["exchange_name"] = assets->getCurrencyExchangeName( q.value("currency_id").toUInt() );
                  j_itm["amount"] = QString::number( q.value("amount").toDouble(),'f',8 );
                  j_itm["created_at"] = QString::number( q.value("created_at").toDateTime().toSecsSinceEpoch() );
                  j_itm["status"] = q.value("status").toString();
                  j_itm["address"] = q.value("address").toString();
                  const QString ret( QJsonDocument(j_itm ).toJson( QJsonDocument::Compact ) );
                  return ret;
                }
            }
            q.finish();

        }
        return QString("{}");
    });


    session->provide( "swiftbot.balances.current.deposits", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        QJsonArray j_items;
        QSqlQuery q;
        if ( q.exec("SELECT * FROM `"+dbname+"`.`deposits` WHERE date(created_at)=curdate() ORDER BY created_at DESC") ) {
            while( q.next() ) {
              QJsonObject j_itm;
              j_itm["id"] = QString::number( q.value("id").toUInt() );
              j_itm["type"] = "deposit";
              j_itm["tx"] = q.value("txhash").toString();
              j_itm["currency_id"] = QString::number( q.value("currency_id").toUInt() );
              j_itm["exchange_id"] = QString::number( assets->getCurrencyExchangeId( q.value("currency_id").toUInt() ) );
              j_itm["currency_name"] = assets->getCurrencyName(q.value("currency_id").toUInt()).replace("xbt","BTC");
              j_itm["exchange_name"] = assets->getCurrencyExchangeName( q.value("currency_id").toUInt() );
              j_itm["amount"] = QString::number( q.value("amount").toDouble(),'f',8 );
              j_itm["created_at"] = QString::number( q.value("created_at").toDateTime().toSecsSinceEpoch() );
              j_itm["status"] = q.value("status").toString();
              j_itm["address"] = q.value("address").toString();
              j_items.push_back( j_itm );
            }
        }
        const QJsonArray _d( j_items );
        const QString str( QJsonDocument( _d ).toJson( QJsonDocument::Compact ) );
        q.finish();
        return str;

    });

    session->provide( "swiftbot.balances.current.deposits.all", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QJsonArray _d( getDepositsHistory() );
        const QString str( QJsonDocument( _d ).toJson( QJsonDocument::Compact ) );
        return str;
    });

    session->provide( "swiftbot.balances.current.withdraws.all", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QJsonArray _d( getWithdrawHistory() );
        const QString str( QJsonDocument( _d ).toJson( QJsonDocument::Compact ) );
        return str;
    });

    session->provide( "swiftbot.balances.set.status.profit", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const double profit = v.at(0).toDouble();
        _current_profit = profit;
        return true;
    });

    session->provide( "swiftbot.balances.set.status.orders", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const quint32 ocnt = v.at(0).toUInt();
        _current_orders = ocnt;
        return true;
    });

    session->provide( "swiftbot.balances.transfer", [&]( const QVariantList&v, const QVariantMap&m ){
        Q_UNUSED(m)
        const QJsonObject j_params( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        const quint32 from_exchange = j_params.value("from").toString().toUInt();
        const quint32 to_exchange = j_params.value("to").toString().toUInt();
        const double amount = j_params.value("amount").toString().toDouble();
        const quint32 coin_id = j_params.value("coin").toString().toUInt();
        const quint32 from_currency_id = assets->getTokenCurrencyIdByCoinExchange( coin_id, from_exchange );
        const quint32 to_currency_id = assets->getTokenCurrencyIdByCoinExchange( coin_id, to_exchange );
        QString tokentype = "BTC";
        if ( coin_id == 2 || coin_id == 7 ) {
            tokentype = "ERC20";
        } else if ( coin_id == 10 ) {
            tokentype = "XRP";
        }
        const QString target_address = assets->getDepositAddress( to_currency_id, tokentype );
        QJsonObject j_new_params;
        j_new_params["address"] = target_address;
        j_new_params["currency_id"] = QString::number( from_currency_id );
        j_new_params["amount"] = QString::number( amount, 'f', 8 );
        j_new_params["local_id"] = QString::number( amount, 'f', 8 )+QString::number( QDateTime::currentSecsSinceEpoch() );
        const QString ret( QJsonDocument( j_new_params ).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->provide( "swiftbot.balances.group.get", [&]( const QVariantList&v, const QVariantMap&m ) {
        QJsonObject j_ret;
        const QStringList exchs( assets->getExchangesNames() );
        for( auto it = exchs.begin(); it != exchs.end(); it++ ) {
            const QString exchange_name ( *it );
            const QList<quint32> curs( assets->getAllExchangeCurrencies( exchange_name.toLower() ) );
            QJsonObject j_exch_item;
            for( auto cit = curs.begin(); cit != curs.end(); cit++ ) {
                QJsonObject jc;
                const quint32 cid( *cit );
                jc["exchange_name"] = exchange_name;
                jc["exchange_id"] = QString::number( assets->getCurrencyExchangeId( *cit ) );
                jc["currency_id"] = QString::number( *cit );
                jc["coin_id"] = QString::number( assets->getCurrencyCoin( *cit ) );
                jc["total"]  = QString::number( _bals_total.value( cid, 0 ), 'f', 8 );
                jc["available"]  = QString::number( _bals_available.value( cid, 0 ), 'f', 8 );
                jc["requested"]  = QString::number( _bals_requested.value( cid, 0 ), 'f', 8 );
                jc["reserved"]  = QString::number( _bals_reserved.value( cid, 0 ), 'f', 8 );
                jc["orders"]  = QString::number( _bals_orders.value( cid, 0 ), 'f', 8 );
                jc["withdraws"] = QString::number( _active_withdraws.value( cid, 0 ), 'f', 8 );
                jc["deposits"] = QString::number( _active_deposits.value( cid, 0 ), 'f', 8 );
                j_exch_item[ QString::number( cid ) ] = jc;
            }
            j_ret[exchange_name] = j_exch_item;
        }

        Q_UNUSED(m)
        Q_UNUSED(v)
        const QString res( QJsonDocument( j_ret ).toJson( QJsonDocument::Compact ) );
        return res;
    });
//
 //   summaries_timer->start();
    connect( queue_timer, &QTimer::timeout, this, &BalancesWorker::processQueue );
    queue_timer->start();
    processQueue();
    QTimer::singleShot(2000, this, &BalancesWorker::getWithdrawStatuses );
    QTimer::singleShot(5000, this, &BalancesWorker::getDepositsStatuses );
    QTimer::singleShot( 15000, this, &BalancesWorker::updateRatesCache );
    QTimer::singleShot( 35000, this, &BalancesWorker::saveBalanceHistory );
    QTimer::singleShot( 1250, this, &BalancesWorker::getTodayRateDiff );
    QTimer::singleShot( 2500, this, &BalancesWorker::getTodayTradeProfit );
    QTimer::singleShot( 3000, this, &BalancesWorker::loadAddresses );
}

void BalancesWorker::onBalance(const QJsonObject &res) {
    const quint64 uuid = res.value("async_uuid").toString().toULongLong();
    if ( res.value("success").toBool() == true ) {
         const QJsonArray r( res.value("balances").toArray() );
         for( auto it = r.begin(); it != r.end(); it++ ) {
             _mapped[ it->toObject().value("currency_id").toString().toUInt() ] =  it->toObject();
         }
    } else {
        // PUBLISH RPC ERROR HERE!
        if ( res.value("error").toString() == "Method not implemented" ) {
             const quint64 uuid = res.value("async_uuid").toString().toULongLong();
             if ( _waiters_exchs.contains(uuid) ) {

                if ( !_broken_exchanges.contains( _waiters_exchs.value( uuid ) ) ) {
                     _broken_exchanges.push_back( _waiters_exchs.value( uuid ) );
                 }
             }

        } else {
            if ( _waiters_exchs.value( uuid ) > 0  ) {
           _errs_counter[ _waiters_exchs.value( uuid ) ]++;
           QString str;
           for ( auto ii = _errs_counter.begin(); ii != _errs_counter.end();  ii++ ) {
               str += QString::number( ii.key() ) + " = " + QString::number( ii.value() )+ " ";
           }
            addError( "RPC ERRS COUNTER: " + str );
            }
        }
    }
}

void BalancesWorker::calculateSummaries() {
    _total_usd = 0;
    _in_orders_usd = 0;
    _reserved_usd = 0;
    _available_usd = 0;
    _total_btc= 0;
    _in_orders_btc = 0;
    _reserved_btc = 0;
    _available_btc = 0;

    if ( !_mapped.isEmpty() ) {
        QJsonArray _to_cache;
        const QHash<quint32, QJsonObject> m( _mapped );


        for( auto it = m.begin(); it != m.end(); it++ ) {
            const QJsonObject ob( it.value() );
            _to_cache.push_back( ob );
            const quint32 currency_id( ob.value("currency_id").toString().toUInt() );

            const double in_orders = ob.value("in_orders").toString() != "NaN" ? ob.value("in_orders").toString("0").toDouble() : 0;
            const double total = ob.value("total").toString().toDouble();
            const double reserved = ob.value("reserved").toString().toDouble();
            const double available = ob.value("available").toString().toDouble();

            _bals_total[ currency_id ] = total;
            _bals_available[currency_id] = available;
            _bals_reserved[currency_id] = reserved;
            _bals_orders[currency_id] = in_orders;

            const quint32 coinid = assets->getCurrencyCoin( currency_id );
            if ( !_coin_total.contains( coinid ) ) {
                _coin_total[ coinid ] = 0;
            }
            _coin_total[ coinid ] += total;
            if ( coinid == 7 ) {
                _total_usd += total;
                _in_orders_usd += in_orders;
                _reserved_usd += reserved;
                _available_usd += available;
            } else if ( coinid == 1 ) {
                _total_btc += total;
                _in_orders_btc += in_orders;
                _reserved_btc += reserved;
                _available_btc += available;
            }
        }

        __total_usd = _total_usd;
        __in_orders_usd = _in_orders_usd;
        __reserved_usd = _reserved_usd;
        __available_usd = _available_usd;
        __total_btc= _total_btc;
        __in_orders_btc = _in_orders_btc;
        __reserved_btc = _reserved_btc;
        __available_btc = _available_btc;
    }
}

void BalancesWorker::calculateBalancerTransfers() {
    return;
}

void BalancesWorker::loadAddresses() {
    if ( !j_addresses_insert.isEmpty() ) {
        for( auto it = j_addresses_insert.begin(); it != j_addresses_insert.end(); it++ ) {
            QSqlQuery q("INSERT INTO `"+dbname+"`.`currency_addresses` (currency_id,address,token_type,tag) VALUES ('"+it->toObject().value("currency_id").toString()+"','"+it->toObject().value("address").toString()+"','"+it->toObject().value("token").toString()+"',NULL)");
            if ( !q.exec() ) {
                qWarning() << q.lastError().text();
            }
        }
        j_addresses_insert = QJsonArray();
    }
    QSqlQuery q("SELECT *, c.exchange_id FROM `"+dbname+"`.`currency_addresses` a left join currencies c ON c.id=a.currency_id");
    if ( q.exec() ) {
        j_addresses = QJsonArray();
        while ( q.next() ) {
            QJsonObject j;
            j["currency_id"] = QString::number( q.value("currency_id").toUInt() );
            j["coin_id"] = QString::number( assets->getCurrencyCoin( q.value("currency_id").toUInt() ) );
            j["exchange_id"] = QString::number( q.value("exchange_id").toUInt() );
            j["exchange_name"] = SwiftLib::getAssets()->getExchangeName( q.value("exchange_id").toUInt() );
            j["address"] = q.value("address").toString();
            j["token"] = q.value("token_type").toString();
            j["tag"] = q.value("tag").toString();
            j_addresses.push_back( j );
        }
    }
    QTimer::singleShot( 360000, this, &BalancesWorker::loadAddresses );
}

void BalancesWorker::getRate() {
    return;
}

void BalancesWorker::getExternalBtcBalance() {

    if ( SwiftLib::getSettings()->value("EXTERNAL_BTC_WALLET","").toString() != "" ) {
        const QString extwallet = SwiftLib::getSettings()->value("EXTERNAL_BTC_WALLET","").toString();
        QEventLoop loop;
        QNetworkReply * reply = netman->get( QNetworkRequest( QUrl( "https://api.blockchair.com/bitcoin/dashboards/address/"+extwallet) ) );
        connect(reply, &QNetworkReply::finished, [&reply, &loop, this, &extwallet]() {
            const QByteArray data( reply->readAll() );
            const QJsonObject res( QJsonDocument::fromJson( data ).object().value("data").toObject().value( extwallet ).toObject().value("address").toObject() );
            _ext_wallet_balance = res.value("balance").toDouble() / 10000000;
            loop.quit();
        });
        loop.exec();
        reply->deleteLater();
        QTimer::singleShot(10000, this, &BalancesWorker::getExternalBtcBalance );
    }

}

void BalancesWorker::updateRatesCache() {
    if ( session != nullptr && session->isJoined() ) {
        const QHash<quint32, QList<quint32> > _apairs( assets->getArbitragePairs() );
        for( auto it = _apairs.begin(); it != _apairs.end(); it++ ) {
            const quint32 apid = it.key();
            QList<double> _prates;
            if ( !it.value().isEmpty() ) {
                const QList<quint32> _pairs( it.value() );
                for( auto itt = _pairs.begin(); itt != _pairs.end(); itt++ ) {
                    const QString d( session->call("swiftbot.orderbooks.cache", {*itt} ).toString() );
                    const QJsonArray _asks( QJsonDocument::fromJson( d.toUtf8() ).object().value("a").toArray() );
                    if ( !_asks.isEmpty() ) {
                        _last_pairs_rates[ *itt ] = _asks.at(0).toArray().at(1).toString().toDouble();
                        _prates.push_back( _asks.at(0).toArray().at(1).toString().toDouble() );
                    }
                }
            }
            if ( !_prates.isEmpty() ) {
                __cached_rates[apid] = std::accumulate( _prates.begin(), _prates.end(), 0) / _prates.count();
            }
        }
    }
    QTimer::singleShot( 60000, this, &BalancesWorker::updateRatesCache );
}

double BalancesWorker::getSellRate(const quint32 &pair_id) {
    const QJsonArray a( getBids() );
    double price =0;
    for( auto it = a.begin(); it!= a.end(); it++ ) {
        if ( it->toArray().at(0).toString().toUInt() == pair_id ) {
            if ( price == 0 || price > it->toArray().at(1).toString().toDouble() ) {
                price = it->toArray().at(1).toString().toDouble();
            }
        }
    }
    return price;
}

void BalancesWorker::getTodayRateDiff()
{

        QSqlQuery q("SELECT AVG(`rate`) as rate, pair_id FROM `rates` where pair_id IN (11,12) AND date(ts) >= SUBDATE(curdate(),INTERVAL 1 DAY) AND ts<now() GROUP BY pair_id");
        if ( q.exec() ) {
            while ( q.next() ) {
                last_rate_diff_value[ q.value("pair_id").toUInt() == 11 ? 1 : 2 ] = q.value("rate").toDouble();
            }
        }
        QTimer::singleShot( 360000, this, &BalancesWorker::getTodayRateDiff );
}




void BalancesWorker::getTodayTradeProfit() {
    QSqlQuery q("SELECT * FROM orders where status<=2 AND arb_window>0 and date(ts)=curdate()");
    if ( q.exec( ) ) {
        double sell_price = 0;
        double buy_price = 0;
        while ( q.next() ) {
            if ( q.value("type").toUInt() == 0 ) {
                sell_price += q.value("price").toDouble();
            } else  {
                buy_price += q.value("price").toDouble();
            }
        }
        last_profit_value = sell_price - buy_price;
        q.finish();
    } else {
        qWarning() << q.lastError().text();
    }
    QTimer::singleShot( 35000, this, &BalancesWorker::getTodayTradeProfit );
}

void BalancesWorker::insertAddress(const quint32 &exch, const quint32 &coin, const QString &address) {
    const quint32 currency_id = assets->getTokenCurrencyIdByCoinExchange( coin, exch );
    QString chain( coin == 1 ? "btc" : "erc20" );
    QJsonObject j;
    j["currency_id"] = QString::number( currency_id );
    j["coin_id"] = QString::number( coin );
    j["exchange_id"] = QString::number(exch );
    j["exchange_name"] = SwiftLib::getAssets()->getExchangeName( exch );
    j["address"] = address;
    j["token"] = chain;
    j["tag"] = "";
    j_addresses.push_back( j );
    j_addresses_insert.push_back( j );
}

void BalancesWorker::saveBalanceHistory() {
    const QJsonArray jb( getBalances() );
    QSqlQuery cq( _ch );
    QString cqs("INSERT INTO `balances` (d,ts,currency,coin,balance,usd) VALUES ");
    QStringList _parts;

    // CREATE TABLE `balances` (`d` Date, `ts` DateTime('Europe/Moscow'),`currency` Int32,`coin` Int32,`balance` Decimal128(8),`usd` Decimal128(8)) ENGINE = MergeTree(d, (currency, balance), 8192);
    QMap<quint32, double> _coin_aggr;
    double totlusd = 0;
    for( auto it = jb.begin(); it != jb.end(); it++ ) {
        const quint32 coid( it->toObject().value("coin_id").toString().toUInt() );
        const double bal = it->toObject().value("total").toString().toDouble();
        const double usd =  coid == 7 ? bal : coid == 1 ? bal * __cached_rates.value(1) : bal * __cached_rates.value(2);
        totlusd += usd;
        if ( bal > 0 ) {
            _parts.push_back("(toDate(now()),now(),"+QString::number( it->toObject().value("currency_id").toString().toUInt( ) )+","+QString::number(  coid )+","+QString::number( bal,'f',8)+","+QString::number( usd, 'f', 8)+")");
        }
        _coin_aggr[ coid ] += bal;
    }
    cqs += _parts.join(",");
    if ( !cq.exec( cqs ) ) {
        qWarning() << cq.lastError().text();
    }
    const double trade_profit = last_profit_value;
    double rate_changes = 0;
    for( auto it = _coin_aggr.begin(); it != _coin_aggr.end(); it++ ) {
        if ( it.key() != 7 ) {
            rate_changes += it.value() * ( __cached_rates.value( it.key() ) - last_rate_diff_value.value( 1, 1 ) );
        }
    }
    const double by_rate_changes = rate_changes;
    QSqlQuery q;
    q.prepare("INSERT INTO `balance_history` (`profit`,`byrate`,`total`) VALUES (:profit,:byrate,:total)");
    q.bindValue(":total", totlusd);
    q.bindValue(":profit", trade_profit);
    q.bindValue(":byrate", by_rate_changes);
    if ( !q.exec() ) {
        qWarning() << q.lastError().text() << "Error saving balances history";
    }
    q.finish();
    QTimer::singleShot( 180000, this, &BalancesWorker::saveBalanceHistory );
}



double BalancesWorker::getBuyRate(const quint32 &pair_id) {
    const QJsonArray a( getAsks() );
    double price =0;
    for( auto it = a.begin(); it!= a.end(); it++ ) {
        if ( it->toArray().at(0).toArray().at(0).toString().toUInt() == pair_id ) {
            if ( price == 0 || price < it->toArray().at(0).toArray().at(1).toString().toDouble() ) {
                price =  it->toArray().at(0).toArray().at(1).toString().toDouble();
            }
        }
    }
    return price;
}

QJsonArray BalancesWorker::getAsks() const {
    return asks;
}

QJsonArray BalancesWorker::getBids() const {
    return bids;
}

void BalancesWorker::saveReport() {

    calculateSummaries();

}

void BalancesWorker::addLog(const QString &message) {
    QJsonObject j_msg;
    j_msg["group"] = "worker";
    j_msg["message"] = message;
    j_msg["sender_name"] = "balances-worker";
    j_msg["ts"] = QString::number( QDateTime::currentMSecsSinceEpoch() );
    const QString r( QJsonDocument( j_msg ).toJson( QJsonDocument::Compact ) );
    if ( session != nullptr && session->isJoined() ) {
        session->publish("swiftbot.system.logs", {r});
    }
}

void BalancesWorker::addError(const QString &message) {
    QJsonObject j_msg;
    j_msg["group"] = "worker";
    j_msg["message"] = message;
    j_msg["sender_name"] = "balances-worker";
    j_msg["ts"] = QString::number( QDateTime::currentMSecsSinceEpoch() );
    const QString r( QJsonDocument( j_msg ).toJson( QJsonDocument::Compact ) );
    if (  session != nullptr && session->isJoined() ) {
        session->publish("swiftbot.system.errors", {r});
    }
}


bool BalancesWorker::hasDepositAddress(const quint32 &currency_id, const QString &token_type) const {
    const QString addr = assets->getDepositAddress( currency_id, token_type );
    return addr != "#";
}

quint32 BalancesWorker::getWithdrawCurrencyId(const quint32 &exchange_id, const quint32 &coin_id) const {
    if ( coin_id == 1 ) {
        return assets->getCurrencyIdByName("BTC", exchange_id );
    } else if ( coin_id == 7 ) {
        return assets->getCurrencyIdByName("USDT", exchange_id );
    } else {
        return 0;
    }
}

quint32 BalancesWorker::getDepositCurrencyId(const quint32 &exchange_id, const quint32 &coin_id) const {
    if ( coin_id == 1 ) {
        return assets->getCurrencyIdByName("BTC", exchange_id );
    } else if ( coin_id == 7 ) {
        return assets->getCurrencyIdByName("USDT", exchange_id );
    } else {
        return 0;
    }
}


