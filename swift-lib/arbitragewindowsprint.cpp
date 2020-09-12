#include "arbitragewindowsprint.h"
#include "arbitragewindow.h"
#include "orderparams.h"

double ArbitrageWindowSprint::getRequiredStartAmount() const {
    return start_amount;
}

ArbitrageWindowSprint::ArbitrageWindowSprint(QObject *parent):QObject(parent),is_locked(false),async_uuid_sell_order(0) {

}

ArbitrageWindowSprint::ArbitrageWindowSprint(const quint32 &wid, const double &start_amnt, QObject *parent) : QObject(parent),is_locked(false),async_uuid_sell_order(0) {
    window_id = wid;
    start_amount =  start_amnt;
    create();
}

ArbitrageWindowSprint::ArbitrageWindowSprint(const QSqlRecord &rec, QObject *parent) : QObject(parent),is_locked(false) ,async_uuid_sell_order(0) {
    id = rec.value("id").toUInt();
    window_id = rec.value("window_id").toUInt();
    start_amount  = rec.value("start_amount").toDouble();
    start_rate= rec.value("start_rate").toDouble();
    start_fee= rec.value("start_fee").toDouble();
    start_conversion_fee= rec.value("start_conversion_fee").toDouble();
    start_result= rec.value("start_result").toDouble();
    start_transfered= rec.value("start_transfered").toDouble();
    start_transfer_fee= rec.value("start_transfer_fee").toDouble();
    return_amount= rec.value("return_amount").toDouble();
    return_rate= rec.value("return_rate").toDouble();
    return_fee= rec.value("return_fee").toDouble();
    return_conversion_fee= rec.value("return_conversion_fee").toDouble();
    return_transfer_fee= rec.value("return_transfer_fee").toDouble();
    returned_total= rec.value("returned_total").toDouble();
    created_at = rec.value("created_at").toDateTime();
    started_at= rec.value("started_at").toDateTime();
    transfered_at= rec.value("transfered_at").toDateTime();
    returned_at= rec.value("returned_at").toDateTime();
    completed_at= rec.value("completed_at").toDateTime();
}

QJsonObject ArbitrageWindowSprint::toJson() const {
    QJsonObject j;
    j["window_id"] = QString::number( window_id );
    j["start_amount"] = QString::number( start_amount );
    j["start_amount"] = QString::number( start_amount,'f', 8 );
    return j;
}

bool ArbitrageWindowSprint::placeStartOrder() {
    return false;
}

bool ArbitrageWindowSprint::waitForStartOrderPlace() const {
    return false;
}

bool ArbitrageWindowSprint::waitForReturnOrderPlace() const {
    if ( is_locked ) {
        return false;
    }
    // Starting order
    if ( return_rate == 0 && returned_total == 0 ) {
        return true;
    } else {
        return false;
    }
    // Returning order order
}

double ArbitrageWindowSprint::getStartSize() {
    const double sell_amount = getWindow()->getSellBalance();
    const double buy_amount = getWindow()->getBuyBalance();
 //   const double sell_rate = getWindow()->sell_rate;
    const double buy_rate = getWindow()->buy_rate;
    if ( sell_amount > buy_rate * buy_amount ) {
        if( sell_amount > start_amount ) {
            return buy_rate * buy_amount - ( buy_rate * buy_amount * getWindow()->buy_fee );
        } else {
            return start_amount;
        }
    } else {
        if( sell_amount > start_amount ) {
            return sell_amount;
        } else {
            return start_amount;
        }
    }
}

void ArbitrageWindowSprint::create() {
    QSqlQuery q;
    q.prepare("INSERT INTO `"+SwiftLib::getUserDbName()+"`.`windows_sprints` (`window_id`,`start_amount`,`start_rate`,`start_fee`,`start_conversion_fee`,`start_result`,`start_transfered`,`start_transfer_fee`,`return_amount`,`return_rate`,`return_fee`,`return_conversion_fee`,`return_transfer_fee`,`returned_total`) \
VALUES (:window_id,:start_amount,:start_rate,:start_fee,:start_conversion_fee,:start_result,:start_transfered,:start_transfer_fee,:return_amount,:return_rate,:return_fee,:return_conversion_fee,:return_transfer_fee,:returned_total )");

    q.bindValue(":window_id",window_id);
    q.bindValue(":start_amount",start_amount);
    q.bindValue(":start_rate",0);
    q.bindValue(":start_fee",0);
    q.bindValue(":start_conversion_fee",0);
    q.bindValue(":start_result",0);
    q.bindValue(":start_transfered",0);
    q.bindValue(":start_transfer_fee",0);
    q.bindValue(":return_amount",0);
    q.bindValue(":return_rate",0);
    q.bindValue(":return_fee",0);
    q.bindValue(":return_conversion_fee",0);
    q.bindValue(":return_transfer_fee",0);
    q.bindValue(":returned_total",0);
    if ( q.exec() ) {
        id = q.lastInsertId().toUInt();
    } else {
        qWarning() << q.lastError().text();
        qWarning() << q.executedQuery();
    }
}

void ArbitrageWindowSprint::lockSelf() {
    is_locked = true;
    getWindow()->setParam("self_lock", "wait_order");
}

ArbitrageWindow *ArbitrageWindowSprint::getWindow() {
    return static_cast<ArbitrageWindow*>( parent() );
}

QJsonObject ArbitrageWindowSprint::createStartSellOrder() {
    const quint32 exch_id = getWindow()->sell_exchange_id;
    double fee_size = getWindow()->sell_fee;
    QJsonObject j_order_object;
    const double price = ( start_amount * start_rate );

    j_order_object["exchange_id"] = QString::number( exch_id );
    j_order_object["market_id"] = QString::number( getWindow()->sell_pair_id );
    j_order_object["created_at"] = QString::number(QDateTime::currentMSecsSinceEpoch() );
    j_order_object["status"] = "0";
    j_order_object["price"] = QString::number( price, 'f', 8 );
    j_order_object["rate"] = QString::number( start_rate, 'f', 8 );
    j_order_object["amount"] = QString::number( start_amount, 'f', 8 );
    j_order_object["amount_left"] = QString::number( start_amount, 'f', 8 );
    j_order_object["fee"] = QString::number( fee_size, 'f', 8 );
    j_order_object["type"] = "sell";
    const QString preorder( QJsonDocument( j_order_object ).toJson( QJsonDocument::Compact ) );
    const QString local_id_sell = QCryptographicHash::hash( preorder.toUtf8(), QCryptographicHash::Sha1 ).toHex();
    j_order_object["local_id"] = local_id_sell;
    return j_order_object;
}
