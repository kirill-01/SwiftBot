#include "ordersworker.h"
#include <QtConcurrent/QtConcurrent>
#include "systemevent.h"
#include <QStringList>
#include <QNetworkInterface>
#define APP_VERSION "2.0.2059"
#include <QVector>
#include "arbitragemanager.h"
#include "orderparams.h"
OrdersWorker::OrdersWorker(QObject *parent) : QObject(parent), assets(SwiftLib::getAssets() ), o_storage( new OrdersStorage( this ) )
{
    connect( &socket_in, &QTcpSocket::disconnected, this, &OrdersWorker::startClient );

    //fixWinRelations();
}

OrdersWorker::OrdersWorker(const OrdersWorker &rhs) :  QObject(rhs.parent() ),  assets(SwiftLib::getAssets()), o_storage( new OrdersStorage( this ) )
{


}

OrdersWorker &OrdersWorker::operator=(const OrdersWorker &rhs)
{
    Q_UNUSED(rhs)
    return *this;
}

OrdersWorker::~OrdersWorker()
{

}

QString OrdersWorker::createOrderHash(const QJsonObject &j_order) {
    return QCryptographicHash::hash( QJsonDocument( j_order ).toJson(QJsonDocument::Compact ), QCryptographicHash::Sha1 ).toHex();
}

void OrdersWorker::fixWinRelations() {
    QJsonArray _orders;
    QMap<quint32,QPair<quint32,quint32>> _wins;
    QSqlQuery qq("SELECT * FROM windows");
    if ( qq.exec() ) {
        while( qq.next() ) {
            _wins.insert( qq.value("id").toUInt(), {qq.value("sell_market").toUInt(),qq.value("buy_market").toUInt()});
        }
    }
    QSqlQuery q("SELECT * FROM orders");
    if ( q.exec() ) {
        while ( q.next() ) {
            OrderParams * p =  new OrderParams( q.record(), this );
            _orders.push_back( p->toJson() );
            p->deleteLater();
        }
    }
    QStringList _fixed;
    for( auto it = _orders.begin(); it != _orders.end(); it++ ) {
        const QString ruid_1( it->toObject().value("remote_id").toString() );
        const QString luid_1( it->toObject().value("local_id").toString() );
        const double am_f( it->toObject().value("amount").toString().toDouble() );
        if ( !_fixed.contains( ruid_1 ) &&  it->toObject().value("type").toString() == "sell" ) {
            for( auto it2 = _orders.begin(); it2 != _orders.end(); it2++ ) {
                const QString ruid_2( it2->toObject().value("remote_id").toString() );
                const QString luid_2( it2->toObject().value("local_id").toString() );
                if ( !_fixed.contains( ruid_1 ) && !_fixed.contains( ruid_2 ) && ruid_1 != ruid_2 ) {
                    const double am_s( it->toObject().value("amount").toString().toDouble() );
                    const QPair<quint32,quint32> _p( {it->toObject().value("market_id").toString().toUInt(),it2->toObject().value("market_id").toString().toUInt()} );
                    if ( am_s == am_f && _wins.values().contains( _p ) ) {
                        const quint32 wid = _wins.key( _p );
                        QSqlQuery sq;
                        if ( !sq.exec("UPDATE orders SET arb_window="+QString::number( wid )+" WHERE remote_id='"+ruid_1+"'") ) {
                            qWarning() << sq.lastError().text();
                        }

                        if ( !sq.exec("UPDATE orders SET arb_window="+QString::number( wid )+" WHERE remote_id='"+ruid_2+"'") ) {
                            qWarning() << sq.lastError().text();
                        }
                        ArbitrageManager::linkOrders( luid_1, luid_2, wid );
                        _fixed.push_back( ruid_1 );
                        _fixed.push_back( ruid_2 );
                    }
                }
            }
        }
    }

}

void OrdersWorker::checkUpdates() {
    const QJsonArray j_updated_orders( o_storage->getUpdatedOrders() );
    if ( !j_updated_orders.isEmpty() ) {
        for( auto it = j_updated_orders.begin(); it != j_updated_orders.end(); it++ ) {
            emit saveOrder(  it->toObject() );
        }
        o_storage->clearUpdated();
    }

    QTimer::singleShot( 45000, this, &OrdersWorker::checkUpdates );
}

void OrdersWorker::updateConfig(const QString &conf) {

    QSettings * settings = SwiftLib::getSettings();
    QJsonObject received_json( QJsonDocument::fromJson( conf.toUtf8()).object() );
    QJsonArray j_exch_params( received_json.value("exchanges_params").toArray() );
    QJsonArray j_main_params( received_json.value("main_params").toArray() );

    for( auto it = j_exch_params.begin(); it != j_exch_params.end(); it++ ) {
        QJsonObject j_val( it->toObject() );
        QString exch( j_val.value("exchange").toString( ) );
        QString key( j_val.value("key").toString( ) );
        QString secret( j_val.value("secret").toString( ) );
        QString username( j_val.value("username").toString( ) );
        const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
        const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
        const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
        settings->setValue( keypath, key );
        settings->setValue( secretpath, secret );
        settings->setValue( userpath, username );
    }
    for( auto it = j_main_params.begin(); it != j_main_params.end(); it++ ) {
        QJsonObject j_val( it->toObject() );
        settings->setValue( j_val.value("param").toString(), j_val.value("value").toString() );
    }
    settings->sync();
}

void OrdersWorker::startClient() {
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
    connect( session, &Wamp::Session::started, [&]() {
        qWarning() << "Session started";
        connect( session, &Wamp::Session::joined, this, &OrdersWorker::onWampConnected );
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

void OrdersWorker::checkIndividualOpenState() {
    // TO DO !
    return;
}

void OrdersWorker::onProcessApiCurrent(const QJsonArray &j_items, const quint32& market_privided) {

    Q_UNUSED(market_privided)
    for( auto it = j_items.begin(); it != j_items.end(); it++ ) {

         o_storage->addOrder( it->toObject() );
    }
}

void OrdersWorker::onProcessApiHistory(const QJsonArray &j_items)
{
    if ( SwiftLib::getSettings()->value("API_DEBUG","0").toString() == "1" ) {
        qWarning() << j_items;
    }
    for( auto it = j_items.begin(); it != j_items.end(); it++ ) {
        const QJsonObject j_item( it->toObject( ) );
        o_storage->addOrder( j_item );
    }
}

void OrdersWorker::onErrorOrderEvent(const QJsonObject &j_itm) {
    o_storage->addOrder( j_itm );
}

void OrdersWorker::onCompletedOrderEvent(const QJsonObject &j_itm) {
    if ( j_itm.contains("remote_id") && j_itm.contains("local_id") ) {
        QSqlQuery q("INSERT INTO orders_uuids (exchange_id,remote_id,local_id) VALUES ("+QString::number( j_itm.value("exchange_id").toString().toUInt() )+",'"+j_itm.value("remote_id").toString()+"','"+j_itm.value("local_id").toString()+"') ON DUPLICATE KEY UPDATE local_id=VALUES(local_id)");
        if ( !q.exec() ) {
            qWarning() << q.lastError().text();
        }
    }
    o_storage->addOrder( j_itm );
}

void OrdersWorker::onPlacedOrderEvent(const QJsonObject &j_itm) {
    if ( j_itm.contains("remote_id") && j_itm.contains("local_id") ) {
        QSqlQuery q("INSERT INTO orders_uuids (exchange_id,remote_id,local_id) VALUES ("+QString::number( j_itm.value("exchange_id").toString().toUInt() )+",'"+j_itm.value("remote_id").toString()+"','"+j_itm.value("local_id").toString()+"') ON DUPLICATE KEY UPDATE local_id=VALUES(local_id)");
        if ( !q.exec() ) {
            qWarning() << q.lastError().text();
        }
    }
    o_storage->addOrder( j_itm );
}

void OrdersWorker::onCanceledOrderEvent(const QJsonObject &j_itm){
    o_storage->updateOrder( j_itm );
}

void OrdersWorker::onOrdersFromDb(const QJsonArray &stored_) {
    for ( auto it = stored_.begin(); it != stored_.end(); it++ ) {
        o_storage->addOrder( it->toObject() );
    }
}



void OrdersWorker::onWampConnected(quint64 s ) {
    Q_UNUSED(s)

    QJsonObject msg;
    msg["text"] = "ORDERS_MODULE_STARTED";
    msg["cssClass"] = "ordersuccess";
    msg["closeTimeout"] = "1000";
    msg["position"] = "top";
    session->publishMessage("swiftbot.system.messages", msg);

    connect( this, &OrdersWorker::publishMessage, session, &Wamp::Session::publishMessage );
    session->subscribe( "swiftbot.system.events.order", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const QJsonObject j_event( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        if ( j_event.contains("event") && j_event.value("event").toString() == "order") {
            OrderEvent event( j_event );
            const QString event_name( event.getEventName() );
            const QJsonObject j_order( event.getEventTarget() );
            QJsonObject j_m_o( j_order );
            if ( event_name == "error" ) {
                j_m_o["status"] = "4";
                emit errorOrderEvent( j_m_o );
            } else if ( event_name == "placed" ) {
                emit placedOrderEvent( j_m_o );
            } else if ( event_name == "canceled" ) {
                j_m_o["status"] = "3";
                emit canceledOrderEvent(j_m_o );
            } else if ( event_name == "completed" ) {
                j_m_o["status"] = "2";
                emit completedOrderEvent( j_m_o );
            }
        }
    });

    session->subscribe( SwiftLib::getTopic( SwiftLib::ApiRpcAsyncResults ), [&](const QVariantList &v, const QVariantMap &m) {
        Q_UNUSED(m)
        const QJsonObject j_r( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
            if ( j_r.contains("method") && ( j_r.value("method").toString() == "trade.active" || j_r.value("method").toString() == "trade.history" ) ) {
                if ( j_r.contains("orders") && !j_r.value("orders").toArray().isEmpty() ) {
                    const QJsonArray j_a(j_r.value("orders").toArray() );
                    if ( j_r.value("method").toString() == "trade.active" ) {
                        if ( !j_a.isEmpty() ) {
                            const quint32 marketid( j_a.at(0).toObject().value("market_id").toString().toUInt() );
                            emit processApiCurrent( j_a, marketid );
                        }

                    } else {
                       emit processApiHistory( j_a );
                    }
                }
        }
    });

    session->provide("swiftbot.components.orders.status", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        Q_UNUSED(v)
        QString hostip;
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
                 hostip = address.toString();
        }
        QJsonObject j_ret;
        j_ret["status"] = "running";
        j_ret["name"] = "worker_orders";
        j_ret["location"] = hostip;
        j_ret["version"] = APP_VERSION;
        const QString str(QJsonDocument(j_ret).toJson( QJsonDocument::Compact ) );
        return str;
    });
    session->subscribe("swiftbot.events.settings.valuechanged", [&](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        if ( v.size() == 2 ) {
           SwiftLib::getSettings()->setValue( v.at(0).toString(), v.at(1).toString() );
        }
    });
    session->provide("swiftbot.components.orders.command", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        const QString cmd = v.at(0).toString();
        if ( cmd == "restart") {
            qApp->quit();
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        }
        return true;
    });
    
    session->provide("swiftbot.worker.orders.today", [&](const QVariantList&v, const QVariantMap&m ) {
        QJsonObject _itms;
        _itms["open"] =  o_storage->getOpenOrders();
        _itms["closed"] = o_storage->getClosedOrders();
        Q_UNUSED(v)
        Q_UNUSED(m)
        const QString ret( QJsonDocument(_itms).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->provide("swiftbot.worker.orders.list.open", [&](const QVariantList&v, const QVariantMap&m ) {
        QJsonArray _itms( o_storage->getOpenOrders() );
        Q_UNUSED(v)
        Q_UNUSED(m)
        const QString ret( QJsonDocument(_itms).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->provide("swiftbot.worker.orders.list.closed", [&](const QVariantList& v, const QVariantMap& m) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        QJsonArray _itms( o_storage->getClosedOrders() );
        const QString ret( QJsonDocument(_itms).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->provide("swiftbot.worker.orders.list.open.pair", [&](const QVariantList& v, const QVariantMap& m) {
        Q_UNUSED(m)
         const quint32 pair_id = v.at(0).toUInt();
        QJsonArray _itms( o_storage->getOopenOrdersByPair( pair_id ) );

        const QString ret( QJsonDocument(_itms).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->provide("swiftbot.worker.orders.list.closed.pair", [&](const QVariantList& v, const QVariantMap& m) {
        Q_UNUSED(m)
         const quint32 pair_id = v.at(0).toUInt();
        QJsonArray _itms( o_storage->getClosedOrdersByPair( pair_id ) );

        const QString ret( QJsonDocument(_itms).toJson( QJsonDocument::Compact ) );
        return ret;
    });

    session->provide("swiftbot.worker.orders.count.open.pair", [&](const QVariantList&v, const QVariantMap&m ) {
        QJsonArray _itms( o_storage->getOpenOrders() );
        const quint32 pair_id = v.at(0).toUInt();
        quint32 ii = 0;
        if ( !_itms.isEmpty() ) {
            for( auto it = _itms.begin(); it != _itms.end(); it++ ) {
                if ( it->toObject().value("market_id").toString().toUInt() == pair_id ) {
                    ii++;
                }
            }
        }
        Q_UNUSED(m)
        return ii;
    });



    connect( this, &OrdersWorker::processApiCurrent, this, &OrdersWorker::onProcessApiCurrent );
    connect( this, &OrdersWorker::processApiHistory, this, &OrdersWorker::onProcessApiHistory );

    connect( this, &OrdersWorker::errorOrderEvent, this, &OrdersWorker::onErrorOrderEvent );
    connect( this, &OrdersWorker::canceledOrderEvent, this, &OrdersWorker::onCanceledOrderEvent );
    connect( this, &OrdersWorker::placedOrderEvent, this, &OrdersWorker::onPlacedOrderEvent );
    connect( this, &OrdersWorker::completedOrderEvent, this, &OrdersWorker::onCompletedOrderEvent );

    _update_markets_open_orders_timer = new QTimer(this);
    _update_markets_open_orders_timer->setInterval( 19000 );
    connect( _update_markets_open_orders_timer, &QTimer::timeout, this, &OrdersWorker::getOpenOrders );
    getOpenOrders();
    _update_markets_open_orders_timer->start();
    getCompletedOrders();
    QTimer::singleShot( 12500, this, &OrdersWorker::checkIndividualOpenState );
    QTimer::singleShot( 45000, this, &OrdersWorker::checkUpdates );
   // QTimer::singleShot( 180000, this, &OrdersWorker::fixWinRelations );
}

void OrdersWorker::checkAwaitingUpdates() {

}

void OrdersWorker::getOpenOrders() {
    // TODO: Replace to db request and market_id params when required!
    // const QStringList exchanges( assets->getExchangesNames() );
    const QStringList exchanges( SwiftLib::getSettings()->value("EXCHANGES").toString().split(",") );

    for( auto exch = exchanges.begin(); exch != exchanges.end(); exch++ ) {
            if ( QDateTime::currentSecsSinceEpoch() - _exch_last_requested_all.value( assets->getExchangeId( *exch ), 30 ) >= 15  ) {
                if ( assets->isSeparatedApi( assets->getExchangeId( *exch ) ) ) {
                    const QList<quint32> pairs( assets->getExchangePairs( assets->getExchangeId( *exch ) ) );
                    for( auto it = pairs.begin(); it!= pairs.end(); it++ ) {
                        QJsonObject j_params;
                        j_params["market_id"] = QString::number( *it );
                        const QString params( QJsonDocument( j_params ).toJson( QJsonDocument::Compact ) );
                        const QString call_path =  SwiftLib::getRpcPath( SwiftLib::getAssets()->getExchangeId(*exch), SwiftLib::WampExchangesRpcMethods::TradeOpenOrders );
                        session->call( call_path, {params} ).toULongLong();
                        _exch_last_requested_all[ assets->getExchangeId( *exch ) ] = QDateTime::currentSecsSinceEpoch();
                    }
                } else {
                    const QString call_path =  SwiftLib::getRpcPath( SwiftLib::getAssets()->getExchangeId(*exch), SwiftLib::WampExchangesRpcMethods::TradeOpenOrders );
                    session->call( call_path ).toULongLong();
                    _exch_last_requested_all[ assets->getExchangeId( *exch ) ] = QDateTime::currentSecsSinceEpoch();
                }
            }
    }
}

void OrdersWorker::getCompletedOrders() {
    const QStringList exchanges( assets->getExchangesNames() );
    for( auto exch = exchanges.begin(); exch != exchanges.end(); exch++ ) {
        if ( assets->isSeparatedApi( assets->getExchangeId( *exch ) ) ) {
            const QList<quint32> pairs( assets->getExchangePairs( assets->getExchangeId( *exch ) ) );
            for( auto it = pairs.begin(); it!= pairs.end(); it++ ) {
                QJsonObject j_params;
                j_params["market_id"] = QString::number( *it );
                const QString params( QJsonDocument( j_params ).toJson( QJsonDocument::Compact ) );
                const QString call_path =  SwiftLib::getRpcPath( SwiftLib::getAssets()->getExchangeId(*exch), SwiftLib::WampExchangesRpcMethods::TradeHistory );
                session->call( call_path, {params} ).toULongLong();
                _exch_last_requested_all[ assets->getExchangeId( *exch ) ] = QDateTime::currentSecsSinceEpoch();
            }
        } else {
            session->call( SwiftLib::getRpcPath( SwiftLib::getAssets()->getExchangeId(*exch), SwiftLib::WampExchangesRpcMethods::TradeHistory ) ).toULongLong();
        }
    }
    QTimer::singleShot( 120000, this, &OrdersWorker::getCompletedOrders );
}

QJsonObject OrdersWorker::checkState(const QJsonObject &j_obj, const quint32 &state) const {

    if ( j_obj.value("status").toString().toUInt() != state ) {
        QJsonObject j_corrected( j_obj );
        j_corrected["status"] = QString::number( state );
        if ( j_obj.value("amount_left").toString().toDouble() != 0 ) {
            j_corrected["amount_left"] = QString::number( 0 );
        }
        return j_corrected;
    } else {
        if ( j_obj.value("amount_left").toString().toDouble() != 0 ) {
            QJsonObject j_corrected( j_obj );
            j_corrected["amount_left"] = QString::number( 0 );
            return j_corrected;
        } else {
            return j_obj;
        }
    }
}
