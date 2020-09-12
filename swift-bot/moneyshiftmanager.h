#ifndef MONEYSHIFTMANAGER_H
#define MONEYSHIFTMANAGER_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include "../swift-lib/swiftlib.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QMutex>

class MoneyShiftManager : public QObject
{
    Q_OBJECT
public:
    explicit MoneyShiftManager(QObject *parent = nullptr);
    double getShiftSize() const;
    double getShiftPrice() const;
    double getCorrectionRate() const;
    double getProfitCorrectionRate() const;

    bool isPositionalShift() const;

    bool hasShifted() const;
    QJsonObject getJsonState() const;

signals:
    void hasMoneyShift( const double& amount, const double& min_rate, const bool& is_positive );
    void hasZeroShift();
    void json_state( const QJsonObject& j_data );
    void publishMessage( const QString& topic, const QJsonObject& message );
public slots:
    void loadPeriodOrders();

private:
    QMutex mutex;
    QTimer * t;
    double sell_price;
    double sell_amount;
    double buy_price;
    double buy_amount;
    QHash<quint32,double> _pair_sell_price;
    QHash<quint32,double> _pair_sell_amount;
    QHash<quint32,double> _pair_buy_price;
    QHash<quint32,double> _pair_buy_amount;
    quint32 sell_orders_cnt;
    quint32 buy_orders_cnt;
    quint32 orders_cnt;
};

#endif // MONEYSHIFTMANAGER_H
