#include "dbwriter.h"
#include "swiftlib.h"
#include <QCoreApplication>
DbWriter::DbWriter(QObject *parent) : QObject(parent)
{
    saveOrder = QSqlQuery();
    saveOrder.prepare("INSERT INTO `"+SwiftLib::getUserDbName()+"`.`orders` (local_id,remote_id,amount,amount_left,pair_id,fee,rate,type,status,price,ts,arb_window) VALUES (:local_id,:remote_id,:amount,:amleft,:pair_id,:fee,:rate,:type,:status,:price,:ts,:arb_window) ON DUPLICATE KEY UPDATE `arb_window`=VALUES(`arb_window`),`status`=VALUES(`status`),`rate`=VALUES(`rate`),`amount_left`=VALUES(`amount_left`)");
}

void DbWriter::onSaveOrder(const QJsonObject &jitm)
{
    QMutexLocker lock(&m);

    QJsonObject j_itm( jitm );
    if ( j_itm.value("local_id").isNull() || jitm.value("local_id").toString() == "" ) {
       j_itm["local_id"] = jitm.value("remote_id");
    }
    if ( j_itm.value("price").toString().toDouble() == 0 ) {
        j_itm["price"] = QString::number( ( j_itm.value("amount").toString().toDouble() - j_itm.value("amount_left").toString().toDouble() ) * j_itm.value("rate").toString().toDouble(), 'f', 8);
    }
    if ( !j_itm.contains("remote_id") || j_itm.value("remote_id").toString() == "0" || j_itm.value("remote_id").isNull() ) {
        qWarning() << j_itm;
        return;
    }
    QString dbname( SwiftLib::getSettings()->value("MYSQL_DBNAME").toString().trimmed() );
    const double rate = j_itm.value("rate").toString().toDouble();
    const double amount = j_itm.value("amount").toString().toDouble();
    const double price = rate * amount;
    const double fee = price * SwiftLib::getExchangeFee( SwiftLib::getAssets()->getMarketExchangeId( j_itm.value("market_id").toString().toUInt() ) );

    saveOrder.bindValue(":local_id", j_itm.value("local_id").toString() );
    saveOrder.bindValue(":remote_id", j_itm.value("remote_id").toString() );
    saveOrder.bindValue(":amount", amount );
    saveOrder.bindValue(":amleft", j_itm.value("amount_left").toString().toDouble() );
    saveOrder.bindValue(":pair_id", j_itm.value("market_id").toString().toUInt() );
    saveOrder.bindValue(":fee", fee );
    saveOrder.bindValue(":rate", rate );
    saveOrder.bindValue(":type", j_itm.value("type").toString() == "buy" ? 1 : 0 );
    saveOrder.bindValue(":status", j_itm.value("status").toString().toUInt()  );
    saveOrder.bindValue(":price", price );
    saveOrder.bindValue(":arb_window", j_itm.value("arb_window").toString("0").toUInt() );
    const quint64 ts =  j_itm.value("created_at").toVariant().toULongLong();
    if ( ts > 2000000000 ) {
        saveOrder.bindValue(":ts", QDateTime::fromMSecsSinceEpoch( ts ) ) ;
    } else {
        saveOrder.bindValue(":ts", QDateTime::fromSecsSinceEpoch( ts ) ) ;
    }
    if ( !saveOrder.exec() ) {
        qWarning() << "Error insert" << saveOrder.lastError().text() << j_itm;
        //qApp->exit( 1 );
        return;
    }
}

void DbWriter::onSaveOrderState(const QJsonObject &jitm) {
    QMutexLocker lock(&m);
    QJsonObject j_itm( jitm );
        if ( !_db.isOpen() ) {
            if (!_db.open()) {
                qWarning() << _db.lastError().text();
            }
        }
        QString dbname( SwiftLib::getSettings()->value("MYSQL_DBNAME").toString().trimmed() );
        QSqlQuery saveOrderState;
        saveOrderState.prepare("UPDATE `"+dbname+"`.`orders` SET status=:status, amount_left=:aml, price=:price WHERE remote_id=':rid'");
        saveOrderState.bindValue(":status", QString::number( j_itm.value("status").toString().toUInt() ) );
        saveOrderState.bindValue(":aml", QString::number( j_itm.value("amount_left").toString().toDouble(), 'f',8 ) );
        saveOrderState.bindValue(":price", QString::number( j_itm.value("price").toString().toDouble(), 'f',8 ) );
        saveOrderState.bindValue(":rid", j_itm.value("remote_id").toString() );
        if ( !saveOrderState.exec() ) {
             qWarning() << "Err on ORDER UPDATE";
        }
        saveOrderState.finish();

}

void DbWriter::onThreadStarted() {


   loadTodayOrders();
}

void DbWriter::loadTodayOrders() {
    QSqlQuery q( _db );
     QString dbname;

    QJsonArray j_a;
    if ( q.exec("SELECT * FROM `orders` WHERE status > 1") ) {
        while ( q.next() ) {
            QJsonObject j_o;
            j_o["market_id"] = QString::number( q.value("pair_id").toUInt() );
            j_o["remote_id"] = q.value("remote_id").toString();
            j_o["local_id"] = q.value("local_id").toString();
            j_o["amount"] = QString::number(q.value("amount").toDouble(),'f', 8);
            j_o["amount_left"] = QString::number(q.value("amount_left").toDouble(),'f', 8);
            j_o["type"] = q.value("type").toUInt() == 0 ? "sell" : "buy";
            j_o["rate"] = QString::number(q.value("rate").toDouble(),'f', 8);
            j_o["price"] = QString::number(q.value("price").toDouble(),'f', 8);
            j_o["fee"] = QString::number(q.value("fee").toDouble(),'f', 8);
            j_o["status"] = QString::number( q.value("status").toUInt() );
            j_o["arb_window"] = QString::number( q.value("arb_window").toUInt() );
            j_o["created_at"] = QString::number( q.value("ts").toDateTime().toSecsSinceEpoch() );
            j_o["closed_at"] = q.value("status").toUInt() >= 2 ? QString::number( q.value("ts").toDateTime().toSecsSinceEpoch() ) : QString::number(0);
            j_a.push_back( j_o );
        }
    }
    emit storedOrders( j_a );
}

void DbWriter::deleteOrderRec(const QJsonObject &jitm) {
    QSqlQuery q(_db);
    q.prepare("DELETE FROM orders WHERE remote_id=:rid");
    q.bindValue(":rid", jitm.value("remote_id").toString() );
    if ( !q.exec() ) {
         qWarning() << q.lastError().text() << jitm;
    }

}
