#include "candidatsfilter.h"
#include <QElapsedTimer>
#include "arbitragemanager.h"
CandidatsFilter::CandidatsFilter(QObject *parent) : QObject(parent), assets( SwiftLib::getAssets() ), sess(nullptr), arbitrage_manager( new ArbitrageManager )
{


}



quint32 CandidatsFilter::getSystemMode() const {
    if ( sess != nullptr && sess->isJoined() ) {
        const quint32 mode = sess->call("swiftbot.system.mode", QVariantList() ).toUInt();
        return mode;
    } else {
        return 0;
    }
}

void CandidatsFilter::reportWindowsCollected() {
    if ( sess != nullptr && sess->isJoined() ) {
        sess->call("swiftbot.system.set.haswindows");
    }
}

void CandidatsFilter::onWampSession(Wamp::Session *session) {
    sess = session;

}

void CandidatsFilter::onOrderBooks(const QHash<quint32, QMap<double, double>> &asks, const QHash<quint32, QMap<double, double>> &bids) {
    //_asks = asks;
    //_bids = bids;
    arbitrage_manager->calculateOrderbooks( asks, bids );

    // filterCandidats();
    return;
    const quint32 mode = getSystemMode();
    if ( mode > 1 ) {
        if ( mode == 2 ) {
            // All windows
            filterCandidats();
        } else {
            if ( sess != nullptr && sess->isJoined() ) {
                const QString strl = sess->call("swiftbot.system.get.activewindows").toString();
                const QJsonArray lst( QJsonDocument::fromJson( strl.toUtf8() ).array() );
                QList<quint32> _activewins;
                for( auto i = lst.begin(); i != lst.end(); i++ ) {
                    _activewins.push_back( i->toInt() );
                }
                if ( _activewins.isEmpty() ) {
                    filterCandidats();
                } else {
                    filterCandidats( _activewins );
                }

            }
            // Exact windows
        }
    } else if ( mode == 1 ) {
        // filterCandidats();
        // Balance preparing mode
    } else {
        filterCandidats();
        // Data collection mode
    }

}

void CandidatsFilter::onMoneyShiftExists(const double &amount, const double &rate, const bool &is_positive) {
    hasMoneyShift = true;
    money_shift_rate = rate;
    money_shift_amount = amount;
    hasPositiveMoneyShift = is_positive;
}

void CandidatsFilter::onMoneyZeroShift() {
    hasMoneyShift = false;
}


QList<ArbitrageCandidat> CandidatsFilter::filterPair(const quint32 &sell_pair, const quint32 &buy_pair) {

    QMutexLocker lock( &m );


    QList<ArbitrageCandidat> _r;
    const quint32 arb_pair_id_sell( assets->getMarketArbitragePairId( sell_pair ) );
    const quint32 arb_pair_id_buy( assets->getMarketArbitragePairId( buy_pair ) );
    if ( arb_pair_id_sell != arb_pair_id_buy ) {
        return _r;
    }


    const double min_spread = SwiftLib::getSettings()->value("ORDERBOOKS_MIN_SPREAD", 0.1).toDouble();
    const double min_raw_profit =  SwiftLib::getSettings()->value("ARBITRAGE_MIN_PROFIT_RAW", 0.0001).toDouble();
    const double min_order_size = arb_pair_id_sell == 1 ?  SwiftLib::getSettings()->value("ARBITRAGE_MIN_ORDER_SIZE", 0.01).toDouble() : 350;
    const double max_order_size = arb_pair_id_sell == 1 ?  SwiftLib::getSettings()->value("ARBITRAGE_MAX_ORDER_SIZE", 0.01).toDouble() : 10000;

    const quint32 sell_pair_id = sell_pair;
    const double sell_fee_base = SwiftLib::getExchangeFee( assets->getMarketExchangeId( sell_pair_id )  );
    const QMap<double,double> _sell_variants( getBids( sell_pair_id ) );


    if ( !_sell_variants.isEmpty() ) {
        for( auto sell_it = _sell_variants.end(); sell_it != _sell_variants.begin(); sell_it-- ) {
            if ( sell_it == _sell_variants.end() ) {

            } else {
                const double sell_rate = sell_it.key();
                const double sell_amount = sell_it.value();
                // Находим пару с минимальным курсом покупки
                const quint32 buy_pair_id = buy_pair;
                const double buy_fee_base = SwiftLib::getExchangeFee( assets->getMarketExchangeId( buy_pair_id )  );
                const QMap<double,double> _buy_variants( getAsks( buy_pair_id ) );
                if ( !_buy_variants.isEmpty() ) {
                    for( auto buy_it = _buy_variants.begin(); buy_it != _buy_variants.end(); buy_it++ ) {
                        const double buy_rate = buy_it.key();
                        const double buy_amount = buy_it.value();

                        if ( buy_rate < sell_rate ) {

                            const double spread_size = ( sell_rate - buy_rate ) / ( sell_rate - ( ( sell_rate - buy_rate ) / 2 ) ) * 100;
                            if ( spread_size >= min_spread ) {
                                double amount = sell_amount > buy_amount ? buy_amount : sell_amount;
                                // Max order size
                                if ( amount > max_order_size ) {
                                    amount = max_order_size;
                                }
                                // Проверяем сырую профитность
                                const double raw_sell = sell_rate * amount;
                                const double raw_buy = buy_rate * amount;
                                const double sell_price = raw_sell - (raw_sell* sell_fee_base );
                                const double buy_price = raw_buy + (raw_sell* buy_fee_base );
                                const double raw_profit = sell_price - buy_price;
                                if ( raw_profit >= min_raw_profit && amount >= min_order_size ) {
                                   ArbitrageCandidat c(
                                                arb_pair_id_sell,
                                                sell_pair_id,
                                                buy_pair_id,
                                                sell_rate,
                                                raw_sell,
                                                sell_fee_base,
                                                buy_rate,
                                                raw_buy,
                                                buy_fee_base,
                                                amount
                                                );
                                    if ( c.profit() > 0 && c.profit() > min_raw_profit  ) {
                                        _r.push_back(c);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return _r;
}

double CandidatsFilter::getExchangeFee(const quint32 &pair_id) const {
    Q_UNUSED( pair_id )
    return 0.0015;
}



void CandidatsFilter::filterCandidats( const QList<quint32>& awins ) {

    if ( _arbitrage_pairs.isEmpty() ) {
        _arbitrage_pairs = arbitrage_manager->getPairsRels();

    }

    QList<ArbitrageCandidat> _filtered_candidats;

    if ( !awins.isEmpty() ) {
        for( auto it = _windows.begin(); it != _windows.end(); it++ ) {
            if ( awins.contains( it.value().id ) ) {
                _filtered_candidats.append( filterPair( it.value().sell_pair, it.value().buy_pair ) );
            }
        }
    } else {
        for ( auto it = _arbitrage_pairs.begin(); it != _arbitrage_pairs.end(); it++ ) {
            // double mid_rate = _mid_rates.value( arbitrage_pair_id, 0.0 );
            const QList<quint32> pairs( it.value() );
            for( auto pit = pairs.begin(); pit != pairs.end(); pit++ ) {
                // Находим пару для продажи по максимальному курсу
                for( auto pit2 = pairs.begin(); pit2 != pairs.end(); pit2++ ) {
                    // Находим пару для продажи по максимальному курсу
                    if ( pit != pit2 ) {
                        _filtered_candidats.append( filterPair( *pit, *pit2 ) );
                    }

                }
            }
        }
    }
    if ( !_filtered_candidats.isEmpty() ) {
/*        QJsonObject j_msg;
        QJsonArray _candidats_json;
        for( auto it = _filtered_candidats.begin(); it != _filtered_candidats.end(); it++ ) {
            QJsonObject itm( it->toJson() );
            QPair<quint32,quint32> kk(assets->getMarketExchangeId( it->sell_pair_id), assets->getMarketExchangeId( it->buy_pair_id));
            if( _windows_rels.contains( kk ) ) {
                itm["window_id"] = QString::number(  _windows_rels.value( kk ) );
            }
            _candidats_json.push_back( itm );
        }
        j_msg["items"] = _candidats_json;
        const QString candidats_json( QJsonDocument(j_msg).toJson(QJsonDocument::Compact) );
        emit publishMessage("swiftbot.arbitrage.candidats", j_msg );
*/
        emit readyCandidats( _filtered_candidats );

    }
}

QMap<double, double> CandidatsFilter::getAsks(const quint32 &pair_id) const {
    const QMap<double,double> asks( _asks[pair_id] );
    QMap<double,double> ret;
    const quint32 skip_top_records = SwiftLib::getSettings()->value("ORDERBOOKS_SKIP_TOP", 0).toUInt();
    const quint32 depth_max = SwiftLib::getSettings()->value("ORDERBOOKS_DEPTH", 10).toUInt();
    const bool aggregate_amounts = SwiftLib::getSettings()->value("ARBITRAGE_AGGREGATE_AMOUNTS", 0).toUInt() == 1;
    quint32 i = 0;
    quint32 skipped = 0;

    if ( aggregate_amounts ) {
        double amount = 0;
        for( auto it = asks.begin(); it != asks.end(); it++ ) {
            amount += it.value();
            if ( i < depth_max ) {
                if ( skipped < skip_top_records ) {
                    skipped++;
                } else {
                    ret.insert( it.key(), amount );
                    i++;
                }
            }
        }
    } else {
        for( auto it = asks.begin(); it != asks.end(); it++ ) {
            if ( i < depth_max ) {
                if ( skipped < skip_top_records ) {
                    skipped++;
                } else {
                    ret.insert( it.key(), it.value() );
                    i++;
                }
            }
        }
    }
    return ret;
}

QMap<double, double> CandidatsFilter::getBids(const quint32 &pair_id) const {
    const QMap<double,double> bids( _bids[pair_id] );
    QMap<double,double> ret;
    const quint32 skip_top_records = SwiftLib::getSettings()->value("ORDERBOOKS_SKIP_TOP", 0).toUInt();
    const quint32 depth_max = SwiftLib::getSettings()->value("ORDERBOOKS_DEPTH", 10).toUInt();
    const bool aggregate_amounts = SwiftLib::getSettings()->value("ARBITRAGE_AGGREGATE_AMOUNTS", 0).toUInt() == 1;
    quint32 i = 0;
    quint32 skipped = 0;
    if ( aggregate_amounts ) {
        double amount = 0;
        for( auto it = bids.end(); it != bids.begin(); --it ) {
            amount += it.value();
            if ( i < depth_max ) {
                if ( skipped < skip_top_records ) {
                    skipped++;
                } else {
                    ret.insert( it.key(), amount );
                    i++;
                }
            }
        }
    } else {
        for( auto it = bids.end(); it != bids.begin(); --it ) {

            if ( i < depth_max ) {
                if ( skipped < skip_top_records ) {
                    skipped++;
                } else {
                    ret.insert( it.key(), it.value() );
                    i++;
                }
            }
        }
    }
    return ret;
}

ArbitrageWindiowStats::ArbitrageWindiowStats() : sell_rate(0),buy_rate(0),spread_size(0),ticks(0) {

}

ArbitrageWindiowStats ArbitrageWindiowStats::operator()(const ArbitrageWindiowStats &other) {
    sell_rate = other.sell_rate;
    buy_rate = other.buy_rate;
    spread_size = other.spread_size;
    ticks = other.ticks;
    return *this;
}

ArbitrageWindiowStats ArbitrageWindiowStats::operator=(const ArbitrageWindiowStats &other) {
    sell_rate = other.sell_rate;
    buy_rate = other.buy_rate;
    spread_size = other.spread_size;
    ticks = other.ticks;
    return *this;
}

ArbitrageWindiowStats::ArbitrageWindiowStats(const ArbitrageWindiowStats &other) : sell_rate(other.sell_rate), buy_rate(other.buy_rate),spread_size(other.spread_size),ticks(other.ticks) {

}

void ArbitrageWindiowStats::addTick() {
    ticks++;
}

ArbitrageWindowDefinition::ArbitrageWindowDefinition(const ArbitrageWindowDefinition &other) : id( other.id ), sell_exchange(other.sell_exchange), sell_pair(other.sell_pair),buy_exchange(other.buy_exchange), buy_pair(other.buy_pair), stats(other.stats) {

}

ArbitrageWindowDefinition ArbitrageWindowDefinition::operator()(const ArbitrageWindowDefinition &other) {
    id = other.id;
    sell_exchange = other.sell_exchange;
    sell_pair = other.sell_pair;
    buy_exchange = other.buy_exchange;
    buy_pair = other.buy_pair;
    stats = other.stats;
    return *this;
}

ArbitrageWindowDefinition ArbitrageWindowDefinition::operator=(const ArbitrageWindowDefinition &other) {
    id = other.id;
    sell_exchange = other.sell_exchange;
    sell_pair = other.sell_pair;
    buy_exchange = other.buy_exchange;
    buy_pair = other.buy_pair;
    stats = other.stats;
    return *this;
}

ArbitrageWindowDefinition::ArbitrageWindowDefinition():id(0),sell_exchange(0),sell_pair(0),buy_exchange(0),buy_pair(0) {

}

ArbitrageWindowDefinition::ArbitrageWindowDefinition(const quint32 &_id, const quint32 &_sell_exchange, const quint32 &_sell_pair, const quint32 &_buy_exchange, const quint32 &_buy_pair):id(_id),sell_exchange(_sell_exchange),sell_pair(_sell_pair),buy_exchange(_buy_exchange),buy_pair(_buy_pair) {

}
