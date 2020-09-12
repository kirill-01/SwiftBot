#include "moneybalancer.h"

MoneyBalancer::MoneyBalancer(QObject *parent) : QObject(parent), assets( SwiftLib::getAssets() ),session(nullptr)
{
    QSqlQuery q("CREATE TABLE IF NOT EXISTS `refills` (\
                `id` int(11) NOT NULL AUTO_INCREMENT,\
                `from_id` int(11) NOT NULL,\
                `to_id` int(11) NOT NULL,\
                `amount` double(64,8) NOT NULL DEFAULT '0.00000000',\
                `status` tinyint(4) NOT NULL DEFAULT '0',\
                `tx1` varchar(64) NOT NULL DEFAULT '0',\
                `tx2` varchar(64) NOT NULL DEFAULT '0',\
                `fee` double(64,8) DEFAULT NULL,\
                `ts_create` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,\
                `ts_update` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,\
                `is_manual_start` tinyint(4) NOT NULL DEFAULT '1',\
                PRIMARY KEY (`id`),\
                UNIQUE KEY `id_UNIQUE` (`status`,`id`)\
              ) ENGINE=InnoDB DEFAULT CHARSET=utf8;");
    if ( !q.exec() ) {
        qWarning() << q.lastError().text();
    }
    // Kraken
    _fees[19] = 0.005;
    _min_withdrawals[19] = 0.02;
    // Kucoin
    _fees[19] = 0.005;
    _min_withdrawals[19] = 0.02;
    // Bittrex
    _fees[1] = 0.0005;
    _min_withdrawals[1] = 0.02;
    // LBank
    _fees[8] = 0.003;
    _min_withdrawals[8] = 0.02;
    _fees[10] = 2.5;
    _min_withdrawals[10] = 200;

    _fees[18] = 2.5;
    _min_withdrawals[18] = 200;

    _fees[22] = 2.5;
    _min_withdrawals[22] = 200;

    // Binance
    _fees[19] = 0.0005;
    _min_withdrawals[19] = 0.02;
    // Kucoin
    _fees[21] = 0.0005;
    _min_withdrawals[21] = 0.02;
}

void MoneyBalancer::loadData() {

    /*
    if ( QSqlQuery("DELETE FROM refills where date(ts_create) != curdate() OR status > 3").exec() ) {

    }
    QHash<quint32, double> _current_withdtaw_pending;
    QHash<quint32, double> _current_deposit_pending;
    QHash<quint32, double> _current_withdtaw_ready;
    QHash<quint32, double> _current_withdtaw_waiting;

    QSqlQuery q("SELECT * FROM refills WHERE status < 3");
    QList<quint32> _outdated;
    QList<QPair<quint32,quint32>> _active_ref;
    _reqs = SwiftLib::getRequested();
    QHash<quint32, double>  __balances = assets->getBalances();
    _refill_records.clear();
    if ( q.exec() ) {
        while ( q.next() ) {
            Refill refill;
            refill.id = q.value("id").toUInt();
            refill.from_id = q.value("from_id").toUInt();
            refill.to_id = q.value("to_id").toUInt();
            refill.amount = q.value("amount").toDouble();
            refill.status = q.value("id").toUInt();
            refill.tx1 = q.value("tx1").toString();
            refill.tx2 = q.value("tx2").toString();
            refill.fee = q.value("fee").toDouble();
            refill.ts_create = q.value("ts_create").toDateTime();
            refill.ts_update = q.value("ts_update").toDateTime();
            refill.is_manual_start = q.value("is_manual_start").toUInt();
            if ( refill.status == 1 || refill.status == 2 ) {
                _current_withdtaw_pending[ refill.from_id ] += ( refill.amount );
                _current_deposit_pending[ refill.to_id ] += ( refill.amount - refill.fee );
            } else if ( refill.status == 0 ) {
                _current_withdtaw_ready[refill.from_id ] += ( refill.amount - refill.fee );
                _current_withdtaw_waiting[ refill.to_id ] += ( refill.amount - refill.fee );
            }
            if ( QDateTime::currentSecsSinceEpoch() - refill.ts_create.toSecsSinceEpoch() < 120 ) {
                if ( __balances[ refill.from_id ] >= refill.amount && _reqs[_reqs.keys().first()].value( refill.to_id) >= refill.amount  ) {
                   if ( refill.status < 3 ) {
                       if ( !_active_ref.contains(QPair<quint32,quint32>({refill.from_id, refill.to_id})) ) {
                           _active_ref.push_back( QPair<quint32,quint32>({refill.from_id, refill.to_id}));
                           _refill_records.push_back( refill );
                       } else {
                           _outdated.push_back( refill.id );
                       }
                   }
                } else {
                    _outdated.push_back( refill.id );
                }
            }  else {
                _outdated.push_back( refill.id );
            }

        }
        q.finish();

        if ( !_outdated.isEmpty() ) {
            QStringList strids;
            for( auto it = _outdated.begin(); it != _outdated.end(); it++ ) {
                strids.push_back( QString::number( *it ) );
            }
            QSqlQuery q("UPDATE refills SET status=5 WHERE id in ("+strids.join(",") +")");
            if ( !q.exec() ) {
                qWarning() << q.lastError().text();
            }
            q.finish();
        }
    }
*/
    const double keep_ratio_value = SwiftLib::getSettings()->value("BALANCER_KEEP_RATIO",0 ).toDouble();
    const bool use_all_subaccounts =  SwiftLib::getSettings()->value("BALANCER_USE_ALL",0 ).toInt() == 1;
    double current_btc_amount=0;
    double current_usd_amount=0;

    if ( use_all_subaccounts ) {
        // Использовать total иои available
        current_btc_amount = session->call("swiftbot.balances.get.total.usd.summ", QVariantList({})).toDouble();
        current_btc_amount = session->call("swiftbot.balances.get.total.btc.summ", QVariantList({})).toDouble();
    } else {
        current_btc_amount = session->call("swiftbot.balances.get.available.usd.summ", QVariantList({})).toDouble();
        current_btc_amount = session->call("swiftbot.balances.get.available.btc.summ", QVariantList({})).toDouble();
    }

    if ( keep_ratio_value > 0 ) {
        const double current_rate = session->call("swiftbot.balances.get.rate.btc.usd", QVariantList({})).toDouble();
        const double current_btc_price = current_btc_amount * current_rate;
        const double currenct_ratio = current_btc_price > current_usd_amount ? current_btc_price / current_usd_amount : current_usd_amount / current_btc_price;

        // Рассчитано текущуу соотношение средств и получено заданное...

    }

    const bool offer_manual_refills =  SwiftLib::getSettings()->value("BALANCER_OFFER_MANUAL",0 ).toInt() == 1;

    // Если не предлагать - игнорировать такие маркеты при рассчете
    //
    // Сначала смотрим самые больше скопления выводимой крипты
    // Считаем профит, полученный со времени последней балансировки - стоимость вывода не может быть больше чем профит
    //
    // Если соотноешение слишком разное - считаем это моментом входа на рынок и выравниваем исходя из максимального соотношения 35/65
    // Выравниваем в первый проход ордерами, где это возможно
    //
    //


    /*
    QHash<quint32, QList<QPair<quint32,double>>> _targets;
    for ( auto it = _reqs.begin(); it != _reqs.end(); it++ ) {
        const QHash<quint32,double> _r( it.value() );
        for( auto it2 = _r.begin(); it2 != _r.end(); it2++ ) {
            const quint32 currency = it2.key();
            if ( assets->isToken( currency ) ) {
                const double amount = it2.value();
                const QList<quint32> _rel_currencies( assets->getCoinRelatedCurrencies(currency) );
                for( auto itt = _rel_currencies.begin(); itt != _rel_currencies.end(); itt++ ) {
                    const quint32 currency_to_withdraw = *itt;
                    const quint32 currency_target = currency;
                    double has_balance = _balances[currency_to_withdraw];
                    if ( ( has_balance < amount && has_balance > _min_withdrawals[ currency_to_withdraw ] ) || ( has_balance > amount && assets->isToken( currency_to_withdraw ) ) ) {
                        if ( _min_withdrawals.contains( currency_to_withdraw ) && _fees.contains( currency_to_withdraw ) ) {

                            const double withdraw_with_fee( has_balance < amount ? _min_withdrawals[ currency_to_withdraw ] + _fees.value( currency_to_withdraw ) : amount + _fees.value( currency_to_withdraw ) );
                            if ( _balances[currency_to_withdraw] > withdraw_with_fee && withdraw_with_fee > 0 && _min_withdrawals[currency_to_withdraw] > 0.0 ) {
                                if ( _current_withdtaw_pending[currency_to_withdraw] == 0.0 ) {
                                    if ( _current_deposit_pending[currency_target] == 0.0 && _current_withdtaw_pending[currency_to_withdraw] == 0.0 ) {

                                        _targets[currency_target].push_back( QPair<quint32,double>({currency_to_withdraw, withdraw_with_fee}) );
                                        Refill refill;
                                        refill.from_id = currency_to_withdraw;
                                        refill.to_id = currency_target;
                                        if ( has_balance > withdraw_with_fee * 3 ) {
                                            refill.amount = withdraw_with_fee * 2;
                                        } else {
                                            refill.amount = withdraw_with_fee;
                                        }

                                        refill.status = 0;
                                        refill.tx1 = "";
                                        refill.tx2 = "";
                                        refill.fee = _fees.value( currency_to_withdraw );
                                        refill.is_manual_start = assets->getCurrencyExchangeName( currency_to_withdraw ).toLower() == "lbank" ? 1 : 0;
                                        if ( _current_withdtaw_ready[ currency_to_withdraw ] == 0.0 && _current_withdtaw_waiting[currency_target] == 0.0) {
                                            if ( !_active_ref.contains(QPair<quint32,quint32>({refill.from_id, refill.to_id})) ) {
                                                if ( refill.insert() ) {
                                                    _refill_records.push_back( refill );
                                                    _current_withdtaw_ready[ refill.from_id ] += refill.amount;
                                                    _current_withdtaw_waiting[ refill.to_id ] += refill.amount;
                                                }
                                            }
                                        }

                                    }
                                }
                            }
                        }

                    }
                }
            }

        }
    }

    bool has_starting_task = false;
    QJsonArray j_returns;
    for( auto t = _refill_records.begin(); t != _refill_records.end(); t++ ) {
        if ( !has_starting_task ) {
            if (beginWithdraw( *t ) ) {
                has_starting_task = true;
                qWarning() << "Was started task";
                t->setActive();
            } else {
                t->setFailed();
            }
        }
        j_returns.push_back( t->json() );
    }
    emit publishMessage( "transfer_candidats", j_returns );
    emit transferCandidats( j_returns );*/
}

void MoneyBalancer::onWampSession(Wamp::Session *s) {
    session = s;
    session->provide("balancer.start.manual", [=]( const QVariantList& v, const QVariantMap& m) {
        const quint32 refid = v.at(0).toUInt();
        Refill ref;
        ref.load( refid );
        const QString exchname( assets->getCurrencyExchangeName( ref.from_id ) );
        QJsonObject jparam;
        jparam["currency_id"] = QString::number( ref.from_id );
        jparam["address"] = assets->getDepositAddress( ref.to_id );
        jparam["amount"] = QString::number( ref.amount, 'f', 8 );
        const QString strpar( QJsonDocument( jparam ).toJson( QJsonDocument::Compact ) );
        QVariant res = session->call("apicmd."+exchname+".withdraw", QVariantList({strpar}) );
        const QJsonObject result( QJsonDocument::fromJson( res.toString().toUtf8() ).object() );
        // QJsonObject({"message":"[object Object]","result":{"uuid":"AGBNOLX-VSMO65-UIGAYD"},"success":true})
        Q_UNUSED(m)
        if ( result.value("success").toBool( false ) == true ) {
            ref.tx1 = result.value("result").toObject().value("uuid").toString();
            ref.setActive();
        } else {
            ref.setFailed();
        }
        return true;
    });
}

bool MoneyBalancer::beginWithdraw(const Refill &ref) {

    SwiftLib::getSettings()->sync();
    if ( SwiftLib::getSettings()->value("BALANCER", 0).toUInt() != 1 ) {
        return false;
    }
    if ( assets->getCurrencyExchangeName( ref.from_id ).toLower() == "lbank" ) {
        return false;
    }
    if ( assets->getCurrencyExchangeName( ref.from_id ).toLower() == "cexio" ) {
        return false;
    }
    if ( session != nullptr && session->isJoined() ) {
        const QString exchname( assets->getCurrencyExchangeName( ref.from_id ) );
        QJsonObject jparam;
        jparam["currency_id"] = QString::number( ref.from_id );
        jparam["address"] = assets->getDepositAddress( ref.to_id );
        jparam["amount"] = QString::number( ref.amount, 'f', 8 );
        const QString strpar( QJsonDocument( jparam ).toJson( QJsonDocument::Compact ) );
        QVariant res = session->call("apicmd."+exchname+".withdraw", QVariantList({strpar}) );
        const QJsonObject result( QJsonDocument::fromJson( res.toString().toUtf8()).object() );
        qWarning() << "Money transaction starting result" << result;
        // "{\"info\":{\"error\":[],\"result\":{\"refid\":\"AGBJION-SAOZEM-4OZNES\"}},\"id\":{\"refid\":\"AGBJION-SAOZEM-4OZNES\"}}"
        // {"message":"","result":{"uuid":"d2ffad9d-2c9b-4738-a4d7-624e840ec17b"},"success":true}
        if ( result.value("success").toBool(false) == true ) {
           Refill rr = ref;
           rr.setActive();
           return true;
        } else {
            Refill rr = ref;
            rr.setFailed();
            return false;
        }
    } else {
        return false;
    }
}
