#include "arbitragepair.h"
#include "arbitragewindowsprint.h"
#include "arbitragewindow.h"
#include "arbitragemanager.h"
ArbitrageCandidat::ArbitrageCandidat(const quint32 &_arbitrage_pair_id, const quint32 &_sell_pair_id, const quint32 &_buy_pair_id, const double &_sell_rate, const double &_sell_price, const double &_sell_fee, const double &_buy_rate, const double &_buy_price, const double &_buy_fee, const double &_amount) :
    arbitrage_pair_id(_arbitrage_pair_id),
    sell_pair_id(_sell_pair_id),
    buy_pair_id(_buy_pair_id),
    sell_rate(_sell_rate),
    sell_price(_sell_price),
    sell_fee(_sell_fee),
    buy_rate(_buy_rate),
    buy_price(_buy_price),
    buy_fee(_buy_fee),
    amount(_amount)
{

}

ArbitrageCandidat::ArbitrageCandidat(const ArbitrageCandidat &other) :
    arbitrage_pair_id(other.arbitrage_pair_id),
    sell_pair_id(other.sell_pair_id),
    buy_pair_id(other.buy_pair_id),
    sell_rate(other.sell_rate),
    sell_price(other.sell_price),
    sell_fee(other.sell_fee),
    buy_rate(other.buy_rate),
    buy_price(other.buy_price),
    buy_fee(other.buy_fee),
    amount(other.amount)
{

}

ArbitrageCandidat ArbitrageCandidat::operator=(const ArbitrageCandidat &other) {
    arbitrage_pair_id = other.arbitrage_pair_id;
    sell_pair_id = other.sell_pair_id;
    buy_pair_id = other.buy_pair_id;
    sell_rate = other.sell_rate;
    sell_price = other.sell_price;
    sell_fee = other.sell_fee;
    buy_rate = other.buy_rate;
    buy_price = other.buy_price;
    buy_fee = other.buy_fee;
    amount = other.amount;
    return *this;
}

double ArbitrageCandidat::profit() const {
    return ( sell_price - ( sell_price * sell_fee ) ) - (buy_price + ( buy_price * buy_fee ) );
}

QJsonObject ArbitrageCandidat::toJson() const {
    QJsonObject j_ret;
    j_ret["arbitrage_pair_id"] = QString::number( arbitrage_pair_id );
    j_ret["sell_pair_id"] = QString::number( sell_pair_id );
    j_ret["buy_pair_id"] = QString::number( buy_pair_id );
    j_ret["sell_rate"] = QString::number( sell_rate, 'f', 8 );
    j_ret["sell_price"] = QString::number( sell_price, 'f', 8 );
    j_ret["sell_fee"] = QString::number( sell_fee, 'f', 8 );
    j_ret["buy_rate"] = QString::number( buy_rate, 'f', 8 );
    j_ret["buy_price"] = QString::number( buy_price, 'f', 8 );
    j_ret["buy_fee"] = QString::number( buy_fee, 'f', 8 );
    j_ret["amount"] = QString::number( amount, 'f', 8 );
    j_ret["profit"] = QString::number( profit(), 'f', 8 );
    return j_ret;
}


double ArbitragePair::getBalance(const quint32 &cid) {
    if ( !_cached_reqs.contains( cid ) ) {
        if ( session != nullptr && session->isJoined() ) {
             const QString& j_ret( session->call("swift.balance.rpc.required",{cid}).toString() );
             const QJsonObject j_o( QJsonDocument::fromJson( j_ret.toUtf8() ).object() );
             const double b =  j_o.value( QString::number( cid ) ).toString().toDouble();
            _cached_reqs.insert( cid, b );
        }
    }
    return _cached_reqs.value(cid, 0);
}

double ArbitragePair::getBalanceAvailable(const quint32 &cid, const bool& force)
{
    if ( !_cached_bals.contains( cid ) || force ) {
        if ( session != nullptr && session->isJoined() ) {
            const QVariant res = session->call("swift.balance.rpc.get");
            const QJsonArray j_a_r( res.toJsonArray() );
            if ( !j_a_r.isEmpty() ) {
                for( auto it = j_a_r.begin(); it != j_a_r.end(); it++ ) {
                    const quint32 _cid = it->toArray().at(0).toString().toUInt();
                    const double _av = it->toArray().at(1).toString().toDouble();
                    _cached_bals[ _cid ] = _av;
                }
            }
        }
    }
    return _cached_bals.value(cid, 0);
}

bool ArbitragePair::hasWindow(const quint32 &id) const {
    return _windows.keys().contains( id );
}

void ArbitragePair::prepareCachedBals() {
    if ( _windows.isEmpty() ) {
        loadDbWindows();
    }
    for( auto it = _windows.begin(); it!= _windows.end(); it++) {
        const double bb = getBalanceAvailable( _windows[it.key()]->getBuyCurrency(), true );
        _cached_bals[ it.value()->getBuyCurrency() ] = bb;
        const double sb = getBalanceAvailable(  _windows[it.key()]->getSellCurrency(), true );
        _cached_bals[ it.value()->getSellCurrency() ] = sb;
    }
}

void ArbitragePair::updateCurrenciesRequirements() {
    QMap<quint32, double> _summary;

    QMap<quint32,QMap<quint32,double>> _from( _required_summary );
    for( auto it = _from.begin(); it != _from.end(); it++ ) {
        const QMap<quint32,double> _r( it.value() );
        for ( auto it2 = _r.begin(); it2 != _r.end(); it2++ ) {
            _summary[ it2.key() ] += it2.value();
        }
    }
    getManager()->setRequested( id, _summary );

}

ArbitrageManager *ArbitragePair::getManager() {
    return qobject_cast<ArbitrageManager*>( parent() );
}

ArbitrageWindow *ArbitragePair::getWindow(const quint32 &id) {
    return _windows[ id ];
}

void ArbitragePair::insertOrderCandidat(OrderParams *sell_order, OrderParams *buy_order) {
    _orders_candidats.push_back( {sell_order, buy_order} );
}

void ArbitragePair::placeOrders() {

}

void ArbitragePair::calculateOrderbooks(const QHash<quint32, QMap<double, double> > &asks, const QHash<quint32, QMap<double, double> > &bids) {

    QHash<quint32, QMap<double, double> > aa;
    QHash<quint32, QMap<double, double> > bb;

    for( auto it = asks.begin(); it != asks.end(); it++ ) {
        if ( _valid_pairs.contains( it.key() ) ) {
           aa.insert( it.key(), it.value() );
        }
    }
    for( auto it = bids.begin(); it != bids.end(); it++ ) {
        if ( _valid_pairs.contains( it.key() ) ) {
           bb.insert( it.key(), it.value() );
        }
    }
    if ( aa.isEmpty() && bb.isEmpty() ) {
        return;
    }

    emit tocalculateOrderbooks(aa, bb );


    double ttlsell = 0;
    double ttlbuy = 0;
    for( auto it = aa.begin(); it != aa.end(); it++ ) {
        _a[it.key()] = it.value();
    }
    const QHash<quint32, QMap<double, double> > a( _a );
    for( auto it = a.begin(); it != a.end(); it++ ) {
        for( auto it2 = it.value().begin(); it2 != it.value().end(); it2++ ) {
            ttlsell += it2.key() * it2.value();
        }
    }
    for( auto it = bb.begin(); it != bb.end(); it++ ) {
        _b[it.key()] = it.value();
    }
     const QHash<quint32, QMap<double, double> > b( _b );
    for( auto it = b.begin(); it != b.end(); it++ ) {
        for( auto it2 = it.value().begin(); it2 != it.value().end(); it2++ ) {
            ttlbuy += it2.key() * it2.value();
        }
    }

    for( auto it = _windows.begin(); it != _windows.end(); it++ ) {
        const double spr = it.value()->getSpread();
        if ( spr > max_spread ) { max_spread = spr; }
        if ( spr < min_spread ) { min_spread = spr; }
    }


    ttl_funds = ttlbuy + ttlsell;
    sellpct = ttlsell / ttl_funds * 100;

    if ( is_locked ) {
        if ( QDateTime::currentSecsSinceEpoch() - lock_time >= 45 ) {
            unlock();
        }
    } else {
        checkCandidats();
    }




}

void ArbitragePair::reloadCache() {
    updateCurrenciesRequirements();
    _cached_bals.clear();
    _cached_reqs.clear();
    QTimer::singleShot( 12000, this, &ArbitragePair::reloadCache );

}

QJsonObject ArbitragePair::toJson( const bool & show_active_only  ) {
    Q_UNUSED( show_active_only )
    QJsonObject j_ret;
    j_ret["id"] = QString::number( id );
    j_ret["base_currency_name"] =  base_currency_name;
    j_ret["price_currency_name"] =  price_currency_name;

    j_ret["spread_percent"] = QString::number( getSpreadSize(), 'f', 4 );


    j_ret["sell_coin_id"] = QString::number( sell_coin_id );
    j_ret["buy_coin_id"] = QString::number( buy_coin_id );

    j_ret["min_spread"] = QString::number(  min_spread, 'f', 8 );
    j_ret["max_spread"] = QString::number(  max_spread, 'f', 8 );
    j_ret["total_ticks"] = QString::number( getTotalTicks() );
    j_ret["sellpct"] = QString::number(  sellpct, 'f', 8 ); ;
    QJsonArray jj_wi;
    QList<double> sell_rates;
    QList<double> buy_rates;

    const QList<quint32> wids( _windows.keys( ) );
    for ( auto it = wids.begin(); it != wids.end(); it++ ) {
         const quint32 wid =*it;
                if (  getWindow(wid)->isValid() && getWindow(wid)->getSpread() > -0.15  ) {
                    sell_rates.push_back( getWindow(wid)->sell_rate );
                    buy_rates.push_back( getWindow(wid)->buy_rate );
                    QJsonObject jj( getWindow(wid)->toJson(false) );
                    jj["base_currency_name"] =  base_currency_name;
                    jj["price_currency_name"] =  price_currency_name;
                    jj_wi.push_back( jj );
                }
                // ttlsam += it.value()->amountsumm();
    }
    std::sort( sell_rates.begin(), sell_rates.end() );
    std::sort( buy_rates.begin(), buy_rates.end() );
    j_ret["sell_rate"] = buy_rates.isEmpty() ? "0" : QString::number( buy_rates.first(), 'f', 5 );
    j_ret["buy_rate"] = sell_rates.isEmpty() ? "0" : QString::number( sell_rates.first(), 'f', 5 );
    j_ret["value"] = QString::number( ttl_funds, 'f', 2 );
    j_ret["windows"] = jj_wi;

    return j_ret;
}

bool ArbitragePair::sortWindows(const QJsonObject &a, const QJsonObject &b)
{
    return a.value("weight").toString().toDouble() >= b.value("weight").toString().toDouble();
}

ArbitragePair::ArbitragePair(QObject *parent) : QObject( parent ), session(nullptr) , id(0), min_spread(0),max_spread(0){

}

ArbitragePair::ArbitragePair(const QSqlRecord &rec, QObject *parent) : QObject( parent ),  session(nullptr) , min_spread(0),max_spread(0) {

    id = rec.value("id").toUInt();
    sell_coin_id = rec.value("base_coin_id").toUInt();
    buy_coin_id = rec.value("price_coin_id").toUInt();
    base_currency_name = SwiftLib::getAssets()->getCoinName( sell_coin_id );
    price_currency_name = SwiftLib::getAssets()->getCoinName( buy_coin_id );
    amount_scale = getManager()->getParam("AMOUNT_PRECISION").toUInt();
    rate_scale =  getManager()->getParam("RATE_PRECISION").toUInt();
    loadDbWindows();
    is_locked = false;
    QTimer::singleShot( 200, this, &ArbitragePair::loadDbWindows );

    QTimer::singleShot( 15000, this, &ArbitragePair::reloadCache );
    connect( this, &ArbitragePair::log, this, &ArbitragePair::sendLog );
    connect( this, &ArbitragePair::debug, this, &ArbitragePair::sendDebug );
}

void ArbitragePair::loadDbWindow(const quint32 &sell_pair, const quint32 &buy_pair) {
    if ( buy_pair <= 0 ||  sell_pair <= 0 ) {
        return;
    }
    QSqlQuery q("SELECT w.*,\
                sp.base_currency_id as sell_base_currency_id, \
                sp.market_currency_id as sell_price_currency_id, \
                bp.base_currency_id as buy_base_currency_id, \
                bp.market_currency_id as buy_price_currency_id \
                FROM windows w LEFT JOIN pairs sp ON sp.id=w.sell_market LEFT JOIN pairs bp ON bp.id=w.buy_market \
            LEFT JOIN currencies c1 ON c1.id=sp.base_currency_id LEFT JOIN currencies c2 ON c2.id=sp.market_currency_id \
            LEFT JOIN currencies c3 ON c3.id=bp.base_currency_id LEFT JOIN currencies c4 ON c4.id=bp.market_currency_id \
            LEFT JOIN exchanges e1 ON e1.id=w.sell_exchange LEFT JOIN exchanges e2 ON e2.id=w.buy_exchange \
            WHERE w.sell_market="+QString::number( sell_pair ).toUtf8()+" AND w.buy_market="+QString::number( buy_pair ).toUtf8()+" AND sp.is_enabled=1 AND bp.is_enabled=1");
            //AND c1.is_enabled=1 AND c2.is_enabled=1 AND c3.is_enabled=1 AND c4.is_enabled=1 AND e1.is_enabled=1 AND e2.is_enabled=1" );
     if ( q.exec() && q.next() ) {
                const QPair<quint32,quint32> _window_exchs(q.value("sell_market").toUInt(),q.value("buy_market").toUInt());
                const quint32 window_id(  q.value("id").toUInt() );
                if ( !_windows_rels.keys().contains( _window_exchs  ) ) {
                    _windows_rels.insert( _window_exchs, window_id );
            }
            if ( _windows.keys().contains( window_id ) ) {
                _windows[ window_id ]->update( q.record() );
            } else {
                _windows[ window_id ] = new ArbitrageWindow( q.record(), this );
                connect( this, &ArbitragePair::hasWampSession,  _windows[ window_id ], &ArbitrageWindow::onWampSession );
                connect( this, &ArbitragePair::errorOrder,  _windows[ window_id ], &ArbitrageWindow::onErrorOrder );
                connect( this, &ArbitragePair::completedOrder,  _windows[ window_id ], &ArbitrageWindow::onCompletedOrder );
                connect( this, &ArbitragePair::placedOrder,  _windows[ window_id ], &ArbitrageWindow::onPlacedOrder );
                connect( this, &ArbitragePair::canceledOrder,  _windows[ window_id ], &ArbitrageWindow::onCanceledOrder );
                connect( this, &ArbitragePair::tocalculateOrderbooks, _windows[ window_id ], &ArbitrageWindow::calculateOrderbooks, Qt::QueuedConnection );
    }

        q.finish();
    } else {
            qWarning() << q.executedQuery();
            qWarning() << q.lastError().text();
    }
}

void ArbitragePair::loadDbWindow(const quint32 &window_id) {
    if ( window_id <= 0 ) {
        return;
    }
    QSqlQuery q("SELECT w.*,\
                sp.base_currency_id as sell_base_currency_id,\
                sp.market_currency_id as sell_price_currency_id,\
                bp.base_currency_id as buy_base_currency_id,\
                bp.market_currency_id as buy_price_currency_id\
                FROM windows w LEFT JOIN pairs sp ON sp.id=w.sell_market LEFT JOIN pairs bp ON bp.id=w.buy_market\
            LEFT JOIN currencies c1 ON c1.id=sp.base_currency_id LEFT JOIN currencies c2 ON c2.id=sp.market_currency_id\
            LEFT JOIN currencies c3 ON c3.id=bp.base_currency_id LEFT JOIN currencies c4 ON c4.id=bp.market_currency_id \
            WHERE w.id="+QString::number( window_id ).toUtf8()+" AND w.arbitrage_pair_id="+QString::number( id ).toUtf8()+" AND sp.is_enabled=1 AND bp.is_enabled=1");
            // AND c1.is_enabled=1 AND c2.is_enabled=1 AND c3.is_enabled=1 AND c4.is_enabled=1" );
    if ( q.exec() ) {
        while( q.next() ) {
            const QPair<quint32,quint32> _window_exchs(q.value("sell_market").toUInt(),q.value("buy_market").toUInt());
            const quint32 window_id(  q.value("id").toUInt() );
            if ( !_windows_rels.keys().contains( _window_exchs  ) ) {
                _windows_rels.insert( _window_exchs, window_id );
            }
            if ( _windows.keys().contains( window_id ) ) {
                _windows[ window_id ]->update( q.record() );
            } else {
                _windows[ window_id ] = new ArbitrageWindow( q.record(), this );
                connect( this, &ArbitragePair::hasWampSession,  _windows[ window_id ], &ArbitrageWindow::onWampSession );
                connect( this, &ArbitragePair::errorOrder,  _windows[ window_id ], &ArbitrageWindow::onErrorOrder );
                connect( this, &ArbitragePair::completedOrder,  _windows[ window_id ], &ArbitrageWindow::onCompletedOrder );
                connect( this, &ArbitragePair::placedOrder,  _windows[ window_id ], &ArbitrageWindow::onPlacedOrder );
                connect( this, &ArbitragePair::canceledOrder,  _windows[ window_id ], &ArbitrageWindow::onCanceledOrder );
                connect( this, &ArbitragePair::tocalculateOrderbooks, _windows[ window_id ], &ArbitrageWindow::calculateOrderbooks, Qt::QueuedConnection );
            }
    }
    q.finish();
    } else {
        qWarning() << "ArbitragePair::loadDbWindow(const quint32 &window_id)" << q.lastError().text();
    }
}

void ArbitragePair::loadDbWindows() {
    all_ticks=0;
    if ( id == 0 ) {
        return;
    }
    const QList<quint32> _eexch( getManager()->_en_exchanges );
    QSqlQuery qp("SELECT `ap`.* FROM `pairs` ap LEFT JOIN exchanges e1 ON ap.exchange_id=e1.id  WHERE ap.arbitrage_pair_id="+QString::number( id ).toUtf8()+" AND ap.is_enabled=1 AND e1.is_enabled=1");
    if( qp.exec( ) ) {
        while ( qp.next() ) {
            if ( _eexch.contains( qp.value("exchange_id").toUInt() ) ) {
                _valid_pairs.push_back( qp.value("id").toUInt() );
            }
        }
    } else {
        qWarning() << "ArbitragePair::loadDbWindows()" << qp.lastError().text();
    }

    QList<QPair<quint32,quint32>> _loaded;
    QSqlQuery q("SELECT w.*,sp.base_currency_id as sell_base_currency_id,\
                sp.market_currency_id as sell_price_currency_id,\
                bp.base_currency_id as buy_base_currency_id,\
                bp.market_currency_id as buy_price_currency_id,\
                e1.id as sell_e_id, e2.id as buy_e_id \
                FROM windows w LEFT JOIN pairs sp ON sp.id=w.sell_market LEFT JOIN pairs bp ON bp.id=w.buy_market \
            LEFT JOIN currencies c1 ON c1.id=sp.base_currency_id LEFT JOIN currencies c2 ON c2.id=sp.market_currency_id \
            LEFT JOIN currencies c3 ON c3.id=bp.base_currency_id LEFT JOIN currencies c4 ON c4.id=bp.market_currency_id \
            LEFT JOIN exchanges e1 ON e1.id=w.sell_exchange LEFT JOIN exchanges e2 ON e2.id=w.buy_exchange \
            WHERE sp.is_enabled=1 and bp.is_enabled=1 AND e1.is_enabled=1 and e2.is_enabled=1 AND c1.is_enabled=1 AND c2.is_enabled=1 AND w.arbitrage_pair_id="+QString::number( id ).toUtf8() );
    if ( q.exec() ) {
        while( q.next() ) {
            if ( _eexch.contains( q.value("sell_e_id").toUInt() ) && _eexch.contains( q.value("buy_e_id").toUInt() ) ) {

                const QPair<quint32,quint32> _window_exchs(q.value("sell_market").toUInt(),q.value("buy_market").toUInt());
                _loaded.push_back( _window_exchs );
                if ( _valid_pairs.contains( _window_exchs.first ) && _valid_pairs.contains( _window_exchs.second ) ) {
                    const quint32 window_id(  q.value("id").toUInt() );
                    if ( !_windows_rels.keys().contains( _window_exchs  ) ) {
                        _windows_rels.insert( _window_exchs, window_id );
                    }
                    if ( !_windows.keys().contains( window_id ) ) {
                        _windows.insert( window_id, new ArbitrageWindow( q.record(), this ) );
                        connect( this, &ArbitragePair::hasWampSession,  _windows[ window_id ], &ArbitrageWindow::onWampSession );
                        connect( this, &ArbitragePair::errorOrder,  _windows[ window_id ], &ArbitrageWindow::onErrorOrder );
                        connect( this, &ArbitragePair::placedOrder,  _windows[ window_id ], &ArbitrageWindow::onPlacedOrder );
                        connect( this, &ArbitragePair::canceledOrder,  _windows[ window_id ], &ArbitrageWindow::onCanceledOrder );
                        connect( this, &ArbitragePair::tocalculateOrderbooks, _windows[ window_id ], &ArbitrageWindow::calculateOrderbooks, Qt::QueuedConnection );
                    }

                }

            }
         }
         q.finish();

        for( auto iit1 = _valid_pairs.begin(); iit1 != _valid_pairs.end(); iit1++  ) {
            const quint32 sid = *iit1;
            for( auto iit2 = _valid_pairs.begin(); iit2 != _valid_pairs.end(); iit2++  ) {
                const quint32 bid = *iit2;
                const QPair<quint32,quint32> wid(sid, bid);
                if ( !_loaded.contains( wid ) && sid != bid && !_windows_rels.contains( wid ) ) {
                    const quint32 nwid = createWindow( id, wid );
                    if ( nwid > 0 ) {
                    }
                }
            }
        }
        qWarning() << "Windows loaded";


    } else {
        qWarning() << "ArbitragePair::loadDbWindows()" << q.lastError().text();
    }

}

void ArbitragePair::createNewWindows() {
    bool update_required=false;
    if( !_new_windows.isEmpty()) {
        while( !_new_windows.isEmpty()) {
            const QPair<quint32,quint32> _pairs = _new_windows.dequeue();
            if ( createWindow( id, _pairs ) > 0 ) {
                update_required=true;
            }
        }
    }
    if ( update_required ) {
        loadDbWindows();
    }
}

double ArbitragePair::getSpreadSize() const {
    QList<double> spredpers;
    for( auto it = _windows.begin(); it != _windows.end(); it++ ) {
        if ( _windows[ it.key() ]->isAvailable() ) {
            spredpers.push_back( _windows[ it.key() ]->getSpread() );
        }
    }
    if ( spredpers.isEmpty() ) {
        return 0.0;
    }
    return static_cast<double>( std::accumulate(spredpers.begin(), spredpers.end(), 0.0 ) / spredpers.size() );
}

void ArbitragePair::update(const QSqlRecord &rec) {
    sell_coin_id = rec.value("base_coin_id").toUInt();
    buy_coin_id = rec.value("price_coin_id").toUInt();
    base_currency_name = SwiftLib::getAssets()->getCoinName( sell_coin_id );
    price_currency_name = SwiftLib::getAssets()->getCoinName( buy_coin_id );
}

quint32 ArbitragePair::createWindow(const quint32 &arbitrage_pair_id, const QPair<quint32, quint32> &trade_pairs) {
        const quint32 sell_exch( SwiftLib::getAssets()->getMarketExchangeId( trade_pairs.first ) );
        const quint32 buy_exch( SwiftLib::getAssets()->getMarketExchangeId( trade_pairs.second ) );
        if ( sell_exch == 0 || buy_exch == 0 || !_valid_pairs.contains( trade_pairs.first ) || !_valid_pairs.contains( trade_pairs.second ) ) {
            return 0;
        }

    QSqlQuery q("INSERT INTO windows (sell_exchange,buy_exchange,arbitrage_pair_id,sell_market,buy_market) VALUES ("+QString::number(sell_exch).toUtf8()+","+QString::number(buy_exch).toUtf8()+","+QString::number(arbitrage_pair_id).toUtf8()+","+QString::number(trade_pairs.first).toUtf8()+","+QString::number(trade_pairs.second).toUtf8()+")");
    if ( q.exec() ) {
        const quint32 win_id = q.lastInsertId().toUInt();
        q.finish();
        if ( win_id > 0 ) {
            loadDbWindow( win_id );
            return win_id;
        }
    } else {
        qWarning() << q.lastError().text();
        qWarning() << "Window create failed!";
    }
    loadDbWindow( trade_pairs.first, trade_pairs.second );

    qWarning() << "Windows created";
    return 0;
}

quint64 ArbitragePair::getTotalTicks() {
    quint64 r(1);
    for( auto it = _windows.begin(); it != _windows.end(); it++ ) {
        r += it.value()->getTicksCount();
    }
    return r;
}


QList<QPair<quint32, quint32> > ArbitragePair::getTradePairs() const {
    QList<QPair<quint32,quint32>> _ret;
    for( auto it = _valid_pairs.begin(); it != _valid_pairs.end(); it++ ) {
        for( auto it2 = _valid_pairs.begin(); it2 != _valid_pairs.end(); it2++ ) {
            if ( *it != *it2 ) {
                QPair<quint32,quint32> t( *it, *it2 );
                QPair<quint32,quint32> t2( *it2, *it );

                if ( !_ret.contains( t ) ) {
                    _ret.push_back( t );
                }
                if ( !_ret.contains( t2 ) ) {
                    _ret.push_back( t2 );
                }
            }
        }
    }
    return _ret;
}

void ArbitragePair::setWampSession(Wamp::Session *sess) {
    session = sess;
    emit debug("Arbitrage pair session connected");
    for( auto it = _windows.begin(); it!= _windows.end(); it++) {
        const double bb = getBalanceAvailable( _windows[it.key()]->getBuyCurrency(), true );
        _cached_bals[ it.value()->getBuyCurrency() ] = bb;
        const double sb = getBalanceAvailable(  _windows[it.key()]->getSellCurrency(), true );
        _cached_bals[ it.value()->getSellCurrency() ] = sb;
    }

    emit hasWampSession( session );
}

void ArbitragePair::onErrorOrder(const QJsonObject &j_itm){
    if ( _valid_pairs.contains( j_itm["market_id"].toString().toUInt() ) ) {
        emit errorOrder( j_itm );
    }
}

void ArbitragePair::onCompletedOrder(const QJsonObject &j_itm){
    if ( _valid_pairs.contains( j_itm["market_id"].toString().toUInt() ) ) {
        unlock();
        emit completedOrder( j_itm );
    }
}

void ArbitragePair::onPlacedOrder(const QJsonObject &j_itm){
    if ( _valid_pairs.contains( j_itm["market_id"].toString().toUInt() ) ) {
        emit placedOrder( j_itm );
    }
}

void ArbitragePair::onCanceledOrder(const QJsonObject &j_itm){
    if ( _valid_pairs.contains( j_itm["market_id"].toString().toUInt() ) ) {
        unlock();
        emit canceledOrder( j_itm );
    }

}

void ArbitragePair::checkCandidats() {
    if ( !_classic_candidats.isEmpty() ) {
        // sort by profit percent
        std::sort( _classic_candidats.begin(), _classic_candidats.end(), [&]( const QVariantList& firts, const QVariantList& second ) {
            return _windows[firts.at(0).toUInt()]->getStatProfitPercent() > _windows[second.at(0).toUInt()]->getStatProfitPercent();
        });
        QList<QVariantList> _filtered;
        // remove without start balances
        QList<QVariantList> _cc( _classic_candidats );
        _classic_candidats.clear();
        for( auto it = _cc.begin(); it != _cc.end(); it++ ) {
            if ( _windows[it->at(0).toUInt()]->getSellBalance() >= it->at(1).toDouble() && _windows[it->at(0).toUInt()]->getBuyBalance() >= it->at(1).toDouble() * it->at(3).toDouble() ) {
                _filtered.push_back( *it );
            }
        }
        // If has something
        if ( !_filtered.isEmpty() ) {
            for( auto it = _filtered.begin(); it != _filtered.end(); it++ ) {
                if ( is_locked ) {
                    // Pair is locked
                } else {
                    const quint32 wid = it->at(0).toUInt();

                    const double size = QString::number( it->at(1).toDouble(), 'f', amount_scale ).toDouble();
                    const double s_rate = QString::number( it->at(2).toDouble(), 'f', rate_scale ).toDouble();
                    const double b_rate = QString::number( it->at(3).toDouble(), 'f', rate_scale ).toDouble();

                    const double profit = _windows[wid]->getRealProfit( size );
                    const double minprofit = getManager()->getParam("MIN_PROFIT_SIZE").toDouble();
                    if ( profit > minprofit ) {
                       OrderParams * sellorder = new OrderParams( this );
                       sellorder->setOrderParams( _windows[wid]->sell_pair_id, size,s_rate, 0 );
                       sellorder->arb_window = wid;

                       OrderParams * buyorder = new OrderParams( this );
                       buyorder->setOrderParams( _windows[wid]->buy_pair_id, size, b_rate, 1 );
                       buyorder->arb_window = wid;

                       const QJsonObject sell_params( sellorder->toJson() );
                       const QJsonObject buy_params( buyorder->toJson() );
                       const bool orders_placing = getManager()->getParam("ORDER_PLACING") == "1";
                       if ( session != nullptr && session->isJoined() && orders_placing && !is_locked ) {
                           lock();
                           QString msg1( "Placing order: " + QJsonDocument( sell_params ).toJson( QJsonDocument::Compact ) );
                           QString msg2( "Placing order: " + QJsonDocument( buy_params ).toJson( QJsonDocument::Compact ) );

                           getManager()->linkOrder( sell_params.value("local_id").toString(), wid, size );
                           getManager()->linkOrder( buy_params.value("local_id").toString(), wid, size );
                           qWarning() << "Trying to place orders";
                           emit log( msg1 );
                           emit log( msg2 );
                           session->call( SwiftLib::getRpcPath( sellorder->exchange_id, SwiftLib::WampExchangesRpcMethods::OrderPlace ), QVariantList({ QJsonDocument(  sellorder->toJson() ).toJson( QJsonDocument::Compact ) }));
                           session->call( SwiftLib::getRpcPath( buyorder->exchange_id, SwiftLib::WampExchangesRpcMethods::OrderPlace ), QVariantList({ QJsonDocument(  buyorder->toJson() ).toJson( QJsonDocument::Compact ) }));
                       }
                       sellorder->deleteLater();
                       buyorder->deleteLater();
                    } else {
                        QString msg3( "Profit is too low to use window: " + QString::number( profit,'f',8 ) );
                        emit log( msg3 );
                        qWarning() << profit << " profit is too low";
                    }
                }
            }
        }
        emit log("Processed "+ QString::number( _classic_candidats.count( ) ) + " candidats in arbitrage pair "+ QString::number( id ));
    } else {
        emit debug("No candidats for arbitrage in arbitrage pair "+ QString::number( id ) );
    }
}

void ArbitragePair::sendLog(const QString &msg) {
    if ( session != nullptr && session->isJoined() ) {
        session->call("swift.system.rpc.logs.log", {"Arbitrage","INFO",msg});
    }
}

void ArbitragePair::sendDebug(const QString &msg)
{
    if ( session != nullptr && session->isJoined() ) {
        session->call("swift.system.rpc.logs.log", {"Arbitrage","DEBUG",msg});
    }
}

ArbitrageVariant::ArbitrageVariant(const ArbitrageVariant &other) : profit(other.profit), amount(other.amount), rate_sell(other.rate_sell), rate_buy(other.rate_buy), fee_sell(other.fee_sell), fee_buy(other.fee_buy) {

}

QJsonObject ArbitrageVariant::toJson() {
    QJsonObject j_ret;
    j_ret["profit"] = QString::number( profit, 'f', 8 );
    j_ret["rate_sell"] = QString::number( rate_sell, 'f', 8 );
    j_ret["rate_buy"] = QString::number( rate_buy, 'f', 8 );
    j_ret["fee_sell"] = QString::number( fee_sell, 'f', 8 );
    j_ret["fee_buy"] = QString::number( fee_buy, 'f', 8 );
    j_ret["amount"] = QString::number( amount, 'f', 8 );
    return j_ret;
}

ArbitrageVariant ArbitrageVariant::operator=(const ArbitrageVariant &other)  {
    profit = other.profit;
    rate_sell = other.rate_sell;
    rate_buy = other.rate_buy;
    fee_sell = other.fee_sell;
    fee_buy = other.fee_buy;
    amount = other.amount;
    return *this;
}

ArbitrageVariant::ArbitrageVariant(const double &p, const double &a, const double &rs, const double &rb, const double &fs, const double &fb) :
    profit(p), amount(a), rate_sell(rs), rate_buy(rb), fee_sell(fs), fee_buy(fb) {
}
