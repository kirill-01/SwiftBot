#ifndef SYSTEMEVENT_H
#define SYSTEMEVENT_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDateTime>



class SwiftEvent : public QObject
{
    Q_OBJECT
public:
    enum EventTypes {
        OrderEvent = 1000,
        BalanceEvent,
        AccountEvent,
        SystemEvent,
        WithdrawEvent,
        DepositEvent,
        CustomEvent
    };

    void setExchangeId( const quint32& eid ) {
        exchange_id = eid;
        _stored_target["exchange_id"] = QString::number( eid );
    }
    virtual QStringList privateProperties();;

    explicit SwiftEvent( const EventTypes& type, const QString& event_name, const QJsonObject& j_targer, QObject *parent = nullptr);

    SwiftEvent( const QJsonObject& j_event, QObject *parent = nullptr );

    QJsonObject toJson();

    QString getEventName() const;
    QString toString();


    QJsonObject getEventTarget() const;
    SwiftEvent::EventTypes getEventType( const QString& str_name );
    QString getEventTypeName();

    void prepareEventTarget();
signals:
private:

    EventTypes _type;
    QString _name;
    QJsonObject _stored_target;
    quint64 _event_time;
    quint32 exchange_id;

};

class DepositEvent : public SwiftEvent {
public:
    DepositEvent( const QJsonObject& j_withdraw_data ) : SwiftEvent( j_withdraw_data ) {

    }
    DepositEvent( const QString& event_name, const QJsonObject& j_withdraw_data ) : SwiftEvent( SwiftEvent::DepositEvent, event_name, j_withdraw_data ) {

    }
};
class WithdrawEvent : public SwiftEvent {
public:
    WithdrawEvent( const QJsonObject& j_withdraw_data ) : SwiftEvent( j_withdraw_data ) {

    }
    WithdrawEvent( const QString& event_name, const QJsonObject& j_withdraw_data ) : SwiftEvent( SwiftEvent::WithdrawEvent, event_name, j_withdraw_data ) {

    }
};

class OrderEvent : public SwiftEvent {
public:
    OrderEvent( const QJsonObject& j_order ) : SwiftEvent( j_order ) {

    }

    OrderEvent( const QString& event_name, const QJsonObject& j_order ) : SwiftEvent( SwiftEvent::OrderEvent, event_name, j_order ) {

    }
    QStringList privateProperties() override {
        return QStringList({"exchange_id","arb_window","market_id","remote_id","local_id","amount","amount_left","type","rate","price","fee","base_currency_id","market_currency_id","status","ts","created_at","closed_at"});
    }
};

#endif // SYSTEMEVENT_H
