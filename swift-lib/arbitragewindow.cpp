#include "arbitragewindow.h"
#include <arbitragemanager.h>

struct Ask {
    Ask( const double&a, const double&b ) : rate( a ), amount( b ) {

    }
    double rate;
    double amount;
    double price( const double& max = 0 ) const {
        if ( max != 0 && max < amount ) {
            return max * rate;
        } else {
            return amount * rate;
        }
    }
};

struct Bid {
    Bid( const double&a, const double&b ) : rate( a ), amount( b ) {

    }
    double rate;
    double amount;
    double price( const double& max = 0 ) const {
        if ( max != 0 && max < amount ) {
            return max * rate;
        } else {
            return amount * rate;
        }
    }
};

struct Asks {
    Asks( const QMap<double, double> & a ) {
        for( auto it = a.begin(); it != a.end(); it++ ) {
            _items.push_back( Ask( it.key(), it.value() ) );
        }
    }
    double getPrice( const double& amount ) {
        double r = 0;
        double r_filled = 0;
        for( auto it = _items.begin(); it != _items.end(); it++ ) {
            if ( r_filled < amount ) {
                const double amleft = amount - r_filled;
                r += it->price( amleft );
                r_filled += amleft > it->amount ? it->amount : amleft;
            } else {
                return r;
            }
        }
        return amount * _items.last().rate;
    }
    QVector<Ask> _items;
};


struct Bids {
    Bids( const QMap<double, double> & a ) {
        for( auto it = a.begin(); it != a.end(); it++ ) {
            _items.push_back( Bid( it.key(), it.value() ) );
        }
    }
    double getPrice( const double& amount ) {
        double r = 0;
        double r_filled = 0;
        for( auto it = _items.rbegin(); it != _items.rend(); it++ ) {
            if ( r_filled < amount ) {
                const double amleft = amount - r_filled;
                r += it->price( amleft );
                r_filled += amleft > it->amount ? it->amount : amleft;
            } else {
                return r;
            }
        }
        return amount * _items.last().rate;
    }
    QVector<Bid> _items;
};

struct OrderbooksVariant {
    OrderbooksVariant( const double&a, const double&rs, const double&br, const double& sf, const double&bf )
        : amount( a ), sell_rate( rs ), buy_rate( br ), sell_fee(sf ), buy_fee( bf ) {
        fee = sellFee() + buyFee();
        profit = ( sellPrice() - sellFee() ) - ( buyPrice() + buyFee() );
    }
    double sellPrice() const {
        return amount * sell_rate;
    }
    double buyPrice() const {
        return amount * buy_rate;
    }
    double sellFee() const {
        return sellPrice() * sell_fee;
    }
    double buyFee() const {
        return buyPrice() * buy_fee;
    }

    double fee;
    double amount;
    double sell_rate;
    double buy_rate;
    double profit;
    double sell_fee;
    double buy_fee;
};

struct OrdersVariants {
    OrdersVariants( const Asks& a, const Bids& b, const double&sf = 0, const double&bf = 0,
                    const double& min_order_size = 0.1,
                    const double& max_order_size = 50,
                    const double& step = 0.1 ) :
    sell_fee(sf), buy_fee(bf), asks(a), bids(b){
        for( double size = min_order_size; size < max_order_size; size += step ) {
            const double buy_price = asks.getPrice( size );
            const double sell_price = bids.getPrice( size );
            if ( sell_price > buy_price ) {
                _variants.push_back( OrderbooksVariant( size, sell_price / size, buy_price / size, sell_fee, buy_fee ) );
            }
        }
    }
    QList<OrderbooksVariant> getProfitable() {
        QList<OrderbooksVariant> _r;
        for( auto it = _variants.begin(); it!= _variants.end(); it++ ) {
            if ( it->profit > 0 ) {
                _r.push_back( *it );
            }
        }
        return _r;
    }
    double sell_fee;
    double buy_fee;
    Asks asks;
    Bids bids;
    QList<OrderbooksVariant> _variants;
};

bool ArbitrageWindow::isLocked() const { return  is_locked; }

void ArbitrageWindow::lock() { is_locked = true; }

void ArbitrageWindow::unlock() { is_locked = false; }

void ArbitrageWindow::placeOrders() {
}

quint32 ArbitrageWindow::getSellExchangeId() const {
    return sell_exchange_id;
}

bool ArbitrageWindow::is_window_order(const QString &uid) const {
    return _placed_orders_local_ids.contains( uid );
}

void ArbitrageWindow::addWindowOrderUid(const QString &uid) {
    _placed_orders_local_ids.push_back( uid );
}

quint32 ArbitrageWindow::getSellCurrency() const {
    return sell_base_currency_id;
}

quint32 ArbitrageWindow::getBuyCurrency() const {
    return buy_price_icurrency_d;
}

void ArbitrageWindow::transferFunds() {

}

bool ArbitrageWindow::conversionStartStepRequired() {
    return !SwiftLib::getAssets()->isToken( sell_price_currency_id );
}

bool ArbitrageWindow::manualStartTransferRequired() {
    QList<quint32> _manual_withdraw_exchanges({9,2,3});
    return _manual_withdraw_exchanges.contains( sell_exchange_id );
}

bool ArbitrageWindow::hasSprint() {
    return false;
}

bool ArbitrageWindow::createSprint(const double &amount) {
    Q_UNUSED(amount)
    return false;

}

QJsonObject ArbitrageWindow::createStep(const double &amount, const bool &is_sell, const quint32 &num) const {
    QJsonObject j;
    j["type"] = is_sell ? "sell" : "buy";
    if ( is_sell ) {
        const double __rate( sell_rate );
        const double __price_raw( amount * __rate );
        const double __fee_percent( sell_fee );
        const double __fee_amount( __price_raw * __fee_percent );
        const double __result( __price_raw - __fee_amount );
        j["step"] = QString::number( num );
        j["amount"] = QString::number( amount, 'f', 8 );
        j["rate"] = QString::number( __rate, 'f', 8 );
        j["fee_size"] = QString::number( __fee_amount, 'f', 8 );
        j["fee_pcnt"] = QString::number( __fee_percent, 'f',8 );
        j["result_raw"] =  QString::number( __price_raw, 'f', 8 );
        j["result"] =  QString::number( __result, 'f', 8 );
    } else {
        const double __rate( buy_rate );
        const double __price_raw( amount );
        const double __fee_percent( buy_fee );
        const double __fee_amount( __price_raw * __fee_percent );
        const double __price( __price_raw - __fee_amount );
        const double __result( __price / __rate );

        j["step"] = QString::number( num );

        j["amount"] = QString::number( __result, 'f', 8 );
        j["rate"] = QString::number( __rate, 'f', 8 );
        j["fee_size"] = QString::number( __fee_amount, 'f', 8 );
        j["fee_pcnt"] = QString::number( __fee_percent, 'f',8 );
        j["result_raw"] =  QString::number( __price_raw, 'f', 8 );
        j["result"] =  QString::number( __result, 'f', 8 );
    }
    return j;
}

QJsonObject ArbitrageWindow::createTransferStep(const double &amount, const quint32 &from, const quint32 &to, const quint32 &num)  {
    QJsonObject j;
    j["step"] = QString::number( num );
    j["type"] = "withdraw";
    j["amount"] = QString::number( amount, 'f', 8 );
    double fee = 0;
    if ( from == sell_price_currency_id || to == buy_price_icurrency_d ) {
        if ( getArbitragePair()->buy_coin_id == 7 ) {
            fee += 0.5;
        } else if ( getArbitragePair()->buy_coin_id == 1 ) {
            fee += 0.0005;
        } else if ( getArbitragePair()->buy_coin_id == 10 ) {
            fee += 0.2;
        }
    } else {
        if ( getArbitragePair()->sell_coin_id == 7 ) {
            fee += 0.5;
        } else if ( getArbitragePair()->sell_coin_id == 1 ) {
            fee += 0.0005;
        } else if ( getArbitragePair()->sell_coin_id == 10 ) {
            fee += 0.2;
        }
    }
    const double transaction_fee_price( fee );
    const double transaction_fee_prcnt( fee / amount * 100  );
    j["fee_size"] = QString::number( transaction_fee_price, 'f', 8 );
    j["fee_pcnt"] = QString::number( transaction_fee_prcnt, 'f', 8 );
    j["result"] =  QString::number(amount - fee, 'f', 8 );
    return j;
}

QJsonObject ArbitrageWindow::createConversionStep(const double &amount, const quint32 &from, const quint32 &num) {
    QJsonObject j;
    j["type"] = "conversion";
    if ( from == sell_price_currency_id ) {
        const double __rate( getFiatConversionRate( sell_exchange_id, amount) );
        const double __price_raw( amount / __rate );
        const double __fee_percent( sell_fee );
        const double __fee_amount( __price_raw * __fee_percent );
        const double __result( __price_raw - __fee_amount );

        j["step"] = QString::number( num );
        j["amount"] = QString::number( amount, 'f', 8 );
        j["rate"] = QString::number( __rate, 'f', 8 );
        j["fee_size"] = QString::number( __fee_amount, 'f', 8 );
        j["fee_pcnt"] = QString::number( __fee_percent, 'f',8 );
        j["result_raw"] =  QString::number( __price_raw, 'f', 8 );
        j["result"] =  QString::number( __result, 'f', 8 );
    } else {
        const double __rate( getFiatConversionRate( buy_exchange_id, amount ) );
        const double __price_raw( amount );
        const double __fee_percent( buy_fee );
        const double __fee_amount( __price_raw * __fee_percent );
        const double __price( __price_raw - __fee_amount );
        const double __result( __price / __rate );

        j["step"] = QString::number( num );

        j["amount"] = QString::number( __result, 'f', 8 );
        j["rate"] = QString::number( 0.985, 'f', 8 );
        j["fee_size"] = QString::number( __fee_amount, 'f', 8 );
        j["fee_pcnt"] = QString::number( __fee_percent * 100, 'f',8 );
        j["result_raw"] =  QString::number( __price_raw, 'f', 8 );
        j["result"] =  QString::number( __result, 'f', 8 );
    }
    return j;
}

QJsonArray ArbitrageWindow::explain(const double &amount) {
    // sell
    quint32 i=1;
    QJsonArray j_steps;
    QJsonObject jlst;
    jlst = createStep( amount, true, i++ );
    j_steps.push_back( jlst );

    // convert
    // transfer
    jlst = createTransferStep(  jlst.value("result").toString().toDouble(), sell_price_currency_id, buy_price_icurrency_d, i++ );
    j_steps.push_back( jlst );

    // buy
    jlst = createStep( jlst.value("result").toString().toDouble(), false,i++ ) ;
    j_steps.push_back( jlst );

    // transfer back
   // jlst = createTransferStep(  jlst.value("result").toString().toDouble(), buy_base_currency_id, sell_base_currency_id, i++ );
   // j_steps.push_back( jlst );
    return j_steps;
}

void ArbitrageWindow::calculateOrderbooks(const QHash<quint32, QMap<double,double>>&asks_, const QHash<quint32, QMap<double,double>>&bids_) {

    if ( !asks_.contains( buy_pair_id ) && !bids_.contains( sell_pair_id ) ) {
        return;
    }

    QMap<double, double> asks( asks_.value( buy_pair_id )  );
    QMap<double, double> bids( bids_.value( sell_pair_id )  );

    if( asks.isEmpty() || bids.isEmpty() ) {
        if ( bids.isEmpty() && !bids_last.isEmpty() ) {
            bids = bids_last;
        }
        if ( asks.isEmpty() && !asks_last.isEmpty() ) {
            asks = asks_last;
        }
    }

    // Caching last data
    if( !asks.isEmpty() ) {
        asks_last = asks;
    }
    if( !bids.isEmpty() ) {
        bids_last = bids;
    }
    // Ingoring outdated data
    if( asks.count() <= 1 || bids.count() <= 1 ) {
        return;
    }

    _valid = true;
    // Max bid and min ask must be valid
    if ( bids.lastKey() <= asks.firstKey() ) {
        return;
    }

    // Modern structures to filters

    Asks _m_asks( asks );
    Bids _m_bids( bids );


    // Variants with sell price > buy price
    OrdersVariants variants( _m_asks, _m_bids, sell_fee, buy_fee, min_order_size, max_order_size, order_size_step );

    if ( variants._variants.isEmpty() ) {
        return;
    }

    // Balances for window json info updating
    bb = getArbitragePair()->getBalanceAvailable(buy_price_icurrency_d);
    sb = getArbitragePair()->getBalanceAvailable(sell_base_currency_id );

    // Get only profitable variants, if not - just update spreads etc..
    QList<OrderbooksVariant> _profitable( variants.getProfitable() );
    if ( _profitable.isEmpty() ) {
        sell_rate = variants._variants.first().sell_rate;
        buy_rate = variants._variants.first().buy_rate;
        max_size = 0;
    } else {
        for ( auto it = _profitable.begin(); it!= _profitable.end(); it++ ) {
            sell_rate = it->sell_rate;
            buy_rate = it->buy_rate;
            max_size = it->amount;
        }
    }

    // Filling stats data
    _spread_avg.push_back( getSpreadPercent() );
    if ( _spread_avg.size() > 100 ) {
        _spread_avg.removeFirst();
    }

    if ( sell_rate > 0 ) {
    _sell_rate_avg.push_back( sell_rate );
        if ( _sell_rate_avg.size() > 100 ) {
            _sell_rate_avg.removeFirst();
        }
    }

    if ( buy_rate > 0 ) {
        _buy_rate_avg.push_back( buy_rate );
        if ( _buy_rate_avg.size() > 100 ) {
            _buy_rate_avg.removeFirst();
        }
    }

    _amount_avg.push_back( max_size * 2 );

    double req_bb = 0;
    double req_sb = 0;
    if ( _profitable.isEmpty() ) {
        return;
    }
    for ( auto it = _profitable.rbegin(); it!= _profitable.rend(); it++ ) {
        if ( sb >= it->amount && bb >= it->sellPrice() && it->profit > 0 ) {
            getArbitragePair()->addClassicCandidat( id, it->amount, it->sell_rate, it->buy_rate );
            return;
        } else {
            if ( sb < it->amount ) {
                req_sb = it->amount - sb;
            }
            if ( req_bb < it->buyPrice() ) {
                req_bb = it->buyPrice() - bb;
            }
        }
    }
    if ( req_bb > 0 ) {
        required_balance_buy = req_bb;
        getArbitragePair()->setRequestedAmount( id, buy_price_icurrency_d, required_balance_buy );
    }
    if ( req_sb > 0 ) {
        required_balance_sell = req_sb;
        getArbitragePair()->setRequestedAmount( id, sell_base_currency_id, required_balance_sell );
    }
    //publishData();

}

ArbitrageWindow::ArbitrageWindow(QObject *parent) : QObject( parent ),sell_rate(0),buy_rate(0),  session(nullptr), max_size(0),amount_scale(5), rate_scale(5)
{
    id=0;
    is_enabled=false;
    sell_order_tested=false;
    buy_order_tested=false;
    base_deposit_tested=false;
    market_deposit_tested=false;
    base_withdraw_tested=false;
    market_withdraw_tested=false;
    _valid=false;
    is_locked = false;
    min_order_size= min_size;
    max_order_size= getArbitragePair()->getManager()->getParam("MAX_ORDER_SIZE").toDouble();
    order_size_step= getArbitragePair()->getManager()->getParam("ORDER_STEP_SIZE").toDouble();
}

bool ArbitrageWindow::open() {
    return true;
}

bool ArbitrageWindow::close() {
    return true;
}

ArbitrageWindow::ArbitrageWindow(const QSqlRecord &rec, QObject *parent) : QObject( parent ),sell_rate(0),buy_rate(0),min_spread(100),session(nullptr),
    max_size(0),amount_scale(5), rate_scale(5) {
    id = rec.value("id").toUInt();
    sell_pair_id = rec.value("sell_market").toUInt();
    buy_pair_id = rec.value("buy_market").toUInt();

    sell_base_currency_id = rec.value("sell_base_currency_id").toUInt();
    sell_price_currency_id = rec.value("sell_price_currency_id").toUInt();

    buy_base_currency_id = rec.value("buy_base_currency_id").toUInt();
    buy_price_icurrency_d = rec.value("buy_price_currency_id").toUInt();

    sell_base_is_token = SwiftLib::getAssets()->isToken( sell_base_currency_id );
    sell_price_is_token = SwiftLib::getAssets()->isToken( sell_price_currency_id );
    buy_base_is_token = SwiftLib::getAssets()->isToken( buy_base_currency_id );
    buy_price_is_token = SwiftLib::getAssets()->isToken( buy_price_icurrency_d );

    min_size=0;
    max_size=0;


    sell_exchange_id = rec.value("sell_exchange").toUInt();
    buy_exchange_id = rec.value("buy_exchange").toUInt();

    sell_fee = SwiftLib::getExchangeFee( sell_exchange_id );
    buy_fee = SwiftLib::getExchangeFee( buy_exchange_id );

    last_available_time=0;
    available_since=0;
    is_available = false;
    weight = rec.value("weight").toDouble();
    ticks_cnt = 0;
    first_send_conversion_fee = 0;
    first_send_transaction_fee = 0;
    back_send_conversion_fee = 0;
    back_send_transaction_fee = 0;
    is_locked = false;
    const quint32 sell_price_coin_id( SwiftLib::getAssets()->getCurrencyCoin( sell_price_currency_id ) );
/*
    const quint32 sell_base_coin_id( SwiftLib::getAssets()->getCurrencyCoin( sell_base_currency_id ) );
    const quint32 buy_base_coin_id( SwiftLib::getAssets()->getCurrencyCoin( buy_base_currency_id ) );

    const quint32 buy_price_coin_id( SwiftLib::getAssets()->getCurrencyCoin( buy_price_icurrency_d ) );
*/
    if ( sell_price_coin_id == 7 ) {
        if ( !SwiftLib::getAssets()->isToken( sell_price_currency_id ) ) {
            first_send_conversion_fee += 0.03;
        }
        first_send_transaction_fee += 1;
    } else if (  sell_price_coin_id == 1 ) {
        first_send_transaction_fee += 0.0005;
    } else if (  sell_price_coin_id == 10 ) {
        first_send_transaction_fee += 0.1;
    }

    if ( buy_base_currency_id == 7 ) {

    } else if (  buy_base_currency_id == 1 ) {
        back_send_transaction_fee += 0.0005;
    } else if (  buy_base_currency_id == 10 ) {
        back_send_transaction_fee += 0.1;
    }

    bname = SwiftLib::getAssets()->getCurrencyName( sell_base_currency_id );
    if ( SwiftLib::getAssets()->getCurrencyCoin( sell_base_currency_id ) == 1 ) {
        bname = "BTC";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_base_currency_id ) == 2 ) {
        bname = "ETH";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_base_currency_id ) == 7 ) {
        bname = "USDT";
    }

    pname = SwiftLib::getAssets()->getCurrencyName( sell_price_currency_id );
    if ( SwiftLib::getAssets()->getCurrencyCoin( sell_price_currency_id ) == 1 ) {
        pname = "BTC";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_price_currency_id ) == 2 ) {
        pname = "ETH";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_price_currency_id ) == 7 ) {
        pname = "USDT";
    }

    _valid=true;
    QTimer::singleShot( 60000, this, &ArbitrageWindow::saveWindow );
    min_spread = getArbitragePair()->getManager()->getParam("MIN_WINDOW_SPREAD").toDouble();
    min_size = getArbitragePair()->getManager()->getParam("MIN_ORDER_SIZE").toDouble();
    minspread = getArbitragePair()->getManager()->getParam("MIN_WINDOW_SPREAD").toDouble();
    amount_scale = getArbitragePair()->getManager()->getParam("AMOUNT_PRECISION").toUInt();
    min_order_size= min_size;
    max_order_size= getArbitragePair()->getManager()->getParam("MAX_ORDER_SIZE").toDouble();
    order_size_step= getArbitragePair()->getManager()->getParam("ORDER_STEP_SIZE").toDouble();
    rate_scale =  getArbitragePair()->getManager()->getParam("RATE_PRECISION").toUInt();
    const quint32 _spprec = SwiftLib::getAssets()->getMarketPricePrecision( sell_pair_id );
    const quint32 _saprec = SwiftLib::getAssets()->getMarketAmountPrecision( sell_pair_id );
    const quint32 _bpprec = SwiftLib::getAssets()->getMarketPricePrecision( buy_pair_id );
    const quint32 _baprec = SwiftLib::getAssets()->getMarketAmountPrecision( buy_pair_id );
    _price_precision = _spprec < _bpprec ? _spprec : _bpprec;
    _amount_precision = _saprec < _baprec ? _saprec : _baprec;
}

void ArbitrageWindow::update(const QSqlRecord &rec) {
    sell_pair_id = rec.value("sell_pair_id").toUInt();
    buy_pair_id = rec.value("buy_pair_id").toUInt();

    sell_base_currency_id = rec.value("sell_base_currency_id").toUInt();
    sell_price_currency_id = rec.value("sell_price_currency_id").toUInt();

    buy_base_currency_id = rec.value("buy_base_currency_id").toUInt();
    buy_price_icurrency_d = rec.value("buy_price_currency_id").toUInt();

    sell_exchange_id = rec.value("sell_exchange").toUInt();
    buy_exchange_id = rec.value("buy_exchange").toUInt();
    weight = rec.value("weight").toDouble();
    bname = SwiftLib::getAssets()->getCurrencyName( sell_base_currency_id );
    if ( SwiftLib::getAssets()->getCurrencyCoin( sell_base_currency_id ) == 1 ) {
        bname = "BTC";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_base_currency_id ) == 2 ) {
        bname = "ETH";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_base_currency_id ) == 7 ) {
        bname = "USDT";
    }

    pname = SwiftLib::getAssets()->getCurrencyName( sell_price_currency_id );
    if ( SwiftLib::getAssets()->getCurrencyCoin( sell_price_currency_id ) == 1 ) {
        pname = "BTC";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_price_currency_id ) == 2 ) {
        pname = "ETH";
    } else if ( SwiftLib::getAssets()->getCurrencyCoin( sell_price_currency_id ) == 7 ) {
        pname = "USDT";
    }
    max_order_size= getArbitragePair()->getManager()->getParam("MAX_ORDER_SIZE").toDouble();
    order_size_step= getArbitragePair()->getManager()->getParam("ORDER_STEP_SIZE").toDouble();
    min_size = getArbitragePair()->getManager()->getParam("MIN_ORDER_SIZE").toDouble();

}

bool ArbitrageWindow::enable() {
    if ( !is_enabled ) {
        is_enabled = true;

    }
    return is_enabled == true;
}

bool ArbitrageWindow::disable() {
    if ( is_enabled ) {
        is_enabled = false;

    }
    return is_enabled != true;
}

bool ArbitrageWindow::check(const quint32 &method) {
    Q_UNUSED(method)
    return true;
}




quint64 ArbitrageWindow::getTicksCount() const {
    return ticks_cnt;
}

double ArbitrageWindow::getFiatConversionRate(const quint32 &exchange_id, const double &amount) {
    Q_UNUSED(exchange_id)
    Q_UNUSED(amount)
     return 1.125;
}

QJsonObject ArbitrageWindow::toJson(  const bool& short_version  ) {
    Q_UNUSED( short_version )
    QJsonObject j_itm( j_prepared );
    if ( !j_itm.contains("id") ) {
        j_itm["id"] =  QString::number( id );
        j_itm["min_size"] =  QString::number( getMinSize(), 'f', 8 );

        j_itm["base_currency_name"] = bname;
        j_itm["price_currency_name"] = pname;
        j_itm["buy_exchange_id"] = QString::number( buy_exchange_id );
        j_itm["sell_exchange_id"] = QString::number( sell_exchange_id );
        j_itm["sell_exchange_name"] = SwiftLib::getAssets()->getExchangeName( sell_exchange_id );
        j_itm["buy_exchange_name"] = SwiftLib::getAssets()->getExchangeName( buy_exchange_id );
        j_itm["sell_base_currency_balance"] = "0.0";
        j_itm["sell_price_currency_balance"] = "0.0";
        j_itm["buy_base_currency_balance"] = "0.0";
        j_itm["buy_price_currency_balance"] = "0.0";
        j_itm["avaible_to_get_profit"] = "0.0";
        j_itm["sell_pair_id"] =  QString::number( sell_pair_id );
        j_itm["buy_pair_id"] =  QString::number( buy_pair_id );
        j_itm["sell_base_currency_id"] =  QString::number( sell_base_currency_id );
        j_itm["sell_price_currency_id"] =  QString::number( sell_price_currency_id );
        j_itm["buy_base_currency_id"] =  QString::number( buy_base_currency_id );
        j_itm["buy_price_currency_id"] =  QString::number( buy_price_icurrency_d );
        j_itm["sell_base_is_token"] = sell_base_is_token;
        j_itm["sell_price_is_token"] =  sell_price_is_token;
        j_itm["buy_base_is_token"] = buy_base_is_token;
        j_itm["buy_price_is_token"] = buy_price_is_token;
        j_prepared = j_itm;
    }

    if ( !j_itm.contains("updated_json") || QDateTime::currentSecsSinceEpoch() - j_itm.value("updated_json").toVariant().toULongLong() > 5 ) {

        const double spreadpercent = getSpreadPercent();
        const double size = _amount_avg.isEmpty() ? 0 : std::accumulate( _amount_avg.begin(), _amount_avg.end(), 0.0 ) / _amount_avg.count();
        const double profit = size == 0 ? 0 : getRealProfit( size );

        j_itm["updated_json"] =  QString::number( QDateTime::currentSecsSinceEpoch() );
        j_itm["sell_rate"] = QString::number( sell_rate, 'f', _price_precision );
        j_itm["sell_rate_avg"] = QString::number( sell_rate_avg(), 'f', _price_precision );
        j_itm["buy_rate"] = QString::number( buy_rate, 'f', _price_precision );
        j_itm["buy_rate_avg"] = QString::number( buy_rate_avg(), 'f', _price_precision );
        j_itm["max_size"] =  QString::number( size, 'f', _amount_precision );
        j_itm["min_spread"] = QString::number( *std::min_element( _spread_avg.begin(), _spread_avg.end() ), 'f', 2 );
        j_itm["max_spread"] = QString::number( *std::max_element( _spread_avg.begin(), _spread_avg.end() ), 'f', 2 );

        j_itm["spread_percent"] = QString::number( spreadpercent, 'f', 4 );
        j_itm["profit_percent"] = QString::number( getRealProfitPercent(), 'f', 4 );

        j_itm["dry_percent"] = QString::number( getDryProfitPercent(), 'f', 4 );
        j_itm["stat_percent"] = QString::number( getRealProfitPercent(), 'f', 4 );
        j_itm["real_percent"] = QString::number( spreadpercent, 'f', 4 );


        j_itm["avaible_to_get_profit"] = QString::number(profit, 'f', _price_precision );

        j_itm["sell_base_currency_balance"] =  QString::number( sb , 'f', 8 );
        j_itm["buy_price_currency_balance"] =  QString::number( bb  , 'f', 8 );
        j_itm["last_available_time"] =  QString::number(last_available_time);
        j_itm["available_since"] =  QString::number( available_since);
        j_itm["active"] = is_available;
        j_itm["is_enabled"] = is_enabled;
        j_itm["req_sell"] = QString::number( size > 0 ? size / 2 : 0, 'f', _amount_precision );
        j_itm["req_buy"] = QString::number( size > 0 ? ( size / 2 * ( buy_rate > 0 ? buy_rate : sell_rate ) ) : 0 , 'f', _price_precision );
    }
    return j_itm;
}

double ArbitrageWindow::getRequestedBalance(const quint32 &currency) {
    return getArbitragePair()->getBalance( currency );
}

double ArbitrageWindow::amountsumm() const {
    QList<double> _d( _amount_avg );
    if ( _d.isEmpty() ) {
        return 0;
    }
    return std::accumulate( _d.begin(), _d.end(), 0.0 ) / _d.size();
}

double ArbitrageWindow::sell_rate_avg() const {
    QList<double> _d( _sell_rate_avg );
    if ( _d.isEmpty() ) {
        return 0;
    }
    return std::accumulate( _d.begin(), _d.end(), 0.0 ) / _d.size();
}

double ArbitrageWindow::buy_rate_avg() const {
    QList<double> _d( _buy_rate_avg );
    if ( _d.isEmpty() ) {
        return 0;
    }
    return std::accumulate( _d.begin(), _d.end(), 0.0 ) / _d.size();
}

double ArbitrageWindow::getBuyBalance() {
    return getArbitragePair()->getBalanceAvailable( buy_price_icurrency_d );

}

double ArbitrageWindow::getSellBalance() {
    return getArbitragePair()->getBalanceAvailable( sell_base_currency_id );

}

double ArbitrageWindow::getOrderSize() {
    const double sb = getSellBalance();
    const double bb = getBuyBalance();
    if ( sb == 0 || bb == 0 ) {
        return 0.1;
    }

    if ( sb >= bb ) {
        if ( bb / buy_rate >= 0.1 ) {
            return bb / buy_rate;
        } else {
            return 0.1;
        }
    } else {
        if ( sb >= 0.1 ) {
            return sb;
        } else {
            return 0.1;
        }
    }
}

bool ArbitrageWindow::isAvailable() const {
    if ( last_available_time > 0 && QDateTime::currentSecsSinceEpoch() - last_available_time <= 180 ) {
        return is_available;
    } else {
        return false;
    }
}

bool ArbitrageWindow::isEnabled() const {
    return is_enabled;
}

bool ArbitrageWindow::isValid() const {
    double real_sell = sell_rate;
    double real_buy = buy_rate;
    double diff = real_sell - real_buy;
    double ret = diff / real_sell * 100;
    return ( id > 0 && sell_exchange_id > 0 && buy_exchange_id > 0 && sell_rate > 0 && buy_rate > 0 && ret > -0.2);
}

bool ArbitrageWindow::isPositive() const {
    return  QDateTime::currentSecsSinceEpoch() - last_available_time <= 3600;
}

double ArbitrageWindow::getMinSize() const {
    return min_size;
}

double ArbitrageWindow::getDryProfitPercent() {
    if ( _amount_avg.isEmpty() ) {
        return 0;
    }
    if ( sell_rate <= 0 || buy_rate <= 0 ) {
        return 0;
    }
    const double size = std::accumulate( _amount_avg.begin(), _amount_avg.end(), 0.0 ) / _amount_avg.count();
    const double size_price = size * ( sell_rate > 0 ? sell_rate : buy_rate );
    const double profit = getRealProfit( size );
    return profit / size_price * 100;
}

double ArbitrageWindow::getStatProfitPercent() const {
    if ( sell_rate == 0 || buy_rate == 0 ) {
        return 0;
    }
    double real_sell = sell_rate;
    double real_buy = buy_rate;
    double diff = real_sell - real_buy;
    double ret =  (diff / real_sell ) * 100;
    return ret;
}

double ArbitrageWindow::getRealProfitPercent() const {
    if ( sell_rate == 0 || buy_rate == 0 ) {
        return 0;
    }
    double _calc_size = max_size > 0 ? max_size : 1;
    double sell_price = ( _calc_size * sell_rate ) - ( _calc_size * sell_rate * sell_fee );
    double buy_price =  ( _calc_size * buy_rate ) + ( _calc_size * buy_rate * buy_fee );
    double diff = sell_price - buy_price;
    double ret = diff / sell_price * 100;
    return ret;
}

double ArbitrageWindow::getRealProfit(const double &amount) const {
    // sell income with fee
    if (  sell_rate == 0 || buy_rate == 0 || amount <= 0 ) {
        return 0;
    }
    double ret = amount * getRealProfitPercent();
    return ret;
}

double ArbitrageWindow::getSpread() const {
    if ( buy_rate == 0 || sell_rate == 0 ) {
        return 0;
    }
    return sell_rate - buy_rate;
}

double ArbitrageWindow::getSpreadPercent() const
{
    if ( getSpread() == 0 ) {
        return 0;
    }
    return getSpread() / sell_rate * 100;
}

void ArbitrageWindow::setAsClassicCandidat() {

    if ( max_size < getSellBalance() ) {
        if ( getRealProfit( max_size ) > 0 ) {
            getArbitragePair()->addClassicCandidat( id, max_size, sell_rate, buy_rate );
        }
    } else {
        if ( getRealProfit( getSellBalance() ) > 0 ) {
            getArbitragePair()->addClassicCandidat( id, getSellBalance(), sell_rate, buy_rate );
        }
    }

}

void ArbitrageWindow::onErrorOrder(const QJsonObject &j_itm){
    if ( hasSprint() ) {
        if ( j_itm.contains("local_id") ) {
            const QString local_id( j_itm.value("local_id").toString() );
            if ( local_id != "" && is_window_order( local_id )
                 ) {
                unlock();
                //opened_sprint->orderError( j_itm );
            }
        }
    }
}

void ArbitrageWindow::onCompletedOrder(const QJsonObject &j_itm){
    if ( hasSprint() ) {
        if ( j_itm.contains("local_id") ) {
            const QString local_id( j_itm.value("local_id").toString() );
            if ( local_id != "" && is_window_order( local_id )
                 ) {
                unlock();
                // opened_sprint->orderPlaced( j_itm );
            }
        }
    }
}

void ArbitrageWindow::onPlacedOrder(const QJsonObject &j_itm){
    if ( hasSprint() ) {
        if ( j_itm.contains("local_id") ) {
            const QString local_id( j_itm.value("local_id").toString() );
            if ( local_id != "" && is_window_order( local_id )
                 ) {
                unlock();
                // opened_sprint->orderPlaced( j_itm );
            }
        }
    }
}

void ArbitrageWindow::onCanceledOrder(const QJsonObject &j_itm){
    if ( hasSprint() ) {
        if ( j_itm.contains("local_id") ) {
            const QString local_id( j_itm.value("local_id").toString() );
            if ( local_id != "" && is_window_order( local_id )
                 ) {
                unlock();
                // opened_sprint->orderPlaced( j_itm );
            }
        }
    }
}

void ArbitrageWindow::checkIsActive() {
    if ( QDateTime::currentSecsSinceEpoch() - last_available_time >= 250 ) {
        is_available = false;
    }
    QTimer::singleShot( 15000, this, &ArbitrageWindow::checkIsActive );
}

void ArbitrageWindow::saveWindow() {
    if ( isValid() && getSpread() != 0 && !_amount_avg.isEmpty() ) {
        QSqlQuery q;
        const double size = std::accumulate( _amount_avg.begin(), _amount_avg.end(), 0.0 ) / _amount_avg.count();
        const double profit = getRealProfit( size );
        q.prepare("INSERT INTO `"+ SwiftLib::getUserDbName().toUtf8() +"`.`window_history` (`window_id`,`spread_size`,`spread_percent`,`weight`,`size`,`profit`) \
                  VALUES (:window_id,:spread_size,:spread_percent,:weight,:size,:profit)");
        q.bindValue(":window_id", id );
        q.bindValue(":spread_size", getSpread() );
        q.bindValue(":spread_percent",getSpreadPercent() );
        q.bindValue(":weight", 10.0 );
        q.bindValue(":size", size);
        q.bindValue(":profit", profit );
        if ( !q.exec() ) {
            qWarning() << q.lastError().text();
        }
        if ( _amount_avg.count() >= 200 ) {
            _amount_avg.clear();
        }
    }
    QTimer::singleShot( 60000, this, &ArbitrageWindow::saveWindow );
}

void ArbitrageWindow::onWampSession(Wamp::Session *sess) {
    session = sess;

}

ArbitragePair *ArbitrageWindow::getArbitragePair() {
    return static_cast<ArbitragePair*>( parent() );
}
