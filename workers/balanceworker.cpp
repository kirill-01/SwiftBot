#include "balanceworker.h"


BalanceWorker::BalanceWorker(QObject *parent) : SwiftWorker( parent ), assets( SwiftLib::getAssets() ) {
    connect( this, &BalanceWorker::eventUpdate, this, &BalanceWorker::onUpdate, Qt::QueuedConnection );
    connect( this, &BalanceWorker::eventDeposit, this, &BalanceWorker::onDeposit, Qt::QueuedConnection );
    connect( this, &BalanceWorker::eventDeposits, this, &BalanceWorker::onDeposits, Qt::QueuedConnection );
    connect( this, &BalanceWorker::eventWithdraws, this, &BalanceWorker::onWithdraws, Qt::QueuedConnection );
    connect( this, &BalanceWorker::eventWithdraw, this, &BalanceWorker::onWithdraw, Qt::QueuedConnection );

    QSqlQuery q;
    if ( q.exec("SELECT * FROM deposits") ) {
        while( q.next() ) {
            QString uuid( q.value("remote_id").toString() );
            _transactions_deposits[ uuid ].id = q.value("id").toUInt();
            _transactions_deposits[ uuid ].local_id = q.value("local_id").toString();
            _transactions_deposits[ uuid ].remote_id =q.value("remote_id").toString();
            _transactions_deposits[ uuid ].currency_id = q.value("currency_id").toUInt();
            _transactions_deposits[ uuid ].amount = q.value("amount").toDouble();
            _transactions_deposits[ uuid ].address = q.value("address").toString();
            _transactions_deposits[ uuid ].created_at = q.value("created_at").toDateTime();
            _transactions_deposits[ uuid ].tx =q.value("txhash").toString();
            _transactions_deposits[ uuid ].status = q.value("status").toString();
            _transactions_deposits[ uuid ].fee =q.value("fee").toDouble();
            _transactions_deposits[ uuid ].exchange_id =  assets->getCurrencyExchangeId( _transactions_deposits[ uuid ].currency_id );

        }
    } else {
        qWarning() << q.lastError().text();
    }
    if ( q.exec("SELECT * FROM withdraws") ) {
        while( q.next() ) {
            QString uuid( q.value("remote_id").toString() );
            _transactions_withdraws[ uuid ].id = q.value("id").toUInt();
            _transactions_withdraws[ uuid ].local_id = q.value("local_id").toString();
            _transactions_withdraws[ uuid ].remote_id =q.value("remote_id").toString();
            _transactions_withdraws[ uuid ].currency_id = q.value("currency_id").toUInt();
            _transactions_withdraws[ uuid ].amount = q.value("amount").toDouble();
            _transactions_withdraws[ uuid ].address = q.value("address").toString();
            _transactions_withdraws[ uuid ].created_at = q.value("created_at").toDateTime();
            _transactions_withdraws[ uuid ].tx =q.value("txhash").toString();
            _transactions_withdraws[ uuid ].status = q.value("status").toString();
            _transactions_withdraws[ uuid ].fee =q.value("fee").toDouble();
            _transactions_withdraws[ uuid ].exchange_id =  assets->getCurrencyExchangeId( _transactions_deposits[ uuid ].currency_id );
        }
    } else {
        qWarning() << q.lastError().text();
    }
}

void BalanceWorker::initWorker(Wamp::Session *sess)
{
    sess->subscribe("swift.balance.feed.events",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        const QString event_name( v.at(0).toString() );
        if ( event_name == "UPDATE" ) {
            emit eventUpdate( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "WITHDRAW" ) {
            emit eventWithdraw( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "WITHDRAWS" ) {
            emit eventWithdraws( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "DEPOSIT" ) {
             emit eventDeposit( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        } else if ( event_name == "DEPOSITS" ) {
             emit eventDeposits( QJsonDocument::fromJson( v.at(1).toString().toUtf8() ).object() );
        }
    });

    sess->provide("swift.balance.rpc.get", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        QVariantList ret;
        QMap<quint32, balancesStorage> st( storage() );
        for( auto it = st.begin(); it != st.end(); it++ ) {
            if ( v.isEmpty() || v.contains( QString::number( it.key() ) ) ) {
                // Id, available, total, usd, coin_id, exchange_id
                ret.push_back( QVariantList({
                                                QString::number( it.key() ),
                 QString::number( it->available, 'f', 8),
                 QString::number( it->total, 'f', 8),
                 QString::number( it->usd, 'f', 8), QString::number( it->coin_id ),
                 QString::number( it->exchange_id )}));
            }
        }
        return ret;
    });
    sess->provide("swift.balance.rpc.withdraw.get", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m);
        QJsonObject ret;
        for( auto it = _transactions_withdraws.begin(); it != _transactions_withdraws.end(); it++ ) {
            if ( it->id == v.at(0).toString().toUInt() ) {
                ret = it->toJson();
            }
        }
        const QString r( QJsonDocument( ret ).toJson(QJsonDocument::Compact ));
        return r;
    });

    sess->provide("swift.balance.rpc.withdraws", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        QJsonArray ret;
        if ( v.isEmpty() ) {
            for( auto it = _transactions_withdraws.begin(); it!= _transactions_withdraws.end(); it++ ) {
                ret.push_back( it->toJson() );
            }
        } else {
            if( v.length() >= 2 ) {
                if ( !v.at(1).toStringList().isEmpty() ) {
                    const QStringList exchs( v.at(1).toStringList() );
                    for( auto it = _transactions_withdraws.begin(); it!= _transactions_withdraws.end(); it++ ) {
                        if ( exchs.contains( QString::number( it->exchange_id) ) ) {
                            ret.push_back( it->toJson() );
                        }
                    }
                } else {
                    const QStringList exchs( v.at(0).toStringList() );
                    for( auto it = _transactions_withdraws.begin(); it!= _transactions_withdraws.end(); it++ ) {
                        if ( exchs.contains( QString::number( it->exchange_id) ) ) {
                            ret.push_back( it->toJson() );
                        }
                    }
                }
            } else {
                const QStringList exchs( v.at(0).toStringList() );
                for( auto it = _transactions_withdraws.begin(); it!= _transactions_withdraws.end(); it++ ) {
                    if ( exchs.contains( QString::number( it->exchange_id) ) ) {
                        ret.push_back( it->toJson() );
                    }
                }
            }
        }

        const QString r( QJsonDocument( ret ).toJson(QJsonDocument::Compact ));
        return r;
    });

    sess->provide("swift.balance.rpc.deposit.get", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m);
        QJsonObject ret;
        for( auto it = _transactions_deposits.begin(); it != _transactions_deposits.end(); it++ ) {
            if ( it->id == v.at(0).toString().toUInt() ) {
                ret = it->toJson();
                ret["exchange_id"] = QString::number( assets->getCurrencyExchangeId( it->currency_id ) );
            }
        }
        const QString r( QJsonDocument( ret ).toJson(QJsonDocument::Compact ));
        return r;
    });
    sess->provide("swift.balance.rpc.deposits", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        QJsonArray ret;
        if ( v.isEmpty() ) {
            for( auto it = _transactions_deposits.begin(); it!= _transactions_deposits.end(); it++ ) {
                ret.push_back( it->toJson() );
            }
        } else {
            if( v.length() >= 2 ) {
                if ( !v.at(1).toStringList().isEmpty() ) {
                    const QStringList exchs( v.at(1).toStringList() );
                    for( auto it = _transactions_deposits.begin(); it!= _transactions_deposits.end(); it++ ) {
                        if ( exchs.contains( QString::number( it->exchange_id) ) ) {
                            ret.push_back( it->toJson() );
                        }
                    }
                } else {
                    const QStringList exchs( v.at(0).toStringList() );
                    for( auto it = _transactions_deposits.begin(); it!= _transactions_deposits.end(); it++ ) {
                        if ( exchs.contains( QString::number( it->exchange_id) ) ) {
                            ret.push_back( it->toJson() );
                        }
                    }
                }
            } else {
                const QStringList exchs( v.at(0).toStringList() );
                for( auto it = _transactions_deposits.begin(); it!= _transactions_deposits.end(); it++ ) {
                    if ( exchs.contains( QString::number( it->exchange_id) ) ) {
                        ret.push_back( it->toJson() );
                    }
                }
            }
        }
        const QString r( QJsonDocument( ret ).toJson(QJsonDocument::Compact ));
        return r;
    });

    sess->provide("swift.balance.rpc.required", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        QJsonObject ret;

        if ( !v.isEmpty() ) {
            for( auto it = v.begin(); it != v.end(); it++ ) {
                ret[QString::number( it->toUInt()) ] = QString::number( _storage.value(it->toUInt()).required, 'f',8);
            }
        } else {
            for( auto it = _storage.begin(); it != _storage.end(); it++ ) {
                ret[QString::number( it.key() ) ] = QString::number(it->required, 'f',8);
            }
        }
        const QString r( QJsonDocument( ret ).toJson(QJsonDocument::Compact ));
        return r;
    });

    sess->provide("swift.balance.rpc.history", [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        QVariantList ret;
        return ret;
    });


    sess->provide("swift.trade.rpc.rates", [&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        Q_UNUSED(v);
        QJsonArray _ret( getRatesDiff() );

        const QString r( QJsonDocument( _ret ).toJson( QJsonDocument::Compact ) );
        return r;
    });

    QTimer::singleShot( 5000, this, &BalanceWorker::updateRates );
    QTimer::singleShot( 15000, this, &BalanceWorker::requestWithdraws );
    QTimer::singleShot( 15000, this, &BalanceWorker::requestDeposits );
    QTimer::singleShot( 10000, this, &BalanceWorker::requestBalances );
    QTimer::singleShot( 12000, this, &BalanceWorker::updateRequested );
    processDepositsQueue();
    processWithdrawsQueue();
    processBalancesQueue();
}

QString BalanceWorker::getWorkerName() const
{
    return "balance";
}

void BalanceWorker::requestBalances() {
    const QStringList exchanges( getConfigValue("EXCHANGES").split(",") );
    if ( !exchanges.isEmpty() ) {
        for( auto it = exchanges.begin(); it!= exchanges.end(); it++ ) {
            const QString ename( *it );
            const quint32 eid = assets->getExchangeId( ename );
            if ( assets->isSeparatedApi( eid ) ) {
                const QList<quint32> cids( assets->getExchangeCurrencies( eid ) );
                for( auto iit = cids.begin(); iit != cids.end(); iit++ ) {
                    _queued_balances_get.enqueue({eid, *iit });
                }
            } else {
                const quint32 uid = session->call("swift.api.rpc."+QString::number(eid)+".balance.get").toUInt();
                if ( uid < 1000 ) {
                    qWarning() << eid << "DISABLE EXCHANGE";
                }
            }
        }
    }
    QTimer::singleShot( 15000, this, &BalanceWorker::requestBalances );
}

void BalanceWorker::processWithdrawsQueue() {
    if ( _queued_withdraws_get.isEmpty() ) {
        QTimer::singleShot( 1000, this, &BalanceWorker::processWithdrawsQueue );
    } else {
        const QPair<quint32,quint32> ids = _queued_withdraws_get.dequeue();
        const QJsonObject params({{"currency_id",QString::number( ids.second )}});
        const QString p( QJsonDocument( params ).toJson( QJsonDocument::Compact ) );
        const quint32 uid = session->call("swift.api.rpc."+QString::number(ids.first)+".balance.withdraws", {p}).toUInt();
        if ( uid < 1000 ) {
            qWarning() << ids.first << "DISABLE EXCHANGE";
        }
        QTimer::singleShot( 1500, this, &BalanceWorker::processWithdrawsQueue );
    }
}

void BalanceWorker::processBalancesQueue() {
    if ( _queued_balances_get.isEmpty() ) {
        QTimer::singleShot( 1000, this, &BalanceWorker::processBalancesQueue );
    } else {
        const QPair<quint32,quint32> ids = _queued_balances_get.dequeue();
        const QJsonObject params({{"currency_id",QString::number( ids.second )}});
        const QString p( QJsonDocument( params ).toJson( QJsonDocument::Compact ) );
        const quint32 uid = session->call("swift.api.rpc."+QString::number(ids.first)+".balance.get", {p}).toUInt();
        if ( uid < 1000 ) {
            qWarning() << ids.first << "DISABLE EXCHANGE";
        }
        QTimer::singleShot( 1500, this, &BalanceWorker::processBalancesQueue );
    }
}

void BalanceWorker::processDepositsQueue() {
    if ( _queued_deposits_get.isEmpty() ) {
        QTimer::singleShot( 1000, this, &BalanceWorker::processDepositsQueue );
    } else {
        const QPair<quint32,quint32> ids = _queued_deposits_get.dequeue();
        const QJsonObject params({{"currency_id",QString::number( ids.second )}});
        const QString p( QJsonDocument( params ).toJson( QJsonDocument::Compact ) );
        const quint32 uid = session->call("swift.api.rpc."+QString::number(ids.first)+".balance.deposits", {p}).toUInt();
        if ( uid < 1000 ) {
            qWarning() << ids.first << "DISABLE EXCHANGE";
        }
        QTimer::singleShot( 1500, this, &BalanceWorker::processDepositsQueue );
    }
}

void BalanceWorker::requestWithdraws() {
    const QStringList exchanges( getConfigValue("EXCHANGES").split(",") );
    if ( !exchanges.isEmpty() ) {
        for( auto it = exchanges.begin(); it!= exchanges.end(); it++ ) {
            const QString ename( *it );
            const quint32 eid = assets->getExchangeId( ename );
            if ( assets->isSeparatedApi( eid ) ) {
                const QList<quint32> cids( assets->getExchangeCurrencies( eid ) );
                for( auto iit = cids.begin(); iit != cids.end(); iit++ ) {
                    _queued_withdraws_get.enqueue({eid, *iit });
                }
            } else {
                const quint32 uid = session->call("swift.api.rpc."+QString::number(eid)+".balance.withdraws").toUInt();
                if ( uid < 1000 ) {
                    qWarning() << eid << "DISABLE EXCHANGE";
                }
            }
        }
    }
    QTimer::singleShot( 55000, this, &BalanceWorker::requestWithdraws );
}

void BalanceWorker::requestDeposits() {
    const QStringList exchanges( getConfigValue("EXCHANGES").split(",") );
    if ( !exchanges.isEmpty() ) {
        for( auto it = exchanges.begin(); it!= exchanges.end(); it++ ) {
            const QString ename( *it );
            const quint32 eid = assets->getExchangeId( ename );
            if ( assets->isSeparatedApi( eid ) ) {
                const QList<quint32> cids( assets->getExchangeCurrencies( eid ) );
                for( auto iit = cids.begin(); iit != cids.end(); iit++ ) {
                    _queued_deposits_get.enqueue({eid, *iit });
                }
            } else {
                const quint32 uid = session->call("swift.api.rpc."+QString::number(eid)+".balance.deposits").toUInt();
                if ( uid < 1000 ) {
                    qWarning() << eid << "DISABLE EXCHANGE";
                }
            }
        }
    }
    QTimer::singleShot( 45000, this, &BalanceWorker::requestDeposits );
}

void BalanceWorker::onUpdate(const QJsonObject &data) {
    const quint32 eid = data.value("exchange_id").toString().toUInt();
    const QJsonArray j_records( data.value("items").toArray() );
    for( auto it = j_records.begin(); it != j_records.end(); it++ ) {
        const QJsonObject j_itm( it->toObject( ) );
        const quint32 cid = j_itm.value("currency_id").toString().toUInt();
        _storage[ cid ].total = j_itm.value("total").toString().toDouble();
        _storage[ cid ].available = j_itm.value("available").toString().toDouble();
        if ( assets->getCurrencyCoin( cid ) == 7 ) {
            _storage[ cid ].usd = _storage[ cid ].total;
        } else {
            _storage[ cid ].usd = __apair_rates.value( assets->getCurrencyCoin(cid ) == 1 ? 1 : 2 ) * j_itm.value("total").toString().toDouble();
        }
        _storage[ cid ].currency_id = cid;
        _storage[ cid ].coin_id = assets->getCurrencyCoin(cid );
        _storage[ cid ].exchange_id = eid;
    }
}

void BalanceWorker::updateRates() {
    QMap<quint32, QMap<quint32,double>> _asks;
    QMap<quint32, QMap<quint32,double>> _bids;
    QSqlQuery q(_ch);
    if ( q.exec("select d,toHour(ts) as h,avg(rate) as r,pair from asks WHERE ts>=toStartOfInterval(now(), INTERVAL "+SwiftLib::getSettings()->value("RATES_INTERVAL","3").toString()+" hour)  group by d,pair,toHour(ts)") ) {
        while( q.next() ) {
            const quint32 pid( q.value("pair").toUInt() );
            const quint32 h( q.value("h").toUInt() );
            const double r( q.value("r").toDouble());
            _asks[pid][h] = r;
        }
    }
    if ( q.exec("select d,toHour(ts) as h,avg(rate) as r,pair from bids WHERE ts>=toStartOfInterval(now(), INTERVAL "+SwiftLib::getSettings()->value("RATES_INTERVAL","3").toString()+" hour)  group by d,pair,toHour(ts)") ) {
        while( q.next() ) {
            const quint32 pid( q.value("pair").toUInt() );
            const quint32 h( q.value("h").toUInt() );
            const double r( q.value("r").toDouble());
            _bids[pid][h] = r;
        }
    }
    QMap<quint32, QList<double>> _ar_rates;

    for( auto it = _asks.begin(); it != _asks.end(); it++ ) {
        const quint32 pid = it.key();
        const double current_buy_rate = _asks[pid].last();
        const double prev_buy_rate = _asks[pid].first();
        const double buy_change = ( current_buy_rate - prev_buy_rate ) / prev_buy_rate * 100;
        const double current_sell_rate = _bids[pid].last();
        const double prev_sell_rate = _bids[pid].first();
        const double sell_change = ( current_sell_rate - prev_sell_rate ) / prev_sell_rate * 100;
        _rates_stats[ pid ].buy = current_buy_rate;
        _rates_stats[ pid ].sell = current_sell_rate;
        _rates_stats[ pid ].prev_buy = prev_buy_rate;
        _rates_stats[ pid ].prev_sell = prev_sell_rate;
        _rates_stats[ pid ].diff_sell = sell_change;
        _rates_stats[ pid ].diff_buy = buy_change;
        _rates_stats[ pid ].pid = pid;
        _rates_stats[ pid ].orders = 0;
        _ar_rates[ assets->getMarketArbitragePairId( pid) ].push_back( current_sell_rate );
        _ar_rates[ assets->getMarketArbitragePairId( pid) ].push_back( current_buy_rate );
    }
    const QList<quint32> _k( _ar_rates.keys() );
    if ( !_k.isEmpty() ) {
        for( auto it = _k.begin(); it != _k.end(); it++ ) {
            __apair_rates[ *it ] = std::accumulate( _ar_rates[ *it ].begin(), _ar_rates[ *it ].end(), 0.0 ) / _ar_rates[ *it ].count();
        }
    }
    QSqlQuery q2("SELECT count(*) as cnt, pair_id FROM orders WHERE date(ts)=curdate() GROUP by pair_id");
    if ( q2.exec() ) {
        while( q2.next() ) {
            const quint32 pid = q2.value("pair_id").toUInt();
            const quint32 ocnt = q2.value("cnt").toUInt();
            _rates_stats[ pid ].orders = ocnt;
        }
    }
    QTimer::singleShot( 60000, this, &BalanceWorker::updateRates );
}



void BalanceWorker::onDeposit(const QJsonObject &data) {
    onDeposits( data );
}

void BalanceWorker::onDeposits(const QJsonObject &data) {
    const QJsonArray records( data.value("items").toArray() );
    QSqlQuery q;
    q.prepare("INSERT INTO `deposits` (`local_id`,`remote_id`,`currency_id`,`amount`,`address`,`created_at`,`txhash`,`status`,`fee`) VALUES (:local_id,:remote_id,:currency_id,:amount,:address,:created_at,:txhash,:status,:fee) ON DUPLICATE KEY UPDATE local_id=:local_id, status=:status,txhash=:txhash,created_at=:created_at");
    for ( auto it = records.begin(); it != records.end(); it++ ) {
        const QString remote_uuid_generated( QCryptographicHash::hash( QJsonDocument(it->toObject() ).toJson( QJsonDocument::Compact ), QCryptographicHash::Sha512 ).toHex() );
        const QString txhash( it->toObject().value("tx").toString() );
        const QString remote_uuid( it->toObject().contains("remote_id") ? it->toObject().value("remote_id").toString() : it->toObject().contains("tx") ? txhash : remote_uuid_generated );

        QString uuid( remote_uuid.isEmpty() ? remote_uuid_generated : remote_uuid );
        if ( uuid.length() > 250 ) {
            uuid = uuid.mid(0, 250 );
        }
        if ( !_transactions_deposits.contains( uuid ) || _transactions_deposits[ uuid ].status != it->toObject().value("status").toString() ) {
            _transactions_deposits[ uuid ].id = 0;
            _transactions_deposits[ uuid ].local_id = uuid;
            _transactions_deposits[ uuid ].remote_id = uuid;
            _transactions_deposits[ uuid ].currency_id = it->toObject().value("currency_id").toString().toUInt();
            _transactions_deposits[ uuid ].amount = it->toObject().value("amount").toString().toDouble();
            _transactions_deposits[ uuid ].address = it->toObject().value("address").toString("#");
            _transactions_deposits[ uuid ].created_at = QDateTime::fromSecsSinceEpoch( it->toObject().value("created_at").toString().toUInt(), QTimeZone("UTC+03:00") );
            _transactions_deposits[ uuid ].tx = it->toObject().value("tx").toString("#");;
            _transactions_deposits[ uuid ].status = it->toObject().value("status").toString();;
            _transactions_deposits[ uuid ].fee = 0;

            q.bindValue(":remote_id",uuid );
            q.bindValue(":local_id", uuid );
            q.bindValue(":currency_id", _transactions_deposits[ uuid ].currency_id );
            q.bindValue(":amount", _transactions_deposits[ uuid ].amount );
            q.bindValue(":address",_transactions_deposits[ uuid ].address );
            q.bindValue(":created_at", _transactions_deposits[ uuid ].created_at );
            q.bindValue(":txhash", _transactions_deposits[ uuid ].tx );
            q.bindValue(":status",_transactions_deposits[ uuid ].status );
            q.bindValue(":fee", 0 );
            if ( !q.exec() ) {
                qWarning() << "DEPOSIT HISTORY ERROR SAVE" << q.lastError().text() << it->toObject() << uuid;
            } else {
                _transactions_deposits[ uuid ].id  = q.lastInsertId().toUInt();
                QJsonObject evit( it->toObject() );
                evit["id"] = QString::number( _transactions_deposits[ uuid ].id  );
                evit["remote_id"] = _transactions_deposits[ uuid ].remote_id;
                evit["local_id"] = _transactions_deposits[ uuid ].local_id;

                QJsonObject j_event({{"exchange_id",QString::number( data.value("exchange_id").toString().toUInt())},{"items",QJsonArray({evit})}});
                const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                session->publish("swift.balance.feed.events", {"DEPOSIT", _event_data});
            }
        }
    }
    q.finish();
}

void BalanceWorker::onWithdraw(const QJsonObject &data) {
    onWithdraws( data );
}

void BalanceWorker::onWithdraws(const QJsonObject &data) {
    const QJsonArray records( data.value("items").toArray() );
    QSqlQuery q;
    q.prepare("INSERT INTO `withdraws` (`local_id`,`remote_id`,`currency_id`,`amount`,`address`,`created_at`,`txhash`,`status`,`fee`) VALUES (:local_id,:remote_id,:currency_id,:amount,:address,:created_at,:txhash,:status,:fee) ON DUPLICATE KEY UPDATE local_id=:local_id, status=:status,txhash=:txhash,created_at=:created_at");
    for ( auto it = records.begin(); it != records.end(); it++ ) {
        const QString remote_uuid_generated( QCryptographicHash::hash( QJsonDocument(it->toObject() ).toJson( QJsonDocument::Compact ), QCryptographicHash::Sha512 ).toHex() );
        const QString txhash( it->toObject().value("tx").toString() );
        const QString remote_uuid( it->toObject().contains("remote_id") ? it->toObject().value("remote_id").toString() : it->toObject().contains("tx") ? txhash : remote_uuid_generated );

        QString uuid( remote_uuid.isEmpty() ? remote_uuid_generated : remote_uuid );
        if ( uuid.length() > 250 ) {
            uuid = uuid.mid(0, 250 );
        }
        if ( !_transactions_withdraws.contains( uuid ) || _transactions_withdraws[ uuid ].status != it->toObject().value("status").toString() ) {
            _transactions_withdraws[ uuid ].id = 0;
            _transactions_withdraws[ uuid ].local_id = uuid;
            _transactions_withdraws[ uuid ].remote_id = uuid;
            _transactions_withdraws[ uuid ].currency_id = it->toObject().value("currency_id").toString().toUInt();
            _transactions_withdraws[ uuid ].amount = it->toObject().value("amount").toString().toDouble();
            _transactions_withdraws[ uuid ].address = it->toObject().value("deposit_address").toString();
            _transactions_withdraws[ uuid ].created_at = QDateTime::fromSecsSinceEpoch( it->toObject().value("created_at").toString().toUInt(), QTimeZone("UTC+03:00") );
            _transactions_withdraws[ uuid ].tx = it->toObject().value("tx").toString("#");
            _transactions_withdraws[ uuid ].status = it->toObject().value("status").toString();
            _transactions_withdraws[ uuid ].fee = 0;

            q.bindValue(":remote_id",uuid );
            q.bindValue(":local_id", uuid );
            q.bindValue(":currency_id", _transactions_withdraws[ uuid ].currency_id );
            q.bindValue(":amount", _transactions_withdraws[ uuid ].amount );
            q.bindValue(":address",_transactions_withdraws[ uuid ].address );
            q.bindValue(":created_at", _transactions_withdraws[ uuid ].created_at );
            q.bindValue(":txhash", _transactions_withdraws[ uuid ].tx );
            q.bindValue(":status",_transactions_withdraws[ uuid ].status );
            q.bindValue(":fee", assets->getCurrencyWithdrawFee( _transactions_withdraws[ uuid ].currency_id ) );
            if ( !q.exec() ) {
                qWarning() << "WITHDRAW HISTORY ERROR SAVE" << q.lastError().text() << it->toObject() << uuid;
            } else {
                QJsonObject evit( it->toObject() );
                _transactions_withdraws[ uuid ].id  = q.lastInsertId().toUInt();
                evit["id"] = QString::number( _transactions_withdraws[ uuid ].id  );
                evit["remote_id"] = _transactions_withdraws[ uuid ].remote_id;
                evit["local_id"] = _transactions_withdraws[ uuid ].local_id;

                QJsonObject j_event({{"exchange_id",QString::number( data.value("exchange_id").toString().toUInt())},{"items",QJsonArray({evit})}});
                const QString _event_data( QJsonDocument( j_event ).toJson( QJsonDocument::Compact ) );
                session->publish("swift.balance.feed.events", {"WITHDRAW", _event_data});
            }
        }
    }
    q.finish();
}
