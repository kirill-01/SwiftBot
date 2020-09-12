#include "arbitragemanager.h"
#include "arbitragepair.h"
#include "arbitragewindow.h"
#include "swiftlib.h"

QStringList ArbitrageManager::getWindowOrdersUids(const quint32 &wid)  {
    static QMap<quint32,QStringList> _linked_orders;
    static qint64 lastloadtime = 0;
    if ( _linked_orders.isEmpty() || QDateTime::currentSecsSinceEpoch() - lastloadtime > 30 ) {
        QFile f( ORDERS_REL_FILENAME );
        f.open( QFile::ReadOnly );
        const QJsonObject j_cached( QJsonDocument::fromJson( f.readAll().constData() ).object() );
        f.close();
        if ( !j_cached.isEmpty() ) {
            for( auto it = j_cached.begin(); it != j_cached.end(); it++) {
                QStringList ordersuids;
                const QJsonArray uids( it.value().toArray() );
                for ( auto it2 = uids.begin(); it2 != uids.end(); it2++ ) {
                    ordersuids.push_back( it2->toString() );
                }
                _linked_orders[ it.key().toUInt() ] = ordersuids;
            }
        }
        lastloadtime = QDateTime::currentSecsSinceEpoch();
    }
    return _linked_orders.value( wid );
}

QMap<quint32, QStringList> ArbitrageManager::getWindowOrdersRels() {
    static QMap<quint32,QStringList> _linked_orders;
    static qint64 lastloadtime = 0;
    if ( _linked_orders.isEmpty() || QDateTime::currentSecsSinceEpoch() - lastloadtime > 30 ) {
        QFile f( ORDERS_REL_FILENAME );
        f.open( QFile::ReadOnly );
        const QJsonObject j_cached( QJsonDocument::fromJson( f.readAll().constData() ).object() );
        f.close();
        if ( !j_cached.isEmpty() ) {
            for( auto it = j_cached.begin(); it != j_cached.end(); it++) {
                QStringList ordersuids;
                const QJsonArray uids( it.value().toArray() );
                for ( auto it2 = uids.begin(); it2 != uids.end(); it2++ ) {
                    ordersuids.push_back( it2->toString() );
                }
                _linked_orders[ it.key().toUInt() ] = ordersuids;
            }
        }
        lastloadtime = QDateTime::currentSecsSinceEpoch();
    }
    return QMap<quint32,QStringList> (_linked_orders);
}

ArbitrageManager::ArbitrageManager(QObject *parent) : QObject( parent ), create_db_windows_timer( new QTimer() )
{
    dbname = SwiftLib::getUserDbName();
    loadDbPairs();
    create_db_windows_timer->setInterval( 10000 );
    connect( create_db_windows_timer, &QTimer::timeout, this, &ArbitrageManager::checkNewWindows );
    create_db_windows_timer->start();
    QTimer::singleShot( 100, this, &ArbitrageManager::loadPairSettings );
    //QTimer::singleShot( 1000, this, &ArbitrageManager::loadDbPairs );
    QTimer::singleShot( 15000, this, &ArbitrageManager::updateRequested );
    QTimer::singleShot( 6000, this, &ArbitrageManager::loadPairRatesHistory );
    QTimer::singleShot( 60000, this, &ArbitrageManager::saveOrdersRelsToDisk );
    QTimer::singleShot( 5000, this, &ArbitrageManager::loadOrdersRelFromDisk );
}

QHash<quint32, QList<quint32> > ArbitrageManager::getPairsRels() const {
    return SwiftLib::getAssets()->getArbitragePairs();
}

QJsonObject ArbitrageManager::toJson(const bool & show_active_only) {
    static QJsonObject cached;
    ethusd = getParam("ETHUSDPAIR");
    btcusd = getParam("BTCUSDPAIR");
    static qint64 last_check_time = 0;
    if ( QDateTime::currentSecsSinceEpoch() - last_check_time < 5 && !cached.isEmpty() ) {
        return cached;
    }
    static quint32 cntr = 0;
    if ( cntr >= 50 ) {
        loadPairSettings();
        cntr = 0;
    }
    cntr++;

    QJsonObject ret;
    QJsonArray j_pairs;


    if ( btcusd == "1" ) {
         j_pairs.push_back( _arbitrage_pairs[1]->toJson( show_active_only ) );
    }
    if ( ethusd == "1" ) {
         j_pairs.push_back( _arbitrage_pairs[2]->toJson( show_active_only ) );
    }
    ret["items"] = j_pairs;
    cached = ret;
    last_check_time = QDateTime::currentSecsSinceEpoch();
    return ret;
}

QJsonArray ArbitrageManager::getWindowHistory(const quint32 &window_id) {
    QJsonArray r;
    QSqlQuery q("SELECT min(spread_percent) as min_spr, max(spread_percent) as max_spr, td as hr FROM `"+ dbname +"`.`window_history` WHERE window_id="+QString::number( window_id )+" AND date(td)<=date_sub(now(), INTERVAL 6 hour) GROUP BY date(td)" );
    if ( q.exec() )  {
        while ( q.next() ) {
            QJsonArray it;
            it.push_back( QString::number( q.value("hr").toDateTime().toSecsSinceEpoch() ) );
            it.push_back( QString::number( q.value("min_spr").toDouble(), 'f', 8 ) );
            it.push_back( QString::number( q.value("max_spr").toDouble(), 'f', 8 ) );
            r.push_back( it );
        }
        q.finish();
    } else {
        qWarning() << "WinSpreadHistory error" << q.lastError().text();
    }
    return r;

}

QJsonArray ArbitrageManager::getWindowTodayHistory(const quint32 &window_id) {
    QJsonArray r;
    QSqlQuery q("SELECT window_id, min(spread_percent) as mins, max(spread_percent) as maxs, hour( td ) as `hour` FROM window_history WHERE date(td)=curdate() AND window_id="+QString::number( window_id )+" GROUP BY window_id,hour(td)");
    if ( q.exec() )  {
        while ( q.next() ) {
            QJsonArray it;
            it.push_back( QString::number( q.value("hour").toUInt() ) );
            it.push_back( QString::number( q.value("mins").toDouble(), 'f', 4 ) );
            it.push_back( QString::number( q.value("maxs").toDouble(), 'f', 4 ) );
            r.push_back( it );
        }
        q.finish();
    } else {
        qWarning() << "WinSpreadHistory error" << q.lastError().text();
    }
    return r;
}

QJsonArray ArbitrageManager::explainWindow(const quint32 &id, const double &amount) {
    Q_UNUSED(id)
    Q_UNUSED(amount)
    return QJsonArray(); //( getWindow( id )->explain( amount ) );
}

ArbitrageWindow *ArbitrageManager::getWindow(const quint32 &wid) {
    for( auto ai = _arbitrage_pairs.begin(); ai != _arbitrage_pairs.end(); ai++ ) {
        if ( _arbitrage_pairs[ ai.key() ]->hasWindow( wid ) ) {
            return _arbitrage_pairs[ ai.key() ]->getWindow( wid );
        }
    }
    return new ArbitrageWindow(this);
}

bool ArbitrageManager::hasWindow(const quint32 &wid) const {
    for( auto ai = _arbitrage_pairs.begin(); ai != _arbitrage_pairs.end(); ai++ ) {
        if ( _arbitrage_pairs[ ai.key() ]->hasWindow( wid ) ) {
            return true;
        }
    }
    return false;
}

void ArbitrageManager::updateRequestedRecord(const quint32 &cid, const double &am) {
    if ( am > 0 ) {
        QSqlQuery q;
        q.prepare("INSERT INTO `"+dbname+"`.`currency_requested` (`currency_id`,`requested`) VALUES (:cid, :req)");
        q.bindValue(":cid", cid );
        q.bindValue(":req", am );
        if ( !q.exec( ) ) {
            qWarning() << q.lastError().text();
        }
        q.finish();
    }
}

void ArbitrageManager::setRequested(const quint32 &pair_id, const QMap<quint32, double> &part) {
    _requested_separated[ pair_id ] = part;

}

void ArbitrageManager::linkOrders(const QString &first, const QString &second, const quint32 &wid) {
    QMap<quint32,QStringList> _linked( ArbitrageManager::getWindowOrdersRels() );
    _linked[ wid ].push_back( first );
    _linked[ wid ].push_back( second );
    QFile f( ORDERS_REL_FILENAME );
    f.open( QFile::ReadWrite );
    QJsonObject j_dat;
    for( auto it = _linked.begin(); it != _linked.end(); it++ ) {
        j_dat[ QString::number( it.key() ) ] = QJsonArray::fromStringList( it.value() );
    }
    const QString str( QJsonDocument( j_dat ).toJson( QJsonDocument::Compact ) );
    f.resize(0);
    f.write( str.toUtf8() );
    f.close();
}

void ArbitrageManager::linkOrder(const QString &local_id, const quint32 &wid, const double &am) {
    _linked_orders[wid].push_back( local_id );
    if ( !_window_amounts[wid].contains( am ) ) {
        _window_amounts[wid].push_back( am );
    }
}

void ArbitrageManager::updateParam(const QString &param, const QString &value) {
    _cached_params[param] = value;
}

QString ArbitrageManager::getParam(const QString &p) {
    if ( _cached_params.isEmpty() ) {
        QSqlQuery q("SELECT * from `settings_storage`");
        if ( q.exec() ) {
            while ( q.next() ) {
                _cached_params[ q.value("param").toString() ] = q.value("value").toString();
            }
        }
    }
    if( _cached_params.contains( p ) ) {
        return _cached_params.value(p);
    } else {
        if ( sess != nullptr && sess->isJoined() ) {
            QString val = sess->call("swift.assets.rpc.settings.get",{p} ).toString().split("=").at(1);
            _cached_params[p] = val;
            return val;
        } else {
            _cached_params[p] = SwiftLib::getSettings()->value(p,"0").toString();
            return _cached_params.value(p);
        }
    }

}

quint32 ArbitrageManager::getWindowOrdersCount(const quint32 &wid) const {
    return _linked_orders.value(wid).size();
}

double ArbitrageManager::getPairMinRate(const quint32 &pid) const {
    return _min_rates.value( pid, 0 );
}

double ArbitrageManager::getPairMaxRate(const quint32 &pid) const {
    return _max_rates.value( pid, 0 );
}

double ArbitrageManager::getPairAvgRate(const quint32 &pid) const {
    return _avg_rates.value( pid, 0 );
}

void ArbitrageManager::updateRequested() {
    _aggregated_requests.clear();
    if ( !_requested_separated.isEmpty() ) {
        const QMap<quint32,QMap<quint32, double>> _r( _requested_separated );
        for( auto it = _r.begin(); it != _r.end(); it++ ) {
            const QMap<quint32, double> __r( it.value() );
            for( auto itt = __r.begin(); itt != __r.end(); itt++ ) {
                _aggregated_requests[ itt.key() ] += itt.value();
            }
        }
    }

    if ( !_aggregated_requests.isEmpty() ) {
        QVariantList _vl;
        for ( auto it = _aggregated_requests.begin(); it != _aggregated_requests.end(); it++ ) {
            _vl.push_back( QString::number( it.key() )+"="+QString::number( it.value(), 'f', 8 ) );
            updateRequestedRecord( it.key(), it.value() );
        }
    }
    if ( sess != nullptr && sess->isJoined() ) {
        sess->call("swift.system.rpc.logs.log", {"Arbitrage","DEBUG","Updating requirements data"});
    }
    QTimer::singleShot( 15000, this, &ArbitrageManager::updateRequested );
}

void ArbitrageManager::calculateOrderbooks(const QHash<quint32, QMap<double, double> > &asks, const QHash<quint32, QMap<double, double> > &bids) {

    emit tocalculateOrderbooks( asks, bids );

}

void ArbitrageManager::checkNewWindows() {
    for ( auto it = _arbitrage_pairs.begin(); it != _arbitrage_pairs.end(); it++ ) {
        it.value()->createNewWindows();
    }
}



void ArbitrageManager::onWampSession(Wamp::Session *session) {
    sess = session;

    for( auto it = _arbitrage_pairs.begin(); it != _arbitrage_pairs.end(); it++ ) {
        connect( this, &ArbitrageManager::errorOrderEvent, _arbitrage_pairs[it.key()], &ArbitragePair::onErrorOrder );
        connect( this, &ArbitrageManager::completedOrderEvent, _arbitrage_pairs[it.key()], &ArbitragePair::onCompletedOrder );
        connect( this, &ArbitrageManager::placedOrderEvent, _arbitrage_pairs[it.key()], &ArbitragePair::onPlacedOrder );
        connect( this, &ArbitrageManager::canceledOrderEvent, _arbitrage_pairs[it.key()], &ArbitragePair::onCanceledOrder );
        _arbitrage_pairs[it.key()]->setWampSession( sess );
        _arbitrage_pairs[it.key()]->prepareCachedBals();
    }

    sess->provide("swift.trade.rpc.window", [&](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        QJsonArray j_ret;
        if ( v.isEmpty() ) {
            for( auto ai = _arbitrage_pairs.begin(); ai != _arbitrage_pairs.end(); ai++ ) {
                for( auto wi = _arbitrage_pairs[ ai.key() ]->_windows.begin(); wi != _arbitrage_pairs[ ai.key() ]->_windows.end(); wi++) {
                    j_ret.push_back( wi.value()->toJson( true ) );
                }
            }
        } else {
            for( auto it = v.begin(); it != v.end(); it++ ) {
                if ( hasWindow( it->toUInt() ) ) {
                    j_ret.push_back( getWindow( it->toUInt() )->toJson( true ) );
                }
            }
        }
        if ( j_ret.count() == 1 ) {
            const QString r( QJsonDocument( j_ret.at(0).toObject() ).toJson( QJsonDocument::Compact ) );
            return r;
        } else {
            const QString r( QJsonDocument( j_ret ).toJson( QJsonDocument::Compact ) );
            return r;
        }
    });


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
        }
    });
    emit sessionConnected( sess );
    QTimer::singleShot( 12500, this, &ArbitrageManager::sendSnapshot );
}

void ArbitrageManager::loadPairSettings() {
    const QStringList _enabled_exchanges( getParam("EXCHANGES").split(",") );
    _en_exchanges.clear();
    if ( !_enabled_exchanges.isEmpty() ) {
        for( auto it = _enabled_exchanges.begin(); it != _enabled_exchanges.end(); it++ ) {
            _en_exchanges.push_back( SwiftLib::getAssets()->getExchangeId( *it ) );
        }
    }
}


void ArbitrageManager::loadDbPairs() {
    QSqlQuery prep_query("CREATE TABLE IF NOT EXISTS `"+ dbname +"`.`window_history` (\
      `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,\
      `spread_size` DOUBLE(16,4) NOT NULL DEFAULT 0,\
      `spread_percent` DOUBLE(16,4) NOT NULL DEFAULT 0,\
      `td` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,\
      `weight` DOUBLE(16,4) NOT NULL DEFAULT 0,\
      `window_id` INT UNSIGNED NOT NULL,\
      PRIMARY KEY (`id`), UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE);");
    if ( !prep_query.exec() ) {
        qWarning() << "PrepQueryInManager failed" << prep_query.lastError().text();
    }


    loadPairSettings();



    QSqlQuery q("SELECT * FROM arbitrage_pairs WHERE is_enabled=1");
    if ( q.exec() ) {
        while( q.next() ) {
            const quint32 pair_id(  q.value("id").toUInt() );
            const quint32 base_coin( q.value("base_coin_id").toUInt() );
            const quint32 price_coin( q.value("price_coin_id").toUInt() );
            _arbitrage_pairs[ pair_id ] = new ArbitragePair( q.record(), this );
            _arbitrage_pairs[ pair_id ]->setCoins( base_coin, price_coin );
            if ( sess !=nullptr && sess->isJoined() ) {
                _arbitrage_pairs[ pair_id ]->setWampSession( sess );
            }
            QList<QPair<quint32,quint32>> _pairs( _arbitrage_pairs[ pair_id ]->getTradePairs() );
            for ( auto it = _pairs.begin(); it != _pairs.end(); it++ ) {
                _pairs_rels.insert(*it, pair_id);
            }
        }
        q.finish();
    } else {
        qWarning() << "select apairs" << q.lastError().text();
    }

    QSqlQuery qq("SELECT * FROM pairs where is_enabled=1");
    if ( qq.exec( ) ) {
        while ( qq.next() ) {
            if ( _en_exchanges.contains( qq.value("exchange_id").toUInt() ) ) {
                _ap.insert( qq.value("id").toUInt(), qq.value("arbitrage_pair_id").toUInt() );
            }
        }
    } else {
        qWarning() << "select pairs" << qq.lastError().text();
    }


    ethusd = getParam("ETHUSDPAIR");
    btcusd = getParam("BTCUSDPAIR");

    if ( btcusd == "1" && _arbitrage_pairs.contains(1) ) {
       // QObject::disconnect(this, &ArbitrageManager::tocalculateOrderbooks, _arbitrage_pairs[1], &ArbitragePair::calculateOrderbooks );
     //   connect( this, &ArbitrageManager::sessionConnected, _arbitrage_pairs[1], &ArbitragePair::setWampSession );
        connect( this, &ArbitrageManager::tocalculateOrderbooks, _arbitrage_pairs[1], &ArbitragePair::calculateOrderbooks, Qt::QueuedConnection );
    }
    if ( ethusd == "1" && _arbitrage_pairs.contains(2)  ) {
       // QObject::disconnect(this, &ArbitrageManager::tocalculateOrderbooks, _arbitrage_pairs[2], &ArbitragePair::calculateOrderbooks );
       // connect( this, &ArbitrageManager::sessionConnected, _arbitrage_pairs[1], &ArbitragePair::setWampSession );
        connect( this, &ArbitrageManager::tocalculateOrderbooks, _arbitrage_pairs[2], &ArbitragePair::calculateOrderbooks, Qt::QueuedConnection );
    }

}

void ArbitrageManager::loadPairRatesHistory() {
    QSqlQuery q("SELECT pair_id, avg(rate) as avg_rate, min(rate) as min_rate, max(rate) as max_rate from rates where date(ts)=curdate() GROUP by pair_id");
    if ( q.exec() ) {
        while( q.next() ) {
            const quint32 pid = q.value("pair_id").toUInt();
            _avg_rates[ pid ] = q.value("avg_rate").toDouble();
            _min_rates[ pid ] = q.value("min_rate").toDouble();
            _max_rates[ pid ] = q.value("max_rate").toDouble();
        }
    }
    q.finish();
    QTimer::singleShot( 60000, this, &ArbitrageManager::loadPairRatesHistory );
}

void ArbitrageManager::saveOrdersRelsToDisk() {
    if ( !_linked_orders.isEmpty() ) {
        QFile f( ORDERS_REL_FILENAME );
        f.open( QFile::ReadWrite );
        QJsonObject j_dat;
        for( auto it = _linked_orders.begin(); it != _linked_orders.end(); it++ ) {
            j_dat[ QString::number( it.key() ) ] = QJsonArray::fromStringList( it.value() );
        }
        const QString str( QJsonDocument( j_dat ).toJson( QJsonDocument::Compact ) );
        f.resize(0);
        f.write( str.toUtf8() );
        f.close();
    }
    QTimer::singleShot( 30000, this, &ArbitrageManager::saveOrdersRelsToDisk );
}

void ArbitrageManager::loadOrdersRelFromDisk() {
    QFile f( ORDERS_REL_FILENAME );
    f.open( QFile::ReadOnly );
    const QJsonObject j_cached( QJsonDocument::fromJson( f.readAll().constData() ).object() );
    f.close();
    if ( !j_cached.isEmpty() ) {
        for( auto it = j_cached.begin(); it != j_cached.end(); it++) {
            QStringList ordersuids;
            const QJsonArray uids( it.value().toArray() );
            for ( auto it2 = uids.begin(); it2 != uids.end(); it2++ ) {
                ordersuids.push_back( it2->toString() );
            }
            _linked_orders[ it.key().toUInt() ] = ordersuids;
        }
    }
    QTimer::singleShot( 30000, this, &ArbitrageManager::loadOrdersRelFromDisk );
}

void ArbitrageManager::sendSnapshot() {
    const QJsonObject ret( toJson( true ) );
    const QString r( QJsonDocument( ret ).toJson( QJsonDocument::Compact ) );
    if ( sess != nullptr && sess->isJoined() ) {
        sess->publish("swift.orders.feed.windows.snapshot", {r});
    }
    QTimer::singleShot( 2500, this, &ArbitrageManager::sendSnapshot );
}
