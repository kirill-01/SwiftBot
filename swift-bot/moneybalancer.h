#ifndef MONEYBALANCER_H
#define MONEYBALANCER_H

#include <QObject>
#include "../swift-lib/swiftlib.h"
#include <QMutex>
struct Refill {
    enum Status {
        NewRefill,
        Sending,
        HasTransactions,
        Finished,
        Failed,
        Outdated
    };
    quint32 id=0;
    quint32 from_id;
    quint32 to_id;
    double amount;
    quint32 status;
    QString tx1;
    QString tx2;
    double fee;
    QDateTime ts_create;
    QDateTime ts_update;
    quint32 is_manual_start;

    QString getStatusText() const {
        if ( status == 0 ) {
            return "Готов к старту";
        } else if ( status == 1 ) {
            return "Запуск трансфера";
        } else if ( status == 2 ) {
            return "В процессе";
        } else if ( status == 3 ) {
            return "Завершен";
        } else if ( status == 4 ) {
            return "Ошибка запуска";
        } else {
            return "Не актуален";
        }
    }

    QJsonObject json() const {
        AssetsStorage * assets = SwiftLib::getAssets();
        QJsonObject j_itm;
        j_itm["to"] = assets->getCurrencyExchangeName( to_id );
        j_itm["to_address"] = assets->getDepositAddress( to_id );
        j_itm["status_text"] = getStatusText();
        j_itm["from"] = assets->getCurrencyExchangeName( from_id );
        j_itm["amount"] = QString::number( amount, 'f', 8);
        j_itm["id"] = QString::number( id );
        j_itm["fee"] = QString::number( fee, 'f', 8);
        j_itm["status"] = QString::number( status );
        j_itm["is_manual_start"] = QString::number( is_manual_start );
        j_itm["currency"] = assets->getCurrencyName( from_id ).toUpper().replace("XBT", "BTC");
        return j_itm;
    }
    bool insert() {
        QSqlQuery qq("INSERT INTO refills (from_id,to_id,amount,status,fee,is_manual_start) VALUES ("+QString::number(from_id)+","+QString::number(to_id)+","+QString::number(amount, 'f', 8)+",0,"+QString::number(fee, 'f', 8)+","+QString::number(is_manual_start)+")");
         if ( qq.exec() ) {
             id = qq.lastInsertId().toUInt();
             qq.finish();
             return true;
         } else {
            qWarning() << qq.lastError();
            qq.finish();
             return false;
         }
    }

    void load( const quint32& _id ) {
        QSqlQuery qq("SELECT * FROM refills WHERE id="+QString::number( _id ) );
         if ( qq.exec() && qq.next() ) {
             id=qq.value("id").toUInt();
             from_id=qq.value("from_id").toUInt();
             to_id=qq.value("to_id").toUInt();
             amount=qq.value("amount").toDouble();
             status=qq.value("status").toUInt();
             tx1=qq.value("tx1").toString();
             tx2=qq.value("tx2").toString();
             fee=qq.value("fee").toDouble();
             ts_create=qq.value("ts_create").toDateTime();
             ts_update=qq.value("ts_update").toDateTime();
             is_manual_start=qq.value("is_manual_start").toUInt();
         } else {
             qWarning() << qq.lastError().text();
         }
         qq.finish();
    }

    void setActive() {
        if ( id > 0 && status < 2 ) {
            status = 2;
            QSqlQuery qq("UPDATE refills SET status="+QString::number( status )+", tx1='"+tx1+"' WHERE id="+QString::number( id ));
             if ( !qq.exec() ) {
                 qWarning() << qq.lastError();
             }
             qq.finish();
        }
    }
    void setFailed() {
        if ( id > 0 && status < 4 ) {
            status = 4;
            QSqlQuery qq("UPDATE refills SET status="+QString::number( status )+", tx1='"+tx1+"' WHERE id="+QString::number( id ));
             if ( !qq.exec() ) {
                 qWarning() << qq.lastError();
             }
             qq.finish();
        }
    }


};

class MoneyBalancer : public QObject
{
    Q_OBJECT
public:
    explicit MoneyBalancer(QObject *parent = nullptr);

signals:
    void transferCandidats( const QJsonArray& candidats );
    void publishMessage( const QString& topic, const QJsonObject& msg );
public slots:
    void loadData();
    void onWampSession( Wamp::Session * s );
private:
    bool beginWithdraw( const Refill& ref );
   QMutex m;
   AssetsStorage * assets;
   Wamp::Session * session;

   QHash<QString, QHash<quint32, double>> _reqs;
   QHash<quint32, double> _balances;
   QHash<quint32, double> _fees;
   QHash<quint32, double> _min_withdrawals;
   QHash<quint32, quint32> _sended_withdrawals;
   QList<Refill> _refill_records;
};

#endif // MONEYBALANCER_H
