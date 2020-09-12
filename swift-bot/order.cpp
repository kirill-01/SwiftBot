#include "order.h"
#include "ordersmanager.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlError>
#include "swiftlib.h"

Order::Order(QObject *parent) : QObject(parent)
{

}

Order::Order(const ArbitrageCandidat &candidat, QObject *parent)
    : QObject(parent), _candidat(candidat)
{
    QSqlQuery q("SELECT * FROM markets WHERE id IN ("+QString::number( _candidat.sell_pair_id )+", "+QString::number( _candidat.buy_pair_id )+")");
    if ( q.exec() ) {
        while ( q.next() ) {
            if ( _candidat.sell_pair_id == _candidat.buy_pair_id ) {
                _sell_order_base_currency = q.value("base_currency_id").toUInt();
                _sell_order_price_currency = q.value("market_currency_id").toUInt();
                _buy_order_base_currency = q.value("base_currency_id").toUInt();
                _buy_order_price_currency = q.value("market_currency_id").toUInt();
            } else {
                if ( q.value("id").toUInt() == _candidat.sell_pair_id ) {
                    _sell_order_base_currency = q.value("base_currency_id").toUInt();
                    _sell_order_price_currency = q.value("market_currency_id").toUInt();
                } else if ( q.value("id").toUInt() == _candidat.buy_pair_id  ) {
                    _buy_order_base_currency = q.value("base_currency_id").toUInt();
                    _buy_order_price_currency = q.value("market_currency_id").toUInt();
                }
            }
        }
    } else {
        qWarning() << q.lastError().text();
    }

}

bool Order::placeSellOrder(Wamp::Session *session) {
    QJsonObject j_obj;
    j_obj["pair_id"] = QString::number( _candidat.sell_pair_id );
    j_obj["base_currency_id"] =QString::number( _sell_order_base_currency );
    j_obj["market_currency_id"] =QString::number( _sell_order_price_currency );
    j_obj["amount"] = QString::number( _candidat.amount, 'f', 8 );
     _sell_order_am_left = _candidat.amount;
    j_obj["local_id"] = sellOrderHash();
    j_obj["rate"] = QString::number( _candidat.sell_rate, 'f', 8 );
    j_obj["price"] = QString::number( _candidat.sell_price, 'f', 8 );
    j_obj["type"] = "0";
    const QString str_command( QJsonDocument( j_obj ).toJson( QJsonDocument::Compact ) );
    const QString topic( "apicmd."+getExchangeName( _candidat.sell_pair_id )+".placeOrder" );
    const QString res = session->call( topic, QVariantList({ str_command })).toString();
    const QJsonObject j_result( QJsonDocument::fromJson( res.toUtf8() ).object() );
    if ( j_result.value("status").toString().toUInt() == 2 || j_result.value("status").toInt() == 2) {
        _sell_order_am_left =  j_result.value("amount_left").toString().toDouble();
        if ( j_result.value("price").isString() ) {
            _candidat.sell_price = j_result.value("price").toString().toDouble();
        }
        if ( j_result.value("rate").isString() ) {
            _candidat.sell_rate = j_result.value("rate").toString().toDouble();
        }
        _sell_order_remote_id = j_result.value("remote_id").toString();
        _sell_order_tx1 = addTransaction( _sell_order_base_currency, 0, _candidat.amount );
        _sell_order_tx2 = addTransaction( _sell_order_price_currency, 1, _candidat.sell_price );
        _sell_state = Order::OrderStates::PlacedState;
        return true;
    } else {
        _sell_state = Order::OrderStates::ErrorState;
        return false;
    }
}

bool Order::placeBuyOrder(Wamp::Session *session) {
    QJsonObject j_obj;
    j_obj["pair_id"] = QString::number( _candidat.buy_pair_id );
    j_obj["local_id"] = buyOrderHash();
    j_obj["amount"] = QString::number( _candidat.amount, 'f', 8 );
    j_obj["base_currency_id"] =QString::number( _buy_order_base_currency );
    j_obj["market_currency_id"] =QString::number( _buy_order_price_currency );
    _buy_order_am_left = _candidat.amount;
    j_obj["rate"] = QString::number( _candidat.buy_rate, 'f', 8 );
    j_obj["price"] = QString::number( _candidat.buy_price, 'f', 8 );
    j_obj["type"] = "1";
    const QString str_command( QJsonDocument( j_obj ).toJson( QJsonDocument::Compact ) );
    const QString topic( "apicmd."+getExchangeName( _candidat.buy_pair_id )+".placeOrder" );

    const QString res = session->call( topic, QVariantList({ str_command })).toString();
    const QJsonObject j_result( QJsonDocument::fromJson( res.toUtf8() ).object() );

    if ( j_result.value("status").toString().toUInt() == 2 || j_result.value("status").toInt() == 2) {
        _buy_order_am_left =  j_result.value("amount_left").toString().toDouble();
        if ( j_result.value("price").isString() ) {
            _candidat.buy_price = j_result.value("price").toString().toDouble();
        }
        if ( j_result.value("rate").isString() ) {
            _candidat.buy_rate = j_result.value("rate").toString().toDouble();
        }
        _buy_order_remote_id = j_result.value("remote_id").toString();
        _buy_order_tx1 = addTransaction( _buy_order_base_currency, 1, _candidat.amount );
        _buy_order_tx2 = addTransaction( _buy_order_price_currency, 0, _candidat.buy_price );
        _buy_state = Order::OrderStates::PlacedState;
        return true;
    } else {
        _buy_state = Order::OrderStates::ErrorState;
        return false;
    }
}

bool Order::cancelSellOrder(Wamp::Session *session) {
    QJsonObject j_obj;
    j_obj["remote_id"] = _sell_order_remote_id;
    const QString str_command( QJsonDocument( j_obj ).toJson( QJsonDocument::Compact ) );
    const QString topic( "apicmd."+getExchangeName( _candidat.sell_pair_id )+".cancelOrder" );
    if ( session != nullptr && session->isJoined() ) {
        const QString result = session->call( topic, QVariantList({str_command })).toString();
        QJsonObject j_ret( QJsonDocument::fromJson( result.toUtf8() ).object() );
        if ( j_ret["status"].toString().toUInt() >= 3 ) {
            return true;
        } else {
            return false;
        }
    }
    return false;

}

bool Order::cancelBuyOrder(Wamp::Session *session) {
    QJsonObject j_obj;
    j_obj["remote_id"] = _buy_order_remote_id;
    const QString str_command( QJsonDocument( j_obj ).toJson( QJsonDocument::Compact ) );
    const QString topic( "apicmd."+getExchangeName( _candidat.buy_pair_id )+".cancelOrder" );
    if ( session != nullptr && session->isJoined() ) {
        const QString result = session->call( topic, QVariantList({str_command })).toString();
        QJsonObject j_ret( QJsonDocument::fromJson( result.toUtf8() ).object() );
        if ( j_ret["status"].toString().toUInt() >= 3 ) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

QString Order::getExchangeName( const quint32& pair_id ) const {
    return SwiftLib::getAssets()->getMarketExchangeName( pair_id  );
}

bool Order::saveSellOrder() {
    const QString sellhash ( sellOrderHash() );
    const QString buyhash ( buyOrderHash() );
    QString str_query("INSERT INTO `orders` (`local_id`,`remote_id`,`amount`,`amount_left`,`pair_id`,`fee`,`rate`,`type`,`status`,`price`,`tx1`,`tx2`) VALUES (':local_id',':remote_id',:amount,:amleft,:pair_id,:fee,:rate, :type,:status,:price,:tx1,:tx2) ON DUPLICATE KEY UPDATE status=VALUES(status),remote_id=VALUES(remote_id),rate=VALUES(rate),amount_left=VALUES(amount_left), price=VALUES(price)");
    str_query.replace(":local_id", sellhash );
    str_query.replace(":remote_id", _sell_order_remote_id );
    str_query.replace(":amount", QString::number( _candidat.amount, 'f', 8 ) );
    str_query.replace(":amleft", QString::number( _sell_order_am_left, 'f', 8 ));
    str_query.replace(":pair_id", QString::number( _candidat.sell_pair_id ) );
    str_query.replace(":fee", QString::number( _candidat.sell_fee , 'f', 8 ) );
    str_query.replace(":type", QString::number( static_cast<quint32>( Order::OrderTypes::SellType ) ) );
    str_query.replace(":status", QString::number( static_cast<quint32>( _sell_state ) ));
    str_query.replace(":price", QString::number( _candidat.sell_price , 'f', 8 ));
    str_query.replace(":rate", QString::number( _candidat.sell_rate , 'f', 8 ));
    str_query.replace(":tx1", QString::number( _sell_order_tx1 ) );
    str_query.replace(":tx2", QString::number( _sell_order_tx2 ) );

    QSqlQuery q;
    if ( q.exec(str_query) ) {
        q.finish();
        return true;
    } else {
        q.finish();
        qWarning() << q.lastError().text();
        return false;
    }
}

bool Order::saveBuyOrder() {
    const QString sellhash ( sellOrderHash() );
    const QString buyhash ( buyOrderHash() );
    QString str_query("INSERT INTO `orders` (`local_id`,`remote_id`,`amount`,`amount_left`,`pair_id`,`fee`,`rate`,`type`,`status`,`price`,`tx1`,`tx2`) VALUES (':local_id',':remote_id',:amount,:amleft,:pair_id,:fee,:rate, :type,:status,:price,:tx1,:tx2) ON DUPLICATE KEY UPDATE status=VALUES(status),remote_id=VALUES(remote_id),rate=VALUES(rate),amount_left=VALUES(amount_left), price=VALUES(price)");
    str_query.replace(":local_id", buyhash );
    str_query.replace(":remote_id", _buy_order_remote_id );
    str_query.replace(":amount", QString::number( _candidat.amount, 'f', 8 ) );
    str_query.replace(":amleft", QString::number( _buy_order_am_left, 'f', 8 ) );
    str_query.replace(":pair_id", QString::number( _candidat.buy_pair_id ) );
    str_query.replace(":fee", QString::number( _candidat.buy_fee , 'f', 8 ) );
    str_query.replace(":type", QString::number( static_cast<quint32>( Order::OrderTypes::BuyType ) ) );
    str_query.replace(":status", QString::number( static_cast<quint32>( _buy_state ) ) );
    str_query.replace(":price", QString::number( _candidat.buy_price , 'f', 8 ) );
    str_query.replace(":rate", QString::number( _candidat.buy_rate , 'f', 8 ) );
    str_query.replace(":tx1", QString::number( _buy_order_tx1 ) );
    str_query.replace(":tx2", QString::number( _buy_order_tx2 ) );

    QSqlQuery q;
    if ( q.exec(str_query) ) {
        q.finish();
        return true;
    } else {
        q.finish();
        qWarning() << q.lastError().text();
        return false;
    }
}

QString Order::sellOrderHash() const {
    QStringList dat({ QString::number(_candidat.buy_rate, 'f', 8), QString::number(_candidat.sell_fee, 'f', 8), QString::number(_candidat.amount, 'f', 8), QString::number(_candidat.sell_rate, 'f', 8), QString::number(_candidat.sell_pair_id ), "sell" } );
    return QCryptographicHash::hash( dat.join(".").toUtf8(), QCryptographicHash::Md5 ).toHex().toUpper();
}

QString Order::buyOrderHash() const {
    QStringList dat({ QString::number(_candidat.sell_rate, 'f', 8), QString::number(_candidat.buy_fee, 'f', 8), QString::number(_candidat.amount, 'f', 8), QString::number(_candidat.buy_rate, 'f', 8), QString::number(_candidat.buy_pair_id ), "buy" } );
    return QCryptographicHash::hash( dat.join(".").toUtf8(), QCryptographicHash::Md5 ).toHex().toUpper();
}

quint32 Order::addTransaction(const quint32 &currency, const quint32 &type, const double &amount) {
    if ( SwiftLib::getSettings()->value("API_DB_BALANCES=0").toUInt() == 1 )  {
        QString query_str("INSERT INTO _money_transactions (`type`,`account`,`amount`,`hash`) VALUES (:type,:account,:amount,':hash')");
        query_str.replace(":type", QString::number( type ) );
        query_str.replace(":account", QString::number( currency ) );
        query_str.replace(":amount", QString::number( amount, 'f', 8 ) );
        QStringList hash({QString::number( amount, 'f', 8 ), QString::number( type ), QString::number( currency ), QString::number( QDateTime::currentDateTime().toSecsSinceEpoch()) });
        query_str.replace(":hash", QCryptographicHash::hash( hash.join(".").toUtf8(), QCryptographicHash::Md5 ).toHex().toUpper() );
        QSqlQuery q;
        if( q.exec(query_str) ) {
            return q.lastInsertId().toUInt();
        } else {
            qWarning() << q.lastError().text();
            return 0;
        }
    } else {
        return 0;
    }
}
