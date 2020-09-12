#ifndef ORDERPARAMS_H
#define ORDERPARAMS_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QCryptographicHash>
#include <QDateTime>
#include <QSqlQuery>
#include "swiftlib.h"

class OrderParams : public QObject
{
    Q_OBJECT
public:
    explicit OrderParams(QObject *parent = nullptr);

    void setOrderParams( const quint32 &_market_id, const double& _amount, const double& _rate, const quint32 &_type );

    QString getRateString() const;

    QString getAmountString() const;

    QString getRpcPath() const;

    QString getLocalId() const;

    OrderParams( const QJsonObject& j_obj, QObject * parent = nullptr );
    OrderParams( const QSqlRecord& record, QObject * parent = nullptr );

    bool isValid() const;

    QJsonObject toJson();

    QString local_id;
    QString remote_id;
    double amount;
    double amount_left;
    double rate;
    double fee;
    double price;
    quint32 exchange_id;
    quint32 type;
    quint32 market_id;
    quint32 base_currency_id;
    quint32 price_currency_id;
    quint32 status;
    quint32 arb_window;
signals:

private:
    QString createLocalId();
    QString preHash() const;
};

#endif // ORDERPARAMS_H
