#include "userstatuskeeper.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariantList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlDatabase>
#include "swiftlib.h"
#define APP_VERSION "2.0.1885"
UserStatusKeeper::UserStatusKeeper(QObject *parent) : QObject(parent), _orders_cnt(0), _t( new QTimer(this ) ),_current_rate(0)
{
    _t->setInterval( 7500 );
    connect( _t, &QTimer::timeout, this, &UserStatusKeeper::sendUserState );
    _t->start();
    QSqlQuery q( SwiftLib::createQuery("userstatus") );
    QString dbname;
    if ( SwiftLib::getSettings()->value("IS_EMULATION", 0 ).toUInt() == 1 ) {
         dbname = SwiftLib::getSettings()->value("MYSQL_DBNAME").toString()+"_test";
    } else {
        dbname =  SwiftLib::getSettings()->value("MYSQL_DBNAME").toString();
    }

    if( q.exec("SELECT count(*) as cnt FROM `"+dbname+"`.`orders` WHERE status=2 AND date(ts)=curdate()") && q.next() ) {
        _orders_cnt = q.value("cnt").toUInt();
    }
    q.finish();
}

double UserStatusKeeper::getRate() const {
    return _current_rate;
}

void UserStatusKeeper::onWampSession(Wamp::Session *session) {
    sess = session;
}


void UserStatusKeeper::sendUserState() {
     QString dbname;
     if ( SwiftLib::getSettings()->value("IS_EMULATION", 0 ).toUInt() == 1 ) {
          dbname = SwiftLib::getSettings()->value("MYSQL_DBNAME").toString()+"_test";
     } else {
         dbname =  SwiftLib::getSettings()->value("MYSQL_DBNAME").toString();
     }
    QSqlQuery q( SwiftLib::createQuery("userstatus") );
    if( q.exec("SELECT count(*) as cnt FROM `"+dbname+"`.`orders` WHERE status=2 AND date(ts)=curdate()") && q.next() ) {
        _orders_cnt = q.value("cnt").toUInt();
        if ( sess != nullptr && sess->isJoined() ) {
            sess->call("swiftbot.balances.set.status.orders", QVariantList({_orders_cnt}));
        }
    }
    q.finish();
    QJsonObject j_data;
    QString ver(APP_VERSION);
    const QString& username ( SwiftLib::getSettings()->value("USERNAME").toString() );
    const double profit = getUserProfitToday();
    const double fee =  profit / 100 * SwiftLib::getSettings()->value("USERFEE", 50 ).toUInt();
    j_data["version"] = ver;
    j_data["profit_today"] = QString::number( profit, 'f', 8 );
    j_data["fee_payment_today"] = QString::number(fee, 'f', 8 );
    j_data["lk_balance"] = QString::number( getUserLkBalance(username), 'f', 8 );
    j_data["orders_cnt"] = QString::number( _orders_cnt );
    emit publishMessage( "swiftbot.user.status",  j_data );
}

void UserStatusKeeper::onCurrentRate(const double &rate) {
    _current_rate = rate;
}

void UserStatusKeeper::onOrderPlaced() {
    _orders_cnt++;
    _orders_cnt++;

    _lastOrderTime = QDateTime::currentDateTime();
}



QString UserStatusKeeper::getSystemStatus() const {
    if ( SwiftLib::isSystemActive() ) {
        return  "<span class=\"text-color-green\">Работает</span>";
    } else {
        return "<span class=\"text-color-red\">Пауза</span>";
    }
}

QString UserStatusKeeper::getCurrency(const quint32 &currency_id) const {
    if ( _currency_names.contains( currency_id ) ) {
        return _currency_names.value( currency_id );
    } else {
        if ( _coins_rel[currency_id] == 7 ) {
            return "usd";
        } else if ( _coins_rel[currency_id] == 1 ) {
            return "xbt";
        } else if ( _coins_rel[currency_id] == 2 ) {
            return "eth";
        } else {
            return "???";
        }
    }
}

QJsonArray UserStatusKeeper::getNotifies() const {
    QJsonArray j_ret;
    for ( auto it = _notifies.begin(); it != _notifies.end(); it++ ) {
        QJsonObject j_item;
        j_item[ "text" ] = it.key();
        j_item[ "value" ] = it.value();
        j_ret.push_back( j_item );
    }
    return j_ret;
}

double UserStatusKeeper::getSummaryUsdBalance()  {

    if ( sess == nullptr || !sess->isJoined() ) { return 0; }
    const QString bb = sess->call("swiftbot.balances.get.all", QVariantList() ).toString();
    const double current_rate = sess->call("swiftbot.balances.get.rate.btc.usd", QVariantList()).toDouble();

    const QJsonArray j_a_b( QJsonDocument::fromJson( bb.toUtf8() ).array() );
    QHash<quint32,double> _b;

    for( auto it = j_a_b.begin(); it != j_a_b.end(); it++ ) {
        _b[ it->toObject().value("currency_id").toString().toUInt()] = it->toObject().value("total").toString().toDouble();
    }

    double total_usd = 0;
    for( auto it = _b.begin(); it != _b.end(); it++ ) {
        const quint32 coin_id = _coins_rel.value( it.key(), 0 );
        if ( coin_id == 1 ) {
            if ( current_rate <= 0 ) {
                qWarning() << "Zero rate";
            }
            total_usd += it.value() * current_rate;
        } else if ( coin_id == 7 ) {
            total_usd += it.value();
        } else {

        }
    }
    return total_usd;
}

double UserStatusKeeper::getUserLkBalance(const QString &username) {
    Q_UNUSED(username)
    return SwiftLib::getSettings()->value("LK_BALANCE", 0 ).toDouble();
}

double UserStatusKeeper::getUserProfitToday()  {
    QSqlQuery q( SwiftLib::createQuery("userstatus") );

     QString dbname;
     if ( SwiftLib::getSettings()->value("IS_EMULATION", 0 ).toUInt() == 1 ) {
          dbname = SwiftLib::getSettings()->value("MYSQL_DBNAME").toString()+"_test";
     } else {
         dbname =  SwiftLib::getSettings()->value("MYSQL_DBNAME").toString();
     }
    double res = 0;

    if ( !q.exec("SELECT TRUNCATE(amount,4), TRUNCATE(sum(if(type=0,TRUNCATE(-amount,4),TRUNCATE(amount,4))),4) as a, sum(if(type=0,price,-price)) as s FROM `"+dbname+"`.`orders` where date(ts)=curdate() AND status=2 group by TRUNCATE(amount,4)") ) {
        qWarning() << "Get user profit error: " << q.lastError().text();
    } else {
        while ( q.next() ) {
             if ( q.value("a").toDouble() == 0 ) {
                 res += q.value("s").toDouble();
             }
        }
    }
    q.finish();
    if ( sess != nullptr && sess->isJoined() ) {
        sess->call("swiftbot.balances.set.status.profit", QVariantList({res}));
    }

    return res;
}

double UserStatusKeeper::getUserFeeToday()  {
    const double profit_today = getUserProfitToday();
    if ( profit_today <= 0 ) {
        return 0;
    } else {
        return profit_today / 100 * SwiftLib::getSettings()->value("USERFEE", 50 ).toUInt();
    }
}
