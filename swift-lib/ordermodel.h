#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include <QObject>

class OrderModel : public QObject
{
    Q_OBJECT
public:
    explicit OrderModel(QObject *parent = nullptr);

signals:

private:
    QString local_uuid;
    QString remote_uuid;
    double amount;
    double amount_left;
    double fee_size;
    double fee_amount;
    double price;
    quint32 market_id;
    quint32 exchange_id;
    quint32 status;
    quint32 type;
    quint64 created_at;
    quint64 closed_at;

};

#endif // ORDERMODEL_H
