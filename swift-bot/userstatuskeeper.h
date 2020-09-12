#ifndef USERSTATUSKEEPER_H
#define USERSTATUSKEEPER_H

#include <QObject>
#include "wampclient.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


class UserStatusKeeper : public QObject
{
    Q_OBJECT
public:
    explicit UserStatusKeeper(QObject *parent = nullptr);
    double getRate() const;
    Wamp::Session * sess;
signals:
    void userState( const QJsonObject& userstat );
    void balances( const QHash<quint32,double>& bals );
    void publishMessage( const QString& topic, const QJsonObject& msg );
public slots:
    void onWampSession( Wamp::Session * session );
    void sendUserState();
    void onCurrentRate( const double& rate );
    void onOrderPlaced();

private:
    QMutex mutex;
    void saveRateToFile( const double& rate );

    QString getSystemStatus() const;

    QString getCurrency( const quint32& currency_id ) const;
    QJsonArray getNotifies() const;
    double getSummaryUsdBalance();
    double getUserLkBalance( const QString& username );
    double getUserProfitToday();
    double getUserFeeToday();
    quint64 _orders_cnt;
    QTimer * _t;
    QHash<QString, QString> _notifies;
    double _current_rate;
    QDateTime _lastOrderTime;
    QHash<quint32,double> _current_balance;
    QHash<quint32, quint32> _coins_rel;
    QHash<quint32,quint32> _exchanges_currency_rels;
    QHash<quint32,QString> _exchange_names;
    QHash<quint32,QString> _currency_names;

    QHash<quint32, double> _pair_rates;
};

#endif // USERSTATUSKEEPER_H
