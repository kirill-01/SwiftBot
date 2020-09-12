#ifndef ORDER_H
#define ORDER_H
#include "arbitragecandidat.h"
#include <QObject>
#include "wampclient.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QCryptographicHash>

class Order : public QObject
{
    Q_OBJECT
public:
    enum OrderTypes {
        SellType = 0,
        BuyType
    };
    enum OrderStates {
        NewState = 0,
        PlacingState,
        PlacedState,
        CanceledState,
        ErrorState
    };
    quint32 _sell_order_base_currency;
    quint32 _sell_order_price_currency;
    quint32 _buy_order_base_currency;
    quint32 _buy_order_price_currency;
    explicit Order(QObject *parent = nullptr);
    Order( const ArbitrageCandidat& candidat,  QObject *parent = nullptr );
    bool placeSellOrder( Wamp::Session * session );
    bool placeBuyOrder( Wamp::Session * session );
    bool cancelSellOrder( Wamp::Session * session );
    bool cancelBuyOrder( Wamp::Session * session );
    QString getExchangeName( const quint32& pair_id) const;
    bool saveSellOrder();
    bool saveBuyOrder();
    QString sellOrderHash() const;
    QString buyOrderHash() const;
    quint32 addTransaction( const quint32& currency, const quint32& type, const double& amount );
    double amount() const {
        return _candidat.amount;
    }
    double buyPrice() const {
        return _candidat.buy_price;
    }
signals:

public slots:

private:
    quint32 _sell_order_tx1;
    quint32 _sell_order_tx2;
    quint32 _buy_order_tx1;
    quint32 _buy_order_tx2;
    QString _sell_order_remote_id;
    QString _buy_order_remote_id;
    Order::OrderStates _sell_state;
    Order::OrderStates _buy_state;
    ArbitrageCandidat _candidat;
    double _sell_order_am_left;
    double _buy_order_am_left;
};

#endif // ORDER_H
