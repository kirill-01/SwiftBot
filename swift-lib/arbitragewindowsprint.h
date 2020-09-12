#ifndef ARBITRAGEWINDOWSPRINT_H
#define ARBITRAGEWINDOWSPRINT_H

#include "arbitragepair.h"
#include <QObject>

class ArbitrageWindowSprint : public QObject
{
    Q_OBJECT
public:

    double getRequiredStartAmount() const;

    explicit ArbitrageWindowSprint(QObject *parent = nullptr);
    ArbitrageWindowSprint( const quint32& wid, const double& start_amnt, QObject *parent = nullptr );
    ArbitrageWindowSprint( const QSqlRecord& rec, QObject *parent = nullptr );
    QJsonObject toJson() const;
    bool placeStartOrder();
    bool waitForStartOrderPlace(  ) const;
    bool waitForReturnOrderPlace(  ) const;
    double getStartSize();
    void setStartRate( const double&rate, const double& fee ) {
        start_rate = rate;
        start_fee = fee;
    }
    void orderError( const QJsonObject j_obj ) {
        if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else {

        }
    }
    void orderPlaced( const QJsonObject j_obj ) {
        if ( j_obj.value("local_id").toString() == start_order_local_id ) {
            // start_result = j_obj.value("")
            saveSprintStart();
        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else {

        }
    }
    void orderCanceled( const QJsonObject j_obj ) {
        if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else if ( j_obj.value("local_id").toString() == start_order_local_id ) {

        } else {

        }
    }
    QString start_order_local_id;
    QString start_convert_order_local_id;
    QString return_order_local_id;
    QString return_convert_order_local_id;
public slots:

signals:

private:
    void create();
    void saveSprintStart() {
        QSqlQuery q("UPDATE `"+SwiftLib::getUserDbName()+"`.`windows_sprints` SET start_rate="+QString::number( start_rate,'f', 8 )+", start_result="+QString::number( start_result,'f', 8 )+", started_at=CURRENT_TIMESTAMP() WHERE id="+QString::number( id ) );
        if ( !q.exec() ) {
            qWarning() << "WinSprintStartSave error" << q.lastError().text();
        }
    }
    bool is_locked;
    void lockSelf();
    ArbitrageWindow * getWindow();

    quint64 id;
    quint64 async_uuid_sell_order;
    quint32 window_id;
    double start_amount;
    double start_rate;
    double start_fee;
    double start_conversion_fee;
    double start_result;
    double start_transfered;
    double start_transfer_fee;

    double return_amount;
    double return_rate;
    double return_fee;
    double return_conversion_fee;
    double return_transfer_fee;
    double returned_total;
    QDateTime created_at;
    QDateTime started_at;
    QDateTime transfered_at;
    QDateTime returned_at;
    QDateTime completed_at;
    QJsonObject createStartSellOrder();
};


#endif // ARBITRAGEWINDOWSPRINT_H
