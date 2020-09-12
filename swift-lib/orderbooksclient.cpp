#include "orderbooksclient.h"
#include "swiftlib.h"
OrderbooksClient::OrderbooksClient(QObject *parent) : QObject(parent), orderbooks_client(nullptr)
{

}

void OrderbooksClient::onOrderBooks(const QJsonObject &j_books) {
    QHash<quint32, QMap<double,double>> _new_asks, _new_bids;

    const QJsonArray temo_asks( j_books.value("a").toArray() );
    for( auto it = temo_asks.begin(); it != temo_asks.end(); it++ ) {
        const QJsonArray itm( it->toArray() );
        if ( !itm.isEmpty() ) {
            for( auto it1 = itm.begin(); it1 != itm.end(); it1++ ) {
                if ( _filtered_pairs.isEmpty() || _filtered_pairs.contains( it1->toArray().at(0).toString().toInt() ) ) {
                    _new_asks[ it1->toArray().at(0).toString().toInt() ].insert( it1->toArray().at(1).toString().toDouble(), it1->toArray().at(2).toString().toDouble() );
                }
            }
        }
    }
    const QJsonArray temo_bids( j_books.value("b").toArray() );

    for( auto it = temo_bids.begin(); it != temo_bids.end(); it++ ) {
        const QJsonArray itm( it->toArray() );
        if ( !itm.isEmpty() ) {
            for( auto it1 = itm.begin(); it1 != itm.end(); it1++ ) {
                if ( _filtered_pairs.isEmpty() || _filtered_pairs.contains( it1->toArray().at(0).toString().toInt() ) ) {
                    _new_bids[ it1->toArray().at(0).toString().toInt() ].insert( it1->toArray().at(1).toString().toDouble(), it1->toArray().at(2).toString().toDouble() );
                }
            }
        }

    }
    emit orderbooks( _new_asks, _new_bids );
}

void OrderbooksClient::onWampSession(Wamp::Session *sess) {
    session = sess;
    session->subscribe("orderbooks", [&]( const QVariantList&v, const QVariantMap&m ) {
        const QJsonObject j_books( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        onOrderBooks( j_books );
        Q_UNUSED(m)
    });
}

void OrderbooksClient::stopClient() {
    session->stop();
}

void OrderbooksClient::startClient(const QList<quint32> &filter) {
    _filtered_pairs = QList<quint32>( filter );
    orderbooks_client = new WampClient("orderbooks", SwiftLib::getSettings()->value("WAMP_SERVER", "localhost").toString(),8085, false );
    connect( orderbooks_client, &WampClient::clientConnected, this, &OrderbooksClient::onWampSession );
    orderbooks_client->startClient();
}
