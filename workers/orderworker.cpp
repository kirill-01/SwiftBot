#include "orderworker.h"


OrderWorker::OrderWorker(QObject *parent) : SwiftWorker( parent ), o_storage(new OrdersStorage(this) ), assets( SwiftLib::getAssets() ) {
    connect( this, &OrderWorker::errorOrderEvent, this, &OrderWorker::onErrorOrderEvent, Qt::QueuedConnection );
    connect( this, &OrderWorker::updatedOrderEvent, this, &OrderWorker::onUpdatedOrderEvent, Qt::QueuedConnection );
    connect( this, &OrderWorker::completedOrderEvent, this, &OrderWorker::onCompletedOrderEvent, Qt::QueuedConnection );
    connect( this, &OrderWorker::placedOrderEvent, this, &OrderWorker::onPlacedOrderEvent, Qt::QueuedConnection );
    connect( this, &OrderWorker::canceledOrderEvent, this, &OrderWorker::onCanceledOrderEvent, Qt::QueuedConnection );
    connect( this, &OrderWorker::historyOrderEvent, this, &OrderWorker::onHistoryOrderEvent, Qt::QueuedConnection );
    connect( this, &OrderWorker::activeOrderEvent, this, &OrderWorker::onActiveOrderEvent, Qt::QueuedConnection );
}

QStringList OrderWorker::listenParams()
{
    return QStringList();
}

void OrderWorker::onParamChanged(const QString &param, const QString &value)
{
    Q_UNUSED( param )
    Q_UNUSED( value )
}

void OrderWorker::initWorker(Wamp::Session *sess)
{
    sess->subscribe( "swift.orders.feed.events", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(m)
        const QString event_name( v.at(0).toString() );
        if ( event_name == "PLACED" ) {
            emit placedOrderEvent( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "CANCELED" ) {
            emit canceledOrderEvent( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "COMPLETED" ) {
            emit completedOrderEvent( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "ERROR" ) {
            emit errorOrderEvent( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "ACTIVE" ) {
            emit activeOrderEvent( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "HISTORY" ) {
            emit historyOrderEvent( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        }
    });

    sess->provide("swift.trade.rpc.active",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m);
        QJsonArray j_ret;
        if ( !v.isEmpty() ) {
            j_ret = o_storage->getOopenOrdersByPair( v.at(0).toUInt() );
        } else {
            j_ret = o_storage->getOpenOrders();
        }
        const QString r( QJsonDocument( j_ret ).toJson( QJsonDocument::Compact ) );
        return r;
    });
    sess->provide("swift.trade.rpc.history",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m);
        QJsonArray j_ret;
        if ( !v.isEmpty() && v.size() == 1 ) {
            j_ret = o_storage->getClosedOrdersByPair( v.at(0).toUInt() );
        } else if ( !v.isEmpty() && v.size() == 2 ) {
            j_ret = o_storage->getClosedOrdersByPair( v.at(0).toUInt(), v.at(1).toString() );
        } else {
            j_ret = o_storage->getClosedOrders();
        }
        const QString r( QJsonDocument( j_ret ).toJson( QJsonDocument::Compact ) );
        return r;
    });
    sess->provide("swift.trade.rpc.today",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m);
        QJsonObject _itms;
        _itms["open"] =  o_storage->getOpenOrders();
        _itms["closed"] = o_storage->getClosedOrders();
        Q_UNUSED(v)
        Q_UNUSED(m)
        const QString ret( QJsonDocument(_itms).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    sess->provide("swift.trade.rpc.summary",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        QJsonObject ret( getTradingSummary( v ) );
        const QString r( QJsonDocument( ret ).toJson(QJsonDocument::Compact ));
        return r;
    });

    QTimer::singleShot( 1000, this, &OrderWorker::requestActives );
    QTimer::singleShot( 7000, this, &OrderWorker::requestHistory );
    QTimer::singleShot( 5000, this, &OrderWorker::checkUpdates );
}

QString OrderWorker::getWorkerName() const
{
    return "order";
}

void OrderWorker::onErrorOrderEvent(const QJsonObject &j_itm)
{
    //o_storage->addOrder( j_itm.value("items").toArray().first().toObject() );
}

void OrderWorker::onUpdatedOrderEvent(const QJsonObject &j_itm)
{
    o_storage->addOrder( j_itm.value("items").toArray().first().toObject() );
}

void OrderWorker::onCompletedOrderEvent(const QJsonObject &j_itm)
{
    o_storage->addOrder( j_itm.value("items").toArray().first().toObject() );
}

void OrderWorker::onPlacedOrderEvent(const QJsonObject &j_itm)
{
    o_storage->addOrder( j_itm.value("items").toArray().first().toObject() );
}

void OrderWorker::onCanceledOrderEvent(const QJsonObject &j_itm)
{
    o_storage->addOrder( j_itm.value("items").toArray().first().toObject() );
}

void OrderWorker::onHistoryOrderEvent(const QJsonObject &j_itm)
{
    const QJsonArray j_items( j_itm.value("items").toArray() );
        for( auto it = j_items.begin(); it != j_items.end(); it++ ) {
            const QJsonObject j_item( it->toObject( ) );
            o_storage->addOrder( j_item );
        }
}

void OrderWorker::onActiveOrderEvent(const QJsonObject &j_itm)
{
    const QJsonArray j_items( j_itm.value("items").toArray() );
        for( auto it = j_items.begin(); it != j_items.end(); it++ ) {
            const QJsonObject j_item( it->toObject( ) );
            o_storage->addOrder( j_item );
        }
}

QJsonObject OrderWorker::getTradingSummary(const QVariantList &v)  {
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

void OrderWorker::onOrdersFromDb(const QJsonArray &stored_) {
    for ( auto it = stored_.begin(); it != stored_.end(); it++ ) {
        o_storage->addOrder( it->toObject() );
    }
}

void OrderWorker::startClient() {
    blockSignals( true );
    if( socket_in.isOpen() ) {
        socket_in.close();
    }
    if( socket_out.isOpen() ) {
        socket_out.close();
    }
    if ( session != nullptr ) {
        //   delete session;
    }
    blockSignals( false );
    session = new Wamp::Session( socket_in, Wamp::Session::MessageFormat::Msgpack, false );
    connect( &socket_in, &QTcpSocket::connected, session, &Wamp::Session::start );

    QObject::connect(session, &Wamp::Session::joined, [&](qint64 s) {
        onWampSession( session );
     });

    connect( session, &Wamp::Session::started, [&]() {
        qWarning() << "Session started";
        session->join( SwiftLib::getSettings()->value(SETTINGS_WAMP_REALM, "swiftbot").toString() );

    });
    connect( session, &Wamp::Session::left, [&](){
        qWarning() << "session left";
    });

    socket_out.connectToHost( SwiftLib::getSettings()->value(SETTINGS_WAMP_SERVER, "localhost").toString(), 8192 );
    if ( socket_out.waitForConnected() ) {
        socket_in.connectToHost( SwiftLib::getSettings()->value(SETTINGS_WAMP_SERVER, "localhost").toString(), 8193);
    }
}

void OrderWorker::processActiveQueue() {
    if ( _queued_active_get.isEmpty() ) {
        QTimer::singleShot( 1000, this, &OrderWorker::processActiveQueue );
    } else {
        const QPair<quint32,quint32> ids = _queued_active_get.dequeue();
        const QJsonObject params({{"market_id",QString::number( ids.second )}});
        const QString p( QJsonDocument( params ).toJson( QJsonDocument::Compact ) );
        const quint32 uid = session->call("swift.api.rpc."+QString::number(ids.first)+".trade.active", {p}).toUInt();
        if ( uid < 1000 ) {
            qWarning() << ids.first << "DISABLE EXCHANGE";
        }
        QTimer::singleShot( 1500, this, &OrderWorker::processActiveQueue );
    }
}

void OrderWorker::processHistoryQueue() {
    if ( _queued_active_get.isEmpty() ) {
        QTimer::singleShot( 1000, this, &OrderWorker::processHistoryQueue );
    } else {
        const QPair<quint32,quint32> ids = _queued_history_get.dequeue();
        const QJsonObject params({{"market_id",QString::number( ids.second )}});
        const QString p( QJsonDocument( params ).toJson( QJsonDocument::Compact ) );
        const quint32 uid = session->call("swift.api.rpc."+QString::number(ids.first)+".trade.history", {p}).toUInt();
        if ( uid < 1000 ) {
            qWarning() << ids.first << "DISABLE EXCHANGE";
        }
        QTimer::singleShot( 1500, this, &OrderWorker::processHistoryQueue );
    }
}

void OrderWorker::requestActives() {
    const QStringList exchanges( getConfigValue("EXCHANGES").split(",") );
    if ( !exchanges.isEmpty() ) {
        for( auto it = exchanges.begin(); it!= exchanges.end(); it++ ) {
            const QString ename( *it );
            const quint32 eid = assets->getExchangeId( ename );
            if ( assets->isSeparatedApi( eid ) ) {
                const QList<quint32> pids( assets->getExchangePairs( eid ) );
                for( auto iit = pids.begin(); iit != pids.end(); iit++ ) {
                    _queued_active_get.enqueue({eid, *iit });
                }
            } else {
                const quint32 uid = session->call("swift.api.rpc."+QString::number(eid)+".trade.active").toUInt();
                if ( uid < 1000 ) {
                    qWarning() << eid << "DISABLE EXCHANGE";
                }
            }
        }
    }
    QTimer::singleShot( 14000, this, &OrderWorker::processActiveQueue );
}

void OrderWorker::requestHistory() {
    const QStringList exchanges( getConfigValue("EXCHANGES").split(",") );
    if ( !exchanges.isEmpty() ) {
        for( auto it = exchanges.begin(); it!= exchanges.end(); it++ ) {
            const QString ename( *it );
            const quint32 eid = assets->getExchangeId( ename );
            if ( assets->isSeparatedApi( eid ) ) {
                const QList<quint32> pids( assets->getExchangePairs( eid ) );
                for( auto iit = pids.begin(); iit != pids.end(); iit++ ) {
                    _queued_history_get.enqueue({eid, *iit });
                }
            } else {
                const quint32 uid = session->call("swift.api.rpc."+QString::number(eid)+".trade.history").toUInt();
                if ( uid < 1000 ) {
                    qWarning() << eid << "DISABLE EXCHANGE";
                }
            }
        }
    }
    QTimer::singleShot( 64000, this, &OrderWorker::processActiveQueue );
}
