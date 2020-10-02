#include "ordersstorage.h"
#include <QDate>
#include "swiftlib.h"
#include <QDateTime>
#include "arbitragemanager.h"

class OrdersStorageData : public QSharedData
{
public:
    QJsonArray orders_array;

};

QJsonObject OrdersStorage::prepareOrderData(const QJsonObject &j_order) {
    QJsonObject j_ret( j_order );
    j_ret["created_at"] = uniTime( j_order.value("created_at").toString().toULongLong() );
    const quint32 exchange_id = j_ret.contains("exchange_id") ? j_ret.value("exchange_id").toString().toUInt() : SwiftLib::getAssets()->getMarketExchangeId( j_ret.value("market_id").toString().toUInt() );
    const double exchange_fee = SwiftLib::getExchangeFee( exchange_id );
    if ( !j_ret.contains("local_id") ) {
        j_ret["local_id"] = getLocalIdByRemote( j_ret.value("remote_id").toString(), exchange_id );
    }
    const QString local_uid( j_order.value("local_id").toString() );
    j_ret["price"] = QString::number( j_ret.value("amount").toString().toDouble() - j_ret.value("amount_left").toString().toDouble() * j_ret.value("rate").toString().toDouble(), 'f', 8 );
    const double order_fee = j_ret.value("price").toString().toDouble() * exchange_fee;
    const QMap<quint32, QStringList> _win_rels( ArbitrageManager::getWindowOrdersRels() );
    for( auto it = _win_rels.begin(); it != _win_rels.end(); it++ ) {
        if ( it.value().contains( local_uid ) || it.value().contains( j_ret.value("remote_id").toString() )  ) {
            j_ret["arb_window"] = QString::number( it.key() );
        }
    }
    if ( !j_ret.contains("exchange_id") ) {
        j_ret["exchange_id"] = QString::number( exchange_id );
    }
    if ( !j_ret.contains("exchange_name") ) {
        j_ret["exchange_name"] = SwiftLib::getAssets()->getExchangeName( exchange_id );
    }
    j_ret["fee"] = QString::number( order_fee );
    return j_ret;
}

OrdersStorage::OrdersStorage(QObject *parent) : QObject(parent), data(new OrdersStorageData)
{
    QTimer::singleShot( 360000, this, &OrdersStorage::provision );
}

OrdersStorage::OrdersStorage(const OrdersStorage &rhs) : QObject(rhs.parent()), data(rhs.data)
{

}

OrdersStorage &OrdersStorage::operator=(const OrdersStorage &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

OrdersStorage::~OrdersStorage()
{

}

void OrdersStorage::addOrder(const QJsonObject &_order_json) {
    QMutexLocker lock( &m );
    QJsonObject order_json( prepareOrderData( _order_json ) );
    const quint32 newindex = data->orders_array.size();
    const quint32 order_state = order_json.value("status").toString("0").toUInt();
    const QString remote_uid = order_json.value("remote_id").toString("#");
    const QString local_uid = order_json.value("local_id").toString("#");
    if ( remote_uid != "" && remote_uid != "#" && _index_ext_id.contains( remote_uid ) ) {
        updateOrder( order_json );
    } else if ( remote_uid != "" && local_uid != "#" && _index_loc_id.contains( local_uid ) ) {
        updateOrder( order_json );
    } else {
        QJsonObject j_n_o( order_json );
        data->orders_array.push_back( j_n_o );
        _index_ext_id[ remote_uid ] = newindex;
        _index_loc_id[ local_uid ] = newindex;
        if ( !_index_updated_orders.contains( newindex ) ) {
            _index_updated_orders.push_back( newindex );
        }
        if ( order_state <= 1 ) {
            if ( _index_closed_orders.contains( remote_uid ) ) {
                _index_closed_orders.remove( remote_uid );
            }
            _index_open_orders[remote_uid] = newindex;
        } else {
            if ( _index_open_orders.contains( remote_uid ) ) {
                _index_open_orders.remove( remote_uid );
            }
            _index_closed_orders[remote_uid] = newindex;
        }
    }

}

void OrdersStorage::updateOrder(const QJsonObject &_order_json) {
    QJsonObject order_json( prepareOrderData( _order_json ) );
    const QString remote_uid = order_json.value("remote_id").toString("#");
    const QString local_uid = order_json.value("local_id").toString("#");
    QJsonObject old( getOrder(remote_uid, local_uid ) );
    if ( order_json != old ) {
        if ( !_index_updated_orders.contains( _index_ext_id.value( remote_uid ) ) ) {
            _index_updated_orders.push_back( _index_ext_id.value( remote_uid ) );
        }
        data->orders_array.replace( _index_ext_id.value( remote_uid ), order_json  );
    }

    if ( order_json.value("status").toString().toUInt() <= 1 ) {
        if (_index_closed_orders.contains( remote_uid ) ) {
            _index_closed_orders.remove( remote_uid );
        }
        if ( !_index_open_orders.contains( remote_uid ) ) {
            _index_open_orders[remote_uid] = _index_ext_id.value( remote_uid );
        }
    } else {
        if (_index_open_orders.contains( remote_uid ) ) {
            _index_open_orders.remove( remote_uid );
        }
        if ( !_index_closed_orders.contains( remote_uid ) ) {
            _index_closed_orders[remote_uid] = _index_ext_id.value( remote_uid );
        }
    }
}

QJsonObject OrdersStorage::getOrder(const QString &remote_uid, const QString &local_uid) const {
    if ( isExistsRemoteId( remote_uid ) ) {
        return data->orders_array.at( _index_ext_id.value( remote_uid ) ).toObject();
    } else if ( local_uid != "#" &&  local_uid != "" && isExistsLocalId( local_uid ) ) {
        return data->orders_array.at( _index_loc_id.value( local_uid ) ).toObject();
    } else {
        return QJsonObject();
    }
}

QJsonArray OrdersStorage::getOpenOrders() {
    QJsonArray ret;
    for( auto it = _index_open_orders.begin(); it != _index_open_orders.end(); it++ ) {
        QJsonObject j( data->orders_array.at( it.value() ).toObject() );
        if ( !j.isEmpty() ) {
            if ( !j.contains("exchange_name") ) {
                j["exchange_name"] = SwiftLib::getAssets()->getMarketExchangeName( j.value("market_id").toString().toUInt() );
            }
            if ( j.value("status").toString().toUInt() > 1 ) {
                _index_closed_orders[ it.key() ] = it.value();
                _index_open_orders.remove( it.key() );
            } else {
                ret.push_back( j );
            }
        }
    }
    return ret;
}

QJsonArray OrdersStorage::getClosedOrders() {
    QJsonArray ret;
    QStringList _orders_unique;
    for( auto it = _index_closed_orders.begin(); it != _index_closed_orders.end(); it++ ) {
        QJsonObject j( data->orders_array.at( it.value() ).toObject() );
        const QDate d( QDateTime::fromSecsSinceEpoch( j.value("created_at").toString().toULongLong() ).date() );
        if ( !j.isEmpty() && d == QDate::currentDate() ) {
            if ( !_orders_unique.contains( j.value("remote_id").toString() ) ) {
                ret.push_back( j );
                _orders_unique.push_back( j.value("remote_id").toString() );
            }
        }
    }
    return ret;
}

QJsonArray OrdersStorage::getClosedOrdersByPair(const quint32 pair_id) {
    QJsonArray ret;
    QStringList _orders_unique;
    for( auto it = _index_closed_orders.begin(); it != _index_closed_orders.end(); it++ ) {
        QJsonObject j( data->orders_array.at( it.value() ).toObject() );
        if ( j.value("market_id").toString().toUInt() == pair_id ) {
            if ( !_orders_unique.contains( j.value("remote_id").toString() ) ) {
                j["exchange_name"] = SwiftLib::getAssets()->getMarketExchangeName( pair_id );
                ret.push_back( j );
                _orders_unique.push_back( j.value("remote_id").toString() );
            }
        }
    }
    return ret;
}

QJsonArray OrdersStorage::getClosedOrdersByPair(const quint32 pair_id, const QString &date) {
    QJsonArray ret;
    QStringList _orders_unique;
    for( auto it = _index_closed_orders.begin(); it != _index_closed_orders.end(); it++ ) {
        QJsonObject j( data->orders_array.at( it.value() ).toObject() );
        if ( j.value("market_id").toString().toUInt() == pair_id && QDateTime::fromString( date ).date() == QDateTime::fromSecsSinceEpoch( j.value("created_at").toString().toUInt() ).date() ) {
            if ( !_orders_unique.contains( j.value("remote_id").toString() ) ) {
                j["exchange_name"] = SwiftLib::getAssets()->getMarketExchangeName( j.value("market_id").toString().toUInt() );
                ret.push_back( j );
                _orders_unique.push_back( j.value("remote_id").toString() );
            }
        }
    }
    return ret;
}

QJsonArray OrdersStorage::getOopenOrdersByPair(const quint32 pair_id)
{
    QJsonArray ret;
    QStringList _orders_unique;
    for( auto it = _index_open_orders.begin(); it != _index_open_orders.end(); it++ ) {
        QJsonObject j( data->orders_array.at( it.value() ).toObject() );

        if ( j.value("market_id").toString().toUInt() == pair_id ) {
            if ( !_orders_unique.contains( j.value("remote_id").toString() ) ) {

                ret.push_back( j );
                _orders_unique.push_back( j.value("remote_id").toString() );
            }
        }
    }
    return ret;
}

QJsonArray OrdersStorage::getUpdatedOrders() {
    QMutexLocker lock( &m );
    QJsonArray ret;
    for ( auto it = _index_updated_orders.begin(); it != _index_updated_orders.end(); it++ ) {
        const QJsonObject j( data->orders_array.at( *it ).toObject() );
        if ( !j.isEmpty() ) {
            ret.push_back( j );
        }
    }
    _index_updated_orders.clear();
    return  ret;
}

QString OrdersStorage::getLocalIdByRemote(const QString &remote_id, const quint32 &exchange_id) {
    static QHash<QString,QString> _cached;
    if ( _cached.contains( remote_id ) ) {
        return _cached.value( remote_id );
    }
    QSqlQuery q("SELECT * FROM orders_uuids WHERE remote_id='"+remote_id+"'");
    if ( q.exec() ) {
        if( q.next() ) {
            _cached[ remote_id ] = q.value("local_id").toString();
            return q.value("local_id").toString();
        } else {
            QSqlQuery qq("INSERT INTO orders_uuids (exchange_id,remote_id,local_id) VALUES ("+QString::number( exchange_id )+",'"+remote_id+"','"+remote_id+"') ON DUPLICATE KEY UPDATE local_id=VALUES(local_id)");
            if ( !qq.exec() ) {
                qWarning() << qq.lastError().text();
            }
            _cached[ remote_id ] = remote_id;
            return remote_id;
        }
    } else {
        qWarning() << q.lastError().text() << "ERROR RECEIVE LOCAL UID";
    }
    return remote_id;
}

void OrdersStorage::provision() {
    // Поиск дубликатов ордеров по remote_id

    QStringList _open_uids( _index_open_orders.keys() );
    QStringList _closed_uids( _index_closed_orders.keys() );
    if ( !_open_uids.isEmpty() && !_closed_uids.isEmpty() ) {
        for( auto it = _open_uids.begin(); it != _open_uids.end(); it++ ) {
            if ( _closed_uids.contains( *it ) ) {
                _index_open_orders.remove( *it );
            }
        }
    }
    // Проверка консистентности данных
    //for( auto it = data->orders_array.begin(); it != data->orders_array.end(); it++ ) {
    //    const quint32 indx = it.i;
    //    const QJsonObject j_obj( it->toObject() );
    //}

    // Удаление из хранилища не сегодняшних закрытых ордеров

    // Две ссылки на один и тот же индекс
    QTimer::singleShot( 360000, this, &OrdersStorage::provision );
}
