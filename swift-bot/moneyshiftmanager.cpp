#include "moneyshiftmanager.h"


MoneyShiftManager::MoneyShiftManager(QObject *parent)  : QObject(parent), t(new QTimer(this) ),
    sell_price(0),
    sell_amount(0),
    buy_price(0),
    buy_amount(0),
    sell_orders_cnt(0),
    buy_orders_cnt(0),
    orders_cnt(0)
{
    t->setInterval(60000);
    connect( t, &QTimer::timeout, this, &MoneyShiftManager::loadPeriodOrders );
    t->start();
}

double MoneyShiftManager::getShiftSize() const {
    if ( hasShifted() ) {
        return isPositionalShift() ? buy_amount - sell_amount : sell_amount - buy_amount;
    } else {
        return 0.0;
    }
}

double MoneyShiftManager::getShiftPrice() const {
    if ( hasShifted() ) {
        return isPositionalShift() ? buy_price - sell_price : sell_price - buy_price;
    } else {
        return 0.0;
    }
}

double MoneyShiftManager::getCorrectionRate() const {
    if ( hasShifted() ) {
        return getShiftPrice() / getShiftSize();
    } else {
        return 0.0;
    }

}

double MoneyShiftManager::getProfitCorrectionRate() const {
    if ( hasShifted() ) {
        return isPositionalShift() ? (getCorrectionRate() + getCorrectionRate() * 0.01)  : (getCorrectionRate() - getCorrectionRate() * 0.01);
    } else {
        return 0.0;
    }
}

bool MoneyShiftManager::isPositionalShift() const {
    return buy_amount > sell_amount;
}

bool MoneyShiftManager::hasShifted() const {
    return static_cast<quint64>( buy_amount * 10000000 ) !=  static_cast<quint64>( sell_amount * 10000000 );
}

QJsonObject MoneyShiftManager::getJsonState() const {
    QJsonObject j_ret;
    j_ret["has_shift"] = hasShifted();
    j_ret["is_positional"] = isPositionalShift();
    j_ret["Mv"] = QString::number( getShiftSize(), 'f', 8 );
    j_ret["Mc"] = QString::number( getShiftPrice(), 'f', 8 );
    j_ret["Cr"] = QString::number( getCorrectionRate(), 'f', 8 );
    j_ret["Crp"] = QString::number( getProfitCorrectionRate(), 'f', 8 );
    return j_ret;
}

void MoneyShiftManager::loadPeriodOrders() {
    QMutexLocker lock( &mutex );
    QSqlQuery q("SELECT * FROM orders WHERE date(ts)=curdate()");
    if ( q.exec() ) {
        while( q.next() ) {
            if ( q.value("status").toUInt() == 2 ) {
                const double amount = q.value("amount").toDouble();
                const double price = q.value("price").toDouble();
                const quint32 pair_id = q.value("pair_id").toUInt();
                if ( q.value("type").toUInt() == 0 ) {
                    sell_price += price;
                    sell_amount += amount;
                    _pair_sell_price[pair_id] += price;
                    _pair_sell_amount[pair_id] += amount;
                    sell_orders_cnt++;
                } else {
                    buy_price += price;
                    buy_amount += amount;
                    _pair_buy_price[pair_id] += price;
                    _pair_buy_amount[pair_id] += amount;
                    buy_orders_cnt++;
                }
            }
        }
    }
    orders_cnt = sell_orders_cnt + buy_orders_cnt;
    if ( hasShifted() ) {
        emit hasMoneyShift( getShiftSize(), getProfitCorrectionRate(), isPositionalShift() );
    } else {
        emit hasZeroShift();
    }
    const QJsonObject j_res( getJsonState()  );
    emit publishMessage("money.shift.status", j_res );
}
