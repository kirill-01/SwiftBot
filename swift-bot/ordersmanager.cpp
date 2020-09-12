#include "ordersmanager.h"
#include <stdlib.h>
#include <QElapsedTimer>
#include <algorithm>
#include <QtConcurrent/QtConcurrent>
#include "systemevent.h"
/**
 * @brief OrdersManager::OrdersManager
 * @param parent
 */


OrdersManager::OrdersManager(QObject *parent) : QObject(parent),
    system_mode(4),
    _rate(0),
     session(nullptr),
    assets( SwiftLib::getAssets()  )
{

    _sell_orders_hashes = assets->getCurrentSellOrdersHashes();
    _buy_orders_hashes = assets->getCurrentBuyOrdersHashes();
    QTimer * t = new QTimer();
    t->setInterval(2000);
    connect(t, &QTimer::timeout,[&](){
        const QList<quint32> _m(_locked_markets);
        const QHash<quint32,quint64> _lt( _lock_market_time );
        for( auto it = _m.begin(); it != _m.end(); it++ ) {
            if ( QDateTime::currentSecsSinceEpoch() - _lt.value(*it,0) >= 20 ) {
                _locked_markets.removeAll( *it );
            }
        }
        if ( session != nullptr && session->isJoined() ) {

            system_mode = session->call("swiftbot.system.mode").toUInt();
        }
    });
    t->start();
    QTimer::singleShot(60000, this, &OrdersManager::clearLocks );
}

void OrdersManager::clearLocks() {
    _used_virtual_asks.clear();
    _used_virtual_bids.clear();
    QTimer::singleShot(60000, this, &OrdersManager::clearLocks );
}

void OrdersManager::onPlaceOrder(const quint32 &pair_id, const double &amount, const double &rate, const quint32 &type) {

    QJsonObject sell_order( SwiftLib::createOrderObject( pair_id, type,  amount, rate) );
    const QString pre_order_str( QJsonDocument( sell_order ).toJson( QJsonDocument::Compact) );
    const QString local_id( session->call("swiftbot.worker.orders.register.remote", QVariantList({ pre_order_str })).toString() );

}

/**
 * @brief OrdersManager::onWampSession
 * @param sess
 */
void OrdersManager::onWampSession(Wamp::Session *sess) {
    qWarning() << "Orders manager connected to wamp";
    session = sess;
    session->call("swiftbot.balances.reserve.cancel.all", QVariantList());
    session->subscribe( "swiftbot.system.events.order", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const QJsonObject j_event( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        OrderEvent event( j_event );
        const QString event_name( event.getEventName() );
        const QJsonObject j_order( event.getEventTarget() );
        // placed updated canceled completed
        if ( event_name == "completed" || event_name == "canceled" ) {
            if ( j_order.value("type").toString() == "sell" ) {
                session->call("swiftbot.balances.reserve.cancel", QVariantList({assets->getMarketBaseCurrency( j_order.value("market_id").toString().toUInt() ), j_order.value("amount").toString().toDouble() }));
            } else {
                session->call("swiftbot.balances.reserve.cancel", QVariantList({assets->getMarketPriceCurrency( j_order.value("market_id").toString().toUInt() ), j_order.value("price").toString().toDouble() }));
            }
           if ( _locked_markets.contains( event.getEventTarget().value("market_id").toString().toUInt() ) ) {
               _locked_markets.removeOne( event.getEventTarget().value("market_id").toString().toUInt() );
   //            qWarning() << "Market unlocked";
           }
        }
    });

}

/**
 * @brief OrdersManager::onCandidats
 * @param candidats
 */
void OrdersManager::onCandidats(const QList<ArbitrageCandidat> &candidats) {
    if ( session == nullptr || !session->isJoined() ) {
        return;
    }
    QList<ArbitrageCandidat> _req;
    QList<ArbitrageCandidat> _itms( createOrdersQueue( candidats ) );
    QHash<quint32,double> _b( _balances );

    if ( _itms.isEmpty() ) {
        return;
    }

    for( auto it = _itms.begin(); it != _itms.end(); it++ ) {
        const ArbitrageCandidat candidat( *it );
        const quint32 sell_curr = assets->getMarketBaseCurrency( candidat.sell_pair_id );
        const quint32 buy_curr = assets->getMarketPriceCurrency( candidat.buy_pair_id );
        if ( !_b.contains(sell_curr ) ) {
            _b.insert(sell_curr, session->call("swiftbot.balances.get.available", QVariantList({sell_curr})).toDouble());
        }
        if ( !_b.contains(buy_curr ) ) {
            _b.insert(buy_curr, session->call("swiftbot.balances.get.available", QVariantList({buy_curr})).toDouble());
        }
        const double sell_balance = _b.value(sell_curr,0);
        const double buy_balance = _b.value(buy_curr,0);

        if ( sell_balance> candidat.amount
             && buy_balance > candidat.buy_price )
        {
            if ( SwiftLib::getSettings()->value("IS_EMULATION", 0).toUInt() == 1 ) {
                if ( !_used_virtual_asks.contains( QPair<quint32,double>( candidat.buy_pair_id, candidat.buy_rate) )
                     && !_used_virtual_bids.contains( QPair<quint32,double>( candidat.sell_pair_id, candidat.sell_rate ) )
                     ) {
                    _used_virtual_asks.push_back(  QPair<quint32,double>( candidat.buy_pair_id, candidat.buy_rate) );
                    _used_virtual_bids.push_back(  QPair<quint32,double>( candidat.sell_pair_id, candidat.sell_rate) );
                    session->call("swiftbot.balances.reserve.create", QVariantList({sell_curr, candidat.amount }));
                    _b[sell_curr] -= candidat.amount;
                    session->call("swiftbot.balances.reserve.create", QVariantList({buy_curr, candidat.buy_price }));
                    _b[buy_curr] -= candidat.buy_price;
                    if ( placeOrder( candidat ) ) {
                        qWarning() << "Orders place sended";
                    }
                }
            } else {
                if ( getMode() == 2 || getMode() == 3 ) {
                    session->call("swiftbot.balances.reserve.create", QVariantList({sell_curr, candidat.amount }));
                    _b[sell_curr] -= candidat.amount;
                    session->call("swiftbot.balances.reserve.create", QVariantList({buy_curr, candidat.buy_price }));
                    _b[buy_curr] -= candidat.buy_price;
                    if ( placeOrder( candidat ) ) {
                        qWarning() << "Orders place sended";
                    }
                }
            }

        } else {
            _req.push_back( candidat );
        }
    }

//    qWarning() << timer.elapsed() << "Queue processed";
    // Предварительно запишем, сколько бабла надо на все варианты за вычетом имеющихся
    QHash<quint32,double> parsed_requests = createMoneyRefillRequests( _req, _b );


    QHash<quint32,double> _required( parsed_requests );
    for( auto it = _required.begin(); it != _required.end(); it++ ) {
        session->call("swiftbot.balances.set.requested",QVariantList({it.key(), it.value() }));
    }
 //   qWarning() << timer.elapsed() << "Queue cleared";
}


void OrdersManager::loadCurrencyTotalUsage() {

    QSqlQuery q("SELECT o.pair_id, m.base_currency_id, m.market_currency_id, type, sum(o.amount) as amounts, sum(o.price) as prices  FROM ccfinance.orders o LEFT JOIN markets m ON m.id=o.pair_id WHERE date(o.ts)=curdate() group by o.type, o.pair_id");
    if ( q.exec() ) {
        while ( q.next() ) {
            quint32 base_currency = q.value("base_currency_id").toUInt();
            quint32 market_currency = q.value("market_currency_id").toUInt();
            quint32 type = q.value("type").toUInt();
            if ( type == 1 ) {
                _currency_in_orders[base_currency] += q.value("amounts").toDouble();
                _currency_in_orders[market_currency] -= q.value("prices").toDouble();
            } else {
                _currency_in_orders[base_currency] -= q.value("amounts").toDouble();
                _currency_in_orders[market_currency] += q.value("prices").toDouble();
            }
        }
    } else {
        qWarning() << q.lastError().text();
    }
    q.finish();
}

void OrdersManager::getAllOpenOrders() {
    _open_orders = QJsonArray( QJsonDocument::fromJson( session->call("open.orders.list.get").toString().toUtf8() ).array() );
}

QHash<quint32, double> OrdersManager::createMoneyRefillRequests(const QList<ArbitrageCandidat> &candidats, const QHash<quint32, double> &_balances) {
    QHash<quint32,double> _requsted;
    for( auto it = candidats.begin(); it != candidats.end();it++ ) {
        _requsted[ assets->getMarketBaseCurrency( it->sell_pair_id ) ] += it->amount;
        _requsted[ assets->getMarketPriceCurrency( it->buy_pair_id ) ] += it->buy_price;
    }
    for( auto it = _requsted.begin(); it != _requsted.end();it++ ) {
        _requsted[ it.key() ] -= _balances.value( it.key(),0 );
    }
    for( auto it = _balances.begin(); it != _balances.end();it++ ) {
        _requsted[it.key()] -= it.value();
    }
    return _requsted;

}

QList<ArbitrageCandidat> OrdersManager::createOrdersQueue(const QList<ArbitrageCandidat> &candidats) {
    QHash<QPair<quint32,quint32>, QList<ArbitrageCandidat> > _grouped_candidats;
    QHash<QPair<quint32,quint32>, double> _max_profits;

    QList<ArbitrageCandidat> candidats_sorted( candidats );
    std::sort(candidats_sorted.begin(), candidats_sorted.end(),  [](const ArbitrageCandidat& a, const ArbitrageCandidat& b) {
        return a.profit() > b.profit();
    });

    return candidats_sorted;
    for( auto it = candidats_sorted.begin(); it != candidats_sorted.end();it++ ) {
        _grouped_candidats[ QPair<quint32,quint32>( it->sell_pair_id, it->buy_pair_id ) ].push_back( *it );
        if ( _max_profits[ QPair<quint32,quint32>( it->sell_pair_id, it->buy_pair_id ) ] < it->profit() ) {
            _max_profits[ QPair<quint32,quint32>( it->sell_pair_id, it->buy_pair_id ) ] = it->profit();
        }
    }

    //  std::sort(_grouped_candidats.begin(), _grouped_candidats.end(),  [](const QPair<quint32,quint32> a, const int b) {return a > b; });
    QList<ArbitrageCandidat> __total_filtered;

    QList<double> _pr( _max_profits.values( ));
    std::sort( _pr.begin(), _pr.end(), std::greater<double>() );

    QHash<quint32,QPair<double,double>> _pair_balances;
    QList<quint32> _processed_c;




    for( auto it = _pr.begin(); it != _pr.end(); it++ ) {
        const QPair<quint32,quint32> _group( _max_profits.key( *it ) );
        QList<ArbitrageCandidat> _in_group( _grouped_candidats.value(_group) );
        std::sort(_in_group.begin(), _in_group.end(),  [](const ArbitrageCandidat& a, const ArbitrageCandidat& b) {
            return a.amount > b.amount;
        });
        const ArbitrageCandidat min_amount = _in_group.first();
        const ArbitrageCandidat max_amount = _in_group.last();

        if ( !_used_pairs.contains( min_amount.sell_pair_id ) && !_used_pairs.contains( min_amount.buy_pair_id ) ) {

            const double sell_balance = session->call("swiftbot.balances.get.available", QVariantList({assets->getMarketBaseCurrency(  max_amount.sell_pair_id )})).toDouble();
            const double buy_balance = session->call("swiftbot.balances.get.available", QVariantList({assets->getMarketPriceCurrency( max_amount.buy_pair_id )})).toDouble();

            if (sell_balance > max_amount.amount && buy_balance > max_amount.buy_price ) {
                __total_filtered.push_back( max_amount );
            } else if ( sell_balance > min_amount.amount && buy_balance > min_amount.buy_price ) {
                __total_filtered.push_back( min_amount );
            }
        }
    }

    return __total_filtered;
}

bool OrdersManager::placeOrder(const ArbitrageCandidat& candidat) {
    // Sell order object
    QJsonObject sell_order( SwiftLib::createOrderObject( candidat.sell_pair_id, 0, candidat.amount, candidat.sell_rate ) );
    const QString preorder( QJsonDocument( sell_order ).toJson( QJsonDocument::Compact ) );
    const QString local_id_sell = QCryptographicHash::hash( preorder.toUtf8(), QCryptographicHash::Sha1 ).toHex();
    sell_order["local_id"] = local_id_sell;
    const QString res( QJsonDocument( sell_order ).toJson( QJsonDocument::Compact ) );
  //  qWarning() << "SELL CANDIDAT" << res;

    // Buy order object
    QJsonObject buy_order( SwiftLib::createOrderObject( candidat.buy_pair_id, 1, candidat.amount, candidat.buy_rate ) );
    const QString buy_preorder( QJsonDocument( buy_order ).toJson( QJsonDocument::Compact ) );
    const QString local_id_buy = QCryptographicHash::hash( buy_preorder.toUtf8(), QCryptographicHash::Sha1 ).toHex();
    buy_order["local_id"] = local_id_buy;
    const QString res2( QJsonDocument( buy_order ).toJson( QJsonDocument::Compact ) );
   // qWarning() << "BUY CANDIDAT" << res2;
    QList<quint64> _requests;
    if ( !_locked_markets.contains( candidat.sell_pair_id ) && !_locked_markets.contains( candidat.buy_pair_id )  ) {
        _locked_markets.push_back( candidat.sell_pair_id  );
        _lock_market_time[candidat.sell_pair_id] = QDateTime::currentSecsSinceEpoch();
        _locked_markets.push_back( candidat.buy_pair_id  );
        _lock_market_time[candidat.buy_pair_id] = QDateTime::currentSecsSinceEpoch();
        session->call(  SwiftLib::getRpcPath( sell_order.value("exchange_id").toString().toUInt(), SwiftLib::WampExchangesRpcMethods::OrderPlace ), QVariantList({res}) ).toULongLong();
        session->call(  SwiftLib::getRpcPath( buy_order.value("exchange_id").toString().toUInt(), SwiftLib::WampExchangesRpcMethods::OrderPlace ), QVariantList({res2}) ).toULongLong();

    }
    return true;
}
