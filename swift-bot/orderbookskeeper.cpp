#include "orderbookskeeper.h"
#include "swiftlib.h"

OrderbooksKeeper::OrderbooksKeeper(QObject *parent) : QObject(parent), flush_timer( nullptr )
{

}

void OrderbooksKeeper::onOrderBooks(const QJsonObject& j_books) {
    QMutexLocker lock( &mutex );
    QHash<quint32, QMap<double,double>> _new_asks, _new_bids;

    const QJsonArray temo_asks( j_books.value("a").toArray() );
    for( auto it = temo_asks.begin(); it != temo_asks.end(); it++ ) {

        // const quint32 pair_id( SwiftLib::getAssets()->ge);
        //if ( _available_pairs.contains( it->toArray().first().toArray().first().toString().toUInt() )) {
            const QJsonArray itm( it->toArray() );
            if ( !itm.isEmpty() ) {
                for( auto it1 = itm.begin(); it1 != itm.end(); it1++ ) {
                    if ( it1 == itm.begin() ) {
                       // _asks_timestamps[ it1->toArray().at(0).toString().toInt()] = QDateTime::currentDateTime();
                    }
                    _new_asks[ it1->toArray().at(0).toString().toInt() ].insert( it1->toArray().at(1).toString().toDouble(), it1->toArray().at(2).toString().toDouble() );
                }
            }
        //}
    }
    const QJsonArray temo_bids( j_books.value("b").toArray() );
    for( auto it = temo_bids.begin(); it != temo_bids.end(); it++ ) {
       // if ( _available_pairs.contains( it->toArray().first().toArray().first().toString().toUInt() )) {
            const QJsonArray itm( it->toArray() );
            if ( !itm.isEmpty() ) {
                for( auto it1 = itm.begin(); it1 != itm.end(); it1++ ) {
                    if ( it1 == itm.begin() ) {
                       // _bids_timestamps[ it1->toArray().at(0).toString().toInt()] = QDateTime::currentDateTime();
                    }
                    _new_bids[ it1->toArray().at(0).toString().toInt() ].insert( it1->toArray().at(1).toString().toDouble(), it1->toArray().at(2).toString().toDouble() );
                }
            }
   //     }
    }
    emit orderbooks( _new_asks, _new_bids );
   // emit publishMessage("swift.orderbooks.latest", j_books );
    _asks = _new_asks;
    _bids = _new_bids;
}


void OrderbooksKeeper::flushOrderbooks() {
    if ( !_asks.isEmpty() && !_bids.isEmpty() ) {
        const quint32 valid_seconds = SwiftLib::getSettings()->value("ORDERBOOKS_VALID_TIME", 3).toUInt();
        QHash<quint32, QMap<double,double>> asks( _asks );
        QHash<quint32, QMap<double,double>> valid_asks, valid_bids;
        // Asks filtering by lifetime
        for( auto it = asks.begin(); it != asks.end(); it++ ) {
            const quint32 keeped_duration = static_cast<quint32>(QDateTime::currentSecsSinceEpoch() - _asks_timestamps.value( it.key() ).toSecsSinceEpoch());
            if ( keeped_duration < valid_seconds ) {
                // If is not null amount
                if ( valid_asks[it.key()].count() >= 20 ) {
                    valid_asks[ it.key() ] = it.value();
                } else {
                    valid_asks[ it.key() ].unite( it.value() );
                }
            }
        }
        // Bids filtering by lifetime
        QHash<quint32, QMap<double,double>> bids( _bids );
        for( auto it = bids.begin(); it != bids.end(); it++ ) {
            const quint32 keeped_duration = static_cast<quint32>(QDateTime::currentSecsSinceEpoch() - _bids_timestamps.value( it.key() ).toSecsSinceEpoch());
            if ( keeped_duration < valid_seconds ) {
                if ( valid_bids[it.key()].count() >= 20 ) {
                    valid_bids[ it.key()]= it.value();
                } else {
                    valid_bids[it.key()].unite( it.value() );
                }
            }
        }
        if ( !valid_asks.isEmpty() && !valid_bids.isEmpty() ) {
          //  emit orderbooks( valid_asks, valid_bids );
        }
    }
}

void OrderbooksKeeper::onWampSession(Wamp::Session *session) {

    sess = session;

    qWarning() << "Orderbooks client connected";
    sess->subscribe("orderbooks", [=]( const QVariantList&v, const QVariantMap&m ) {
        const QJsonObject j_books( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        onOrderBooks( j_books );
        Q_UNUSED(m)
    });
}

void OrderbooksKeeper::startKeeper() {
    orderbooks_client = new WampClient(
                "orderbooks",
                SwiftLib::getSettings()->value("WAMP_SERVER", "localhost").toString(),
                SwiftLib::getSettings()->value("WAMP_PORT_ORDERBOOKS_READER", 8085).toInt(), true );
    flush_timer = new QTimer();
    flush_timer->setInterval( static_cast<qint16>( SwiftLib::getSettings()->value("ORDERBOOKS_CHECK_INTERVAL", "500").toUInt() ) );
    connect( flush_timer, &QTimer::timeout, this, &OrderbooksKeeper::flushOrderbooks );
    flush_timer->start();
    connect( orderbooks_client, &WampClient::clientConnected, this, &OrderbooksKeeper::onWampSession );
    orderbooks_client->startClient();
    qWarning() << "Orderbooks reader connecting port"<<SwiftLib::getSettings()->value("WAMP_PORT_ORDERBOOKS_READER", 8085).toInt();
}
