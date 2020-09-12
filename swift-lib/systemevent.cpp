#include "systemevent.h"


QStringList SwiftEvent::privateProperties(){ return  QStringList(); }

SwiftEvent::SwiftEvent(const SwiftEvent::EventTypes &type, const QString &event_name, const QJsonObject &j_targer, QObject *parent)
    : QObject( parent ),
      _type(type),
      _name(event_name),
      _stored_target( j_targer ),
      _event_time( QDateTime::currentSecsSinceEpoch() ),
        exchange_id( j_targer.value("exchange_id").toString().toUInt() ){
}

SwiftEvent::SwiftEvent(const QJsonObject &j_event, QObject *parent)
    : QObject(parent),
      _type( getEventType( j_event.value("event").toString() ) ),
      _name( j_event.value("name").toString()),
      _stored_target( j_event.value("target").toObject() ),
      _event_time( j_event.value("time").toString().toULongLong() ),
      exchange_id( j_event.value("target").toObject().value("exchange_id").toString().toUInt() ){

}

QJsonObject SwiftEvent::toJson() {
    QJsonObject j_obj;
    j_obj["event"] = getEventTypeName();
    j_obj["name"] = _name;
    j_obj["time"] = QString::number( _event_time );
    j_obj["target"] = getEventTarget();

    j_obj["exchange_id"] = QString::number( exchange_id );
    return j_obj;
}

QString SwiftEvent::getEventName() const {
    return _name;
}

QString SwiftEvent::toString() {
    const QString _ret( QJsonDocument( toJson() ).toJson( QJsonDocument::Compact ) );
    return _ret;
}

QJsonObject SwiftEvent::getEventTarget() const {
    return _stored_target;
}

SwiftEvent::EventTypes SwiftEvent::getEventType(const QString &str_name) {
    if ( str_name == "order" ) {
        return OrderEvent;
    } else if ( str_name == "balance" ) {
        return BalanceEvent;
    } else if ( str_name == "account" ) {
        return AccountEvent;
    } else if ( str_name == "system" ) {
        return SystemEvent;
    } else if ( str_name == "withdraw" ) {
        return WithdrawEvent;
    } else if ( str_name == "deposit" ) {
        return DepositEvent;
    }
    return CustomEvent;
}

QString SwiftEvent::getEventTypeName() {
    if ( _type == OrderEvent ) {
        return "order";
    } else if ( _type == BalanceEvent ) {
        return "balance";
    } else if ( _type == AccountEvent ) {
        return "account";
    } else if ( _type == SystemEvent ) {
        return "system";
    } else if ( _type == WithdrawEvent ) {
        return "withdraw";
    } else if ( _type == DepositEvent ) {
        return "deposit";
    }
    return "custom";
}

void SwiftEvent::prepareEventTarget() {
    const QStringList _params( privateProperties() );
    const QJsonObject __target( _stored_target );
    QJsonObject _new_target_object;
    for( auto it = _params.begin(); it != _params.end(); it++ ) {
        _new_target_object[ *it ] = __target.value( *it );
    }
    _stored_target = _new_target_object;
}
