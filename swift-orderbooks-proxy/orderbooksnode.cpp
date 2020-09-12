#include "orderbooksnode.h"
#include <QMessageAuthenticationCode>

OrderbooksNode::OrderbooksNode(QObject *parent) :
    QObject(parent),
    netmanager(nullptr),
    websocket(nullptr),
    _request_interval_timer(nullptr),
    _publish_interval_timer(nullptr)
{
    req_noncer = QDateTime::currentDateTime().toSecsSinceEpoch();
}

void OrderbooksNode::setExchange(const QString &exchange_name, const QString &target_pair, const quint32 &target_pair_id) {
    _exchange_name = exchange_name;
    _target_pair = target_pair;
    _target_pair_id = target_pair_id;
    _request_interval = 0;
    _publish_interval = 0;
    _current_state = NodeCreated;

}

void OrderbooksNode::setExchange(const QString &exchange_name, const QList<QPair<QString, quint32> > &pairs) {
    _exchange_name = exchange_name;
    _request_interval = 0;
    _publish_interval = 0;
    _target_pairs = pairs;
    for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
        _target_pairs_rels.insert( it->first, it->second );
    }
    _current_state = NodeCreated;
}

void OrderbooksNode::stopNode() {

}

void OrderbooksNode::startNode() {
    const QStringList _supported( QString(SUPPORTED_EXCHANGES).split(",") );

    if ( !_supported.contains( _exchange_name ) ) {
        qWarning() << "error node: " << _exchange_name;
        _current_state = NodeShutdown;
    } else {
        _current_state = NodeStarting;
        qWarning() << "starting node: " << _exchange_name;
        initNodeExchange();
    }
}

void OrderbooksNode::onRawOrderbooks(const QJsonArray &a, const QJsonArray &b, const quint32 &custom_pair_id) {
    QMutexLocker lock( &mutex );

    if ( custom_pair_id == 0 ) {
        qWarning() << _exchange_name;
        return;
    }
    const quint64 current_time = QDateTime::currentSecsSinceEpoch();
    for( auto it = a.begin(); it != a.end(); it++ ) {
        double rate =0;
        double amount =0;
        if ( _exchange_name == "cexio" || _exchange_name == "lbank" ) {
            const QJsonArray rec( it->toArray() );
            rate = rec.at(0).toDouble();
            amount = rec.at(1).toDouble();
        } else if ( _exchange_name == "hitbtc" ) {
            const QJsonObject rec( it->toObject() );
            rate = rec.value("price").toString().toDouble();
            amount = rec.value("size").toString().toDouble();
        }
        else if ( _exchange_name == "bittrex" ) {
            const QJsonObject rec( it->toObject() );
            rate = rec.value("Rate").toDouble();
            amount = rec.value("Quantity").toDouble();
        } else if ( _exchange_name == "idcm" || _exchange_name == "bitfinex" || _exchange_name == "exmo"
                    ||  _exchange_name == "lakebtc" || _exchange_name == "livecoin" || _exchange_name == "kraken"
                    || _exchange_name == "binance" || _exchange_name == "kucoin" ) {
            const QJsonArray rec( it->toArray() );
            rate = rec.at(0).toString().toDouble();
            amount = rec.at(1).toString().toDouble();
        } else if (_exchange_name == "zb" || _exchange_name == "huobi" ) {
            const QJsonArray rec( it->toArray() );
            rate = rec.at(0).toDouble();
            amount = rec.at(1).toDouble();
        }


        if ( _asks_indexed.contains( rate ) ) {
            if ( amount == 0 ) {
                _asks_indexed.remove( rate );
            } else {
                _asks_indexed[ rate ] = QJsonArray({ QString::number( custom_pair_id == 0 ? _target_pair_id : custom_pair_id), QString::number( rate, 'f', 8 ), QString::number( amount, 'f', 8 ), QString::number( current_time ) });
            }
        } else {
            if ( amount > 0 ) {
                _asks_indexed.insert( rate, QJsonArray({ QString::number(  custom_pair_id == 0 ? _target_pair_id : custom_pair_id ), QString::number( rate, 'f', 8 ), QString::number( amount, 'f', 8 ), QString::number( current_time ) }) );
            }
        }
    }

    for( auto it = b.begin(); it != b.end(); it++ ) {
        const QJsonArray rec( it->toArray() );
        double rate =0;
        double amount =0;
        if ( _exchange_name == "cexio" || _exchange_name == "lbank" ) {
            const QJsonArray rec( it->toArray() );
            rate = rec.at(0).toDouble();
            amount = rec.at(1).toDouble();
        } else if ( _exchange_name == "hitbtc" ) {
            const QJsonObject rec( it->toObject() );
            rate = rec.value("price").toString().toDouble();
            amount = rec.value("size").toString().toDouble();
        } else if ( _exchange_name == "bittrex" ) {
            const QJsonObject rec( it->toObject() );
            rate = rec.value("Rate").toDouble();
            amount = rec.value("Quantity").toDouble();
        } else if (_exchange_name == "idcm" || _exchange_name == "bitfinex" || _exchange_name == "exmo"
                   ||  _exchange_name == "lakebtc" || _exchange_name == "livecoin" || _exchange_name == "kraken"
                   || _exchange_name == "binance" || _exchange_name == "kucoin" ) {
            const QJsonArray rec( it->toArray() );
            rate = rec.at(0).toString().toDouble();
            amount = rec.at(1).toString().toDouble();
        } else if (_exchange_name == "zb" || _exchange_name == "huobi" ) {
            const QJsonArray rec( it->toArray() );
            rate = rec.at(0).toDouble();
            amount = rec.at(1).toDouble();
        }

        if ( _bids_indexed.contains( rate ) ) {
            if ( amount == 0 ) {
                _bids_indexed.remove( rate );
            } else {
                _bids_indexed[ rate ] = QJsonArray({ QString::number(  custom_pair_id == 0 ? _target_pair_id : custom_pair_id ), QString::number( rate, 'f', 8 ), QString::number( amount, 'f', 8 ), QString::number( current_time ) });
            }
        } else {
             if ( amount > 0 ) {
                _bids_indexed.insert( rate, QJsonArray({ QString::number(  custom_pair_id == 0 ? _target_pair_id : custom_pair_id ), QString::number( rate, 'f', 8 ), QString::number( amount, 'f', 8 ), QString::number( current_time ) }) );
             }
        }
    }

    lock.unlock();

    if ( rebuildIndexes() ) {
        emit orderbooks( getAsks(), getBids() );
    }
}

QMap<double, QJsonArray> OrderbooksNode::getIndexedAsks() const {
    return _asks_indexed;
}

QMap<double, QJsonArray> OrderbooksNode::getIndexedBids() const {
    return _bids_indexed;
}

bool OrderbooksNode::rebuildIndexes() {
    QMutexLocker lock( &mutex );
    const quint64 current_time = QDateTime::currentSecsSinceEpoch();
    QJsonArray _newAsks;
    QJsonArray _newBids;
    bool has_changes = false;
    if ( !_asks_indexed.isEmpty()  ) {
        quint32 used_records=0;
        const QMap<double, QJsonArray> __items( getIndexedAsks() );
        for( auto it = __items.begin(); it != __items.end(); it++ ) {
            const double rate = it.key();
            const quint64 item_time = it.value().at(3).toString().toUInt();
            if( rate > 0 && current_time - item_time <= 5 ) {
                const QJsonArray itm( it.value() );
                if ( RECORDS_LIMIT > used_records ) {
                    _newAsks.push_back( itm );
                    used_records++;
                } else {
                    _asks_indexed.remove( rate );
                }
            }

        }
    }
    if ( !_bids_indexed.isEmpty()  ) {
        quint32 used_records=0;
        const QMap<double, QJsonArray> __items( getIndexedBids() );
        for( auto it = __items.end(); it != __items.begin(); --it ) {
            if ( it == __items.end() ) {
            } else {
                const double rate= it.key();
                const quint64 item_time = it.value().at(3).toString().toUInt();
                if( rate > 0 && current_time - item_time <= 5 ) {
                    const QJsonArray itm( it.value() );
                    if ( RECORDS_LIMIT > used_records ) {
                        _newBids.push_front( itm );
                        used_records++;
                    } else {
                        _bids_indexed.remove( rate );
                    }
                }
            }
        }
    }
    if ( _asks != _newAsks ) {
        has_changes = true;
        _asks = _newAsks;
    }
    if ( _bids != _newBids ) {
        has_changes = true;
        _bids = _newBids;
    }
    return has_changes;
}

QJsonArray OrderbooksNode::getAsks() const {
    return _asks;
}

QJsonArray OrderbooksNode::getBids() const {
    return _bids;
}

void OrderbooksNode::initLakebtcInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
        const QJsonArray j_asks(j_resp.value("asks").toArray() );
        const QJsonArray j_bids( j_resp.value("bids").toArray() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        if ( _target_pairs.isEmpty() ) {
            QUrl url("https://api.LakeBTC.com/api_v2/bcorderbook?symbol="+_target_pair.toUtf8() );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", _target_pair_id );
        } else {
            for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
                QUrl url("https://api.LakeBTC.com/api_v2/bcorderbook?symbol="+it->first.toUtf8() );
                QNetworkRequest request( url );
                QNetworkReply * reply = netmanager->get( request );
                reply->setProperty("pair_id", it->second );
            }
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initLivecoinInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        for( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            const quint32 pair_id( it->second );
            const QString pairname( it->first );
            const QJsonObject jj( QJsonDocument::fromJson( data ).object() );
            if ( jj.contains( pairname ) ) {
                const QJsonObject j_resp( jj.value( pairname ).toObject() );
                const QJsonArray j_asks(j_resp.value("asks").toArray() );
                const QJsonArray j_bids( j_resp.value("bids").toArray() );
                _asks_indexed.clear();
                _bids_indexed.clear();
                onRawOrderbooks( j_asks, j_bids, pair_id );
            }

        }
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        QUrl url("https://api.livecoin.net/exchange/all/order_book");
        QNetworkRequest request( url );
        QNetworkReply * reply = netmanager->get( request );
        reply->setProperty("mark", "livecoin");
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initExmoInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            if ( j_resp.contains( it->first ) ) {
                const QJsonObject j_data( j_resp.value( it->first ).toObject() );
                const QJsonArray j_asks(j_data.value("ask").toArray() );
                const QJsonArray j_bids( j_data.value("bid").toArray() );
                _asks_indexed.clear();
                _bids_indexed.clear();
                onRawOrderbooks( j_asks, j_bids, it->second );
            }
        }
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        QStringList pairs;
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            pairs.push_back( it->first );
        }
        QUrl url("https://api.exmo.com/v1/order_book/?pair="+pairs.join(",").toUtf8()+"&limit=20");
        QNetworkRequest request( url );
        QNetworkReply * reply = netmanager->get( request );
        Q_UNUSED(reply)
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initBitfinexInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const QJsonArray j_records( QJsonDocument::fromJson( data ).array() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        QJsonArray j_asks;
        QJsonArray j_bids;
        //  Trading: if AMOUNT > 0 then bid else ask.
        for( auto it = j_records.begin(); it != j_records.end(); it++ ) {
            const double amount = it->toArray().at(2).toDouble();
            const double rate = it->toArray().at(1).toDouble();
            QJsonArray rec;
            rec.push_back( QString::number( rate,'f',8 ) );
            rec.push_back( QString::number( amount,'f',8 ).replace("-","") );
            if ( amount > 0 ) {
                j_bids.push_back( rec );
            } else {
                j_asks.push_back( rec );
            }

        }
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://api-pub.bitfinex.com/v2/book/"+it->first.toUtf8()+"/R0?len=25" );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initKucoinInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
        const QJsonObject j_data( j_resp.value("data").toObject() );
        const QJsonArray j_asks(j_data.value("asks").toArray() );
        const QJsonArray j_bids( j_data.value("bids").toArray() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://api.kucoin.com/api/v1/market/orderbook/level2_20?symbol="+it->first.toUtf8() );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initZbInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
        const QJsonArray j_asks(j_resp.value("asks").toArray() );
        const QJsonArray j_bids( j_resp.value("bids").toArray() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("http://api.zb.live/data/v1/depth?market="+it->first.toUtf8()+"&size=20" );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}



void OrderbooksNode::initLbankInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const QJsonObject j_Resp( QJsonDocument::fromJson( data ).object() );
        const QJsonArray j_asks( j_Resp.value("asks").toArray() );
        const QJsonArray j_bids( j_Resp.value("bids").toArray() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://api.lbkex.com/v1/depth.do");
            QUrlQuery urlquery;
            urlquery.addQueryItem("symbol", it->first.toUtf8() );
            urlquery.addQueryItem("size", QString::number( RECORDS_LIMIT ) );
            url.setQuery( urlquery );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initLbankWs() {
    qWarning() << "Websocket initiating in LBANK";
    websocket.reset( new QWebSocket() );
    connect( websocket.get(), &QWebSocket::textMessageReceived, [&](const QString &message){
        const QJsonObject j_message( QJsonDocument::fromJson( message.toUtf8() ).object() );
        if ( j_message.value("action").toString() == "ping" ) {
            QJsonObject pong;
            pong["action"] = "pong";
            pong["pong"] = j_message.value("ping").toString();
            const QString r( QJsonDocument( pong ).toJson( QJsonDocument::Compact ) );
            websocket->sendTextMessage(r);
        } else if ( j_message.contains("depth") ) {
            const QString pairname( j_message.value("pair").toString() );
            const quint32 pairid( _target_pairs_rels.value( pairname, 0 ) );
            QJsonArray j_asks( j_message.value("depth").toObject().value("asks").toArray() );
            QJsonArray j_bids( j_message.value("depth").toObject().value("bids").toArray() );
            onRawOrderbooks( j_asks, j_bids, pairid );
        } else {
            qWarning() << message;
        }
    });
    connect( websocket.get(), &QWebSocket::connected, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QJsonObject pong;
            pong["action"] = "subscribe";
            pong["subscribe"] = "depth";
            pong["depth"] = "20";
            pong["pair"] = it->first;
            const QString r( QJsonDocument( pong ).toJson( QJsonDocument::Compact ) );
            websocket->sendTextMessage(r);
        }
    });
    connect( websocket.get(), &QWebSocket::disconnected, [&](){
        qWarning() << "Websocket disconnected";
        websocket->close();
        QTimer::singleShot(2500, this, &OrderbooksNode::initLbankWs );
    });
    websocket->open(QUrl("wss://www.lbkex.net/ws/V2/"));
}

void OrderbooksNode::initBittrexInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
        const QJsonObject response( j_resp.value("result").toObject() );
        QJsonArray j_bids = response.value("buy").toArray();
        QJsonArray j_asks = response.value("sell").toArray();

        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://bittrex.com/api/v1.1/public/getorderbook");
            QUrlQuery urlquery;
            urlquery.addQueryItem("market", it->first.toUtf8() );
            urlquery.addQueryItem("type", "both" );
            url.setQuery( urlquery );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initCexioInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
        QJsonArray j_bids = j_resp.value("bids").toArray();
        QJsonArray j_asks = j_resp.value("asks").toArray();
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        QUrl url("https://cex.io/api/order_book/BTC/USD?depth=20");
        QUrlQuery urlquery;
        url.setQuery( urlquery );
        QNetworkRequest request( url );
        QNetworkReply * reply = netmanager->get( request );
        Q_UNUSED(reply)
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initCexioWs() {
    qWarning() << "Websocket initiating in cexio";
    websocket.reset( new QWebSocket() );
    connect( websocket.get(), &QWebSocket::textMessageReceived, [&](const QString &message){
        const QJsonObject j_message( QJsonDocument::fromJson( message.toUtf8() ).object() );
        const QJsonObject data( j_message.value("data").toObject() );
        if ( j_message.contains( "e") ) {
            const QString e = j_message.value("e").toString();

            if ( e == "auth") {
                if ( j_message.value("ok").toString() == "ok" ) {
                    const quint64 nonc = ++req_noncer;
                    QJsonObject msg;
                    msg["e"] = "order-book-subscribe";
                    msg["oid"] = QString::number( nonc );
                    QJsonObject params;
                    params["pair"] = QJsonArray({"BTC","USD"});
                    params["subscribe"] = true;
                    params["depth"] = 20;
                    msg["data"] = params;
                    websocket->sendTextMessage( QJsonDocument( msg ).toJson( QJsonDocument::Compact ) );
                    _current_state = NodeConnected;
                } else {
                    emit nodeError(WebsocketAuthError);
                    _current_state = NodeError;
                }
            } else if ( e == "ping" ) {
                const quint64 nonc = ++req_noncer;
                QJsonObject msg;
                msg["e"] = "pong";
                msg["oid"] = QString::number( nonc );
                websocket->sendTextMessage( QJsonDocument( msg ).toJson( QJsonDocument::Compact ) );
            } else if ( e == "order-book-subscribe") {
                if ( data.keys().contains( QString("error") ) ) {
                    qWarning() <<  data.value("error").toString();
                } else {
                    QJsonArray j_asks( data.value("asks").toArray() );
                    QJsonArray j_bids( data.value("bids").toArray() );
                    onRawOrderbooks( j_asks, j_bids );
                }
            } else if ( e == "md_update" ) {
                QJsonArray j_asks( data.value("asks").toArray() );
                QJsonArray j_bids( data.value("bids").toArray() );
                onRawOrderbooks( j_asks, j_bids );
            }
        } else {
            qWarning() << message;
        }
    });
    connect( websocket.get(), &QWebSocket::connected, [&](){
        const qint64 ts = QDateTime::currentDateTime().toSecsSinceEpoch();
        QJsonObject _data;
        QJsonObject _msg;
        const quint64 nonce = ++req_noncer;
        _msg["e"] = "auth";
        _msg["oid"] =  QString::number( nonce );
        QByteArray message = QString( QString::number( ts ) + "6rzhjDFU2te6RbSBV1xaYFohkHE" ).toUtf8();
        QMessageAuthenticationCode code(QCryptographicHash::Sha256);
        code.setKey( "bJKMsWe0ftYJGK0F9f6cwWbdc" );
        code.addData(message);
        _data["key"] ="6rzhjDFU2te6RbSBV1xaYFohkHE";
        _data["signature"] = QString( code.result().toHex() );
        _data["timestamp"] = ts;
        _msg["auth"] = _data;
        const QString msg( QJsonDocument( _msg ).toJson( QJsonDocument::Compact) );
        websocket->sendTextMessage( msg );
    });
    connect( websocket.get(), &QWebSocket::disconnected, [&](){
        qWarning() << "Websocket disconnected";
        websocket->close();
        QTimer::singleShot(2500, this, &OrderbooksNode::initCexioWs );
    });
    websocket->open(QUrl("wss://ws.cex.io/ws/"));
}

void OrderbooksNode::initOkCoinInterval() {

}

void OrderbooksNode::initHuobiInterval() {

    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const QJsonObject j_resp( QJsonDocument::fromJson( data ).object().value("tick").toObject() );
        const QJsonArray j_asks(j_resp.value("asks").toArray() );
        const QJsonArray j_bids( j_resp.value("bids").toArray() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://www.huobi.com.ru/api/market/depth?symbol="+it->first.toUtf8()+"&type=step0" );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();

}

void OrderbooksNode::initNodeExchange() {

    _publish_interval = 500;
    _request_interval = 5000;
    if ( _exchange_name == "cexio" ) {
        initCexioInterval();
        initCexioWs();
    } else if ( _exchange_name == "bittrex" ) {
        _request_interval = 1000;
        initBittrexInterval();
    } else if ( _exchange_name == "lbank" ) {
        _request_interval = 3000;
        initLbankInterval();
        initLbankWs();
    }  else if ( _exchange_name == "kucoin" ) {
        initKucoinInterval();
    } else if ( _exchange_name == "binance" ) {
        _request_interval = 1000;
        initBinanceInterval();
        //   initBinanceWs();
    } else if ( _exchange_name == "kraken" ) {
        initKrakenInterval();
        initKrakenWs();
    } else if ( _exchange_name == "livecoin" ) {
        _request_interval = 1000;
        initLivecoinInterval();
        //        initLivecoinWs();
    } else if ( _exchange_name == "lakebtc" ) {
        _request_interval = 1000;
        initLakebtcInterval();
    } else if ( _exchange_name == "exmo" ) {
        _request_interval = 1000;
        initExmoInterval();
    } else if ( _exchange_name == "hitbtc" ) {
        _request_interval = 1000;
        initHitbtcInterval();
    } else if ( _exchange_name == "bitfinex" ) {
        _request_interval = 1000;
        initBitfinexInterval();
    } else if ( _exchange_name == "okcoin" ) {
        _request_interval = 1000;
        initOkCoinInterval();
    } else if ( _exchange_name == "zb" ) {
        _request_interval = 1000;
        initZbInterval();
    } else if ( _exchange_name == "huobi" ) {
        _request_interval = 1000;
        initHuobiInterval();
    } else if ( _exchange_name == "idcm") {
        _request_interval = 1000;
        initIdcmInterval();
    }


  /*  _publish_interval_timer = new QTimer(this);
    connect( _publish_interval_timer, &QTimer::timeout, this, &OrderbooksNode::sendOrderbooks );
    _publish_interval_timer->start();*/
}

void OrderbooksNode::initHitbtcInterval() {

    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        const QJsonObject j_data( QJsonDocument::fromJson( data ).object() );
        const QJsonArray j_asks(j_data.value("ask").toArray() );
        const QJsonArray j_bids( j_data.value("bid").toArray() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://api.hitbtc.com/api/2/public/orderbook/"+it->first.toUtf8() );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initKrakenInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        QJsonObject j_data;
        if (  QJsonDocument::fromJson( data ).object().value("result").toObject().contains("XBTUSDT") ) {
            j_data = QJsonObject( QJsonDocument::fromJson( data ).object().value("result").toObject().value("XBTUSDT").toObject() );
        } else if (  QJsonDocument::fromJson( data ).object().value("result").toObject().contains("ETHUSDT") ) {
            j_data = QJsonObject( QJsonDocument::fromJson( data ).object().value("result").toObject().value("ETHUSDT").toObject() );
        } else {
            reply->deleteLater();
            return;
        }
        const QJsonArray j_asks(j_data.value("asks").toArray() );
        const QJsonArray j_bids( j_data.value("bids").toArray() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://api.kraken.com/0/public/Depth?pair="+it->first.toUtf8()+"&count=15");
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initKrakenWs() {
    qWarning() << "Websocket initiating in kraken";
    websocket.reset( new QWebSocket() );
    last_pong = QDateTime::currentSecsSinceEpoch();
    connect( websocket.get(), &QWebSocket::textMessageReceived, [&](const QString &message){
        const QJsonArray j_message( QJsonDocument::fromJson( message.toUtf8() ).array() );
        const QJsonObject j_message_obj( QJsonDocument::fromJson( message.toUtf8() ).object() );
        if ( j_message_obj.contains("event") ) {
            if ( j_message_obj.value("event").toString() == "pong"  ) {
                last_pong = QDateTime::currentSecsSinceEpoch();
            } else {

            }
        } else {
            const QString pairname( j_message.last().toString() );
            const quint32 pair_id( getPairId( pairname ) );
            const QJsonArray j_asks( j_message.at(1).toObject().contains("as") ? j_message.at(1).toObject().value("as").toArray() : j_message.at(1).toObject().value("a").toArray() );
            const QJsonArray j_bids( j_message.at(1).toObject().contains("bs") ? j_message.at(1).toObject().value("bs").toArray() : j_message.at(1).toObject().value("b").toArray() );
            if ( !j_asks.isEmpty() || !j_bids.isEmpty() ) {
                onRawOrderbooks( j_asks, j_bids, pair_id );
            } else {
                qWarning() << j_message;
            }
        }
    });
    connect( websocket.get(), &QWebSocket::binaryFrameReceived,[&](const QByteArray& a){
        qWarning() << a;
    });
    connect( websocket.get(), &QWebSocket::connected, [&](){
        QJsonObject j_msg;
        j_msg["event"] = "subscribe";
        QJsonArray j_p;
        for( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            j_p.push_back( it->first );
        }
        j_msg["pair"] = j_p;
        QJsonObject js;
        js["name"] = "book";
        j_msg["subscription"] = js;
        const QString msg( QJsonDocument( j_msg ).toJson( QJsonDocument::Compact ) );
        websocket->sendTextMessage( msg );
        websocket->sendTextMessage("{\"event\":\"ping\"}");
    });
    connect( websocket.get(), &QWebSocket::disconnected, [&](){
        qWarning() << "Websocket disconnected on kraken";
        websocket->close();
        QTimer::singleShot(2500, this, &OrderbooksNode::initKrakenWs );
    });
    QTimer *t = new QTimer();
    t->setInterval( 15000 );
    connect( t, &QTimer::timeout, [&](){
        const quint64 les = QDateTime::currentSecsSinceEpoch() - last_pong;
        if ( les >= 360 ) {
            QTimer::singleShot(2500, this, &OrderbooksNode::initKrakenWs );
        } else {
            websocket->sendTextMessage("{\"event\":\"ping\"}");
        }
    });

    t->start();
    websocket->open(QUrl("wss://ws.kraken.com"));
}

void OrderbooksNode::initBinanceInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){
        const QByteArray data( reply->readAll().constData() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        const QJsonObject j_data( QJsonDocument::fromJson( data ).object() );
        const QJsonArray j_asks(j_data.value("asks").toArray() );
        const QJsonArray j_bids( j_data.value("bids").toArray() );
        _asks_indexed.clear();
        _bids_indexed.clear();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://api.binance.com/api/v3/depth");
            QUrlQuery query;
            query.addQueryItem("symbol", it->first.toUtf8() );
            query.addQueryItem("limit", QString::number( 20 ) );
            url.setQuery( query );
            QNetworkRequest request( url );
            QNetworkReply * reply = netmanager->get( request );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}

void OrderbooksNode::initBinanceWs() {
    qWarning() << "Websocket initiating in binance";
    websocket.reset( new QWebSocket() );
    connect( websocket.get(), &QWebSocket::textMessageReceived, [&](const QString &message){
        const QJsonObject j_message( QJsonDocument::fromJson( message.toUtf8() ).object() );
        if ( j_message.contains("asks") || j_message.contains("bids") ) {
            QJsonArray j_asks( j_message.value("asks").toArray() );
            QJsonArray j_bids( j_message.value("bids").toArray() );
            onRawOrderbooks( j_asks, j_bids );
        } else {
            if( j_message.contains("id") && j_message.value("id").toInt() == 1 ) {

            } else if ( j_message.contains("e") && j_message.value("e").toString() == "depthUpdate") {
              const QString pairname( j_message.value("s").toString() );
              const quint32 pair_id( getPairId( pairname ) );
              QJsonArray j_asks( j_message.value("a").toArray() );
              QJsonArray j_bids( j_message.value("b").toArray() );
              onRawOrderbooks( j_asks, j_bids, pair_id );
            } else {
            qWarning() << j_message;
            }
        }
    });

    last_pong = QDateTime::currentSecsSinceEpoch();
    QTimer * t = new QTimer();
    t->setInterval(15000 );
    connect( websocket.get(), &QWebSocket::pong, [&](){
        // websocket->ping();
        last_pong = QDateTime::currentSecsSinceEpoch();
    });
    connect(t, &QTimer::timeout, [&](){
        const quint64 delay = QDateTime::currentSecsSinceEpoch() - last_pong;
        if ( delay >= 180 ) {
                 QTimer::singleShot(2500, this, &OrderbooksNode::initBinanceWs );
        } else {
            websocket->ping();
        }
    });
    t->start();
    connect( websocket.get(), &QWebSocket::binaryMessageReceived, [&](const QByteArray& a ){
       if ( a == "0x9" ) {
           qWarning() << "PING FRAME RECEIVED";
       } else {
           qWarning() << a;
       }
    });
    connect( websocket.get(), &QWebSocket::binaryFrameReceived,[&](const QByteArray& a){
        if ( a == "0x9" ) {
            qWarning() << "PING FRAME RECEIVED";
        } else {
            qWarning() << a;
        }
    });
    connect( websocket.get(), &QWebSocket::connected, [&](){
        QJsonObject j_msg;
        j_msg["method"] = "SUBSCRIBE";
        QJsonArray j_p;
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            j_p.push_back( QString( it->first.toUtf8()+"@depth10@100ms" ) );
        }
        j_msg["params"] = j_p;
        j_msg["id"] = 1;
        const QString msg( QJsonDocument( j_msg ).toJson( QJsonDocument::Compact ) );
        websocket->sendTextMessage( msg );
    });
    connect( websocket.get(), &QWebSocket::disconnected, [&](){
        qWarning() << "Websocket disconnected";
        websocket->close();
        QTimer::singleShot(2500, this, &OrderbooksNode::initBinanceWs );
    });
    websocket->open(QUrl("wss://stream.binance.com:9443/ws"));
}

void OrderbooksNode::initIdcmInterval() {
    netmanager = new QNetworkAccessManager();
    _request_interval_timer = new QTimer();
    _request_interval_timer->setInterval( _request_interval );
    connect( netmanager, &QNetworkAccessManager::finished, [&]( QNetworkReply * reply ){

        const QByteArray data( reply->readAll().constData() );
        const quint32 pair_id( reply->property("pair_id").toUInt() );
        const QJsonObject j_data( QJsonDocument::fromJson( data ).object().value("data").toObject() );
        const QJsonArray j_asks1(j_data.value("asks").toArray() );
        const QJsonArray j_bids1( j_data.value("bids").toArray() );
        QJsonArray j_asks;
        for( auto it = j_asks1.begin(); it != j_asks1.end(); it++ ) {
            QJsonArray ji;
            ji.push_back( QString::number( it->toObject().value("price").toDouble(), 'f', 8 ) );
            ji.push_back( QString::number( it->toObject().value("amount").toDouble(), 'f', 8 ) );
            j_asks.push_back( ji );
        }
        QJsonArray j_bids;
        for( auto it = j_bids1.begin(); it != j_bids1.end(); it++ ) {
            QJsonArray ji;
            ji.push_back( QString::number( it->toObject().value("price").toDouble(), 'f', 8 ) );
            ji.push_back( QString::number( it->toObject().value("amount").toDouble(), 'f', 8 ) );
            j_bids.push_back( ji );
        }
        _asks_indexed[pair_id] = QJsonArray();
        _bids_indexed[pair_id] = QJsonArray();
        onRawOrderbooks( j_asks, j_bids, pair_id );
        reply->deleteLater();
    });
    connect( _request_interval_timer, &QTimer::timeout, [&](){
        for ( auto it = _target_pairs.begin(); it != _target_pairs.end(); it++ ) {
            QUrl url("https://api.IDCM.cc:8323/api/v1/getdepth");
            QJsonObject j_params;
            j_params["Symbol"] = it->first;
            const QString jsonstr( QJsonDocument( j_params ).toJson( QJsonDocument::Compact ) );
            const QString sign( QMessageAuthenticationCode::hash( jsonstr.toUtf8(), "CP9d9dTjVHnKFd0GmEaHdNRUjS3uYbG515I1Z1QoXjTlA743Qe8xuWvOXfUg9olK1hIAuZHlOMKUUwjcO0EJ28B5zU3K6zfzY9jjotyQwNIIcBTqVIs8KBHSGD1AVXeewbVLc0NkrNceqSTX0COo76OnrrCLsYVzytb1G7eUU7GXotsNJ4zi8Q7qmykLo2StSDFWGRhSoUuZpRYXYO1ooCqImLE7NtwSfd5LNv7RMHUem70uzmMz0t4fcWAVd95V", QCryptographicHash::Sha384 ).toBase64() );
            QNetworkRequest request( url );
            request.setRawHeader("X-IDCM-APIKEY","_FsC3qRiH0OYxsMsXgcJ3g");
            request.setRawHeader("X-IDCM-SIGNATURE", sign.toUtf8() );
            request.setRawHeader("X-IDCM-INPUT",jsonstr.toUtf8() );
            request.setRawHeader("Content-type","application/json" );
            QNetworkReply * reply = netmanager->post( request, jsonstr.toUtf8() );
            reply->setProperty("pair_id", it->second );
        }
    });
    _request_interval_timer->start();
}
