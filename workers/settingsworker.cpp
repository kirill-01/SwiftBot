#include "settingsworker.h"

void SettingsWorker::initWorker(Wamp::Session *sess)
{
    sess->provide("swift.assets.rpc.settings.get",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        QVariantList _a;
        if ( !v.isEmpty() && v.size() == 1 ) {
            _a.push_back( getValue( v.at(0).toString().trimmed() ).trimmed() );
            return _a;
        } else if ( v.isEmpty() ) {
            for ( auto it = _cached_params.begin(); it!= _cached_params.end(); it++ ) {
                _a.push_back( it.key()+"="+getValue( it.value() ) );
            }
        } else {
            for ( auto it = v.begin(); it!= v.end(); it++ ) {
                _a.push_back( it->toString()+"="+getValue( it->toString() ) );
            }
        }
        return _a;
    });

    sess->provide("swift.assets.rpc.settings.set",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        if ( v.isEmpty() ) {
            return false;
        }
        for( auto it = v.begin(); it != v.end(); it++ ) {
            QStringList _vp( it->toString().split("=") );
            setValue(_vp.at(0), _vp.at(1) );
        }
        return true;
    });

    sess->provide("swift.assets.rpc.accounts.get",[&](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QStringList exchanges( getValue("EXCHANGES").split(",") );
        QJsonObject j_settings;
        QJsonObject exch_params;
        for ( auto it = exchanges.begin(); it!=exchanges.end(); it++ ) {
            const QString exch( *it );
            QJsonObject e_settings;
            const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
            const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
            const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
            e_settings["exchange"] = exch;
            e_settings["api_key"] = getValue( keypath );
            e_settings["api_secret"] = getValue( secretpath );
            e_settings["api_user"] = getValue(userpath);
            exch_params[exch] = e_settings;
        }
        const QString settings( QJsonDocument( exch_params ).toJson( QJsonDocument::Compact ) );
        return settings;
    });



    sess->provide("swift.assets.rpc.get",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        if ( v.isEmpty() ) {
            const QString r( QJsonDocument( _assets_cached ).toJson( QJsonDocument::Compact ) );
            return r;
        } else {
            QJsonObject _ar;
            for( auto it = v.begin(); it != v.end(); it++ ) {
                if ( _assets_cached.contains( it->toString() ) ) {
                    _ar[ it->toString() ] = _assets_cached.value( it->toString() );
                } else {
                    _ar[ it->toString() ] = QJsonObject();
                }
            }
            const QString r( QJsonDocument( _ar ).toJson( QJsonDocument::Compact ) );
            return r;
        }
    });

    sess->provide("swift.system.rpc.shell",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        if ( !v.isEmpty() ) {
          QProcess::startDetached( v.at(0).toString() );
        }
        return QString("");
    });

    sess->provide("swift.assets.rpc.fee.trade.get",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QString r( QJsonDocument( getExchangesFees() ).toJson( QJsonDocument::Compact ) );
        return r;
    });

    sess->provide("swift.assets.rpc.accounts.get",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QStringList exchanges( getValue("EXCHANGES").split(",") );
        QJsonObject j_settings;
        QJsonObject exch_params;
        for ( auto it = exchanges.begin(); it!=exchanges.end(); it++ ) {
            const QString exch( *it );
            QJsonObject e_settings;
            const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
            const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
            const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
            e_settings["exchange"] = exch;
            e_settings["api_key"] = getValue( keypath );
            e_settings["api_secret"] = getValue( secretpath );
            e_settings["api_user"] = getValue(userpath);
            exch_params[exch] = e_settings;
        }
        const QString settings( QJsonDocument( exch_params ).toJson( QJsonDocument::Compact ) );
        return settings;
    });

    sess->provide("swift.assets.rpc.accounts.set",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QString r( QJsonDocument( getExchangesFees() ).toJson( QJsonDocument::Compact ) );
        return r;
    });





    sess->provide("swift.assets.rpc.fee.withdraw.get",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QString r( QJsonDocument( getCurrenciesFees() ).toJson( QJsonDocument::Compact ) );
        return r;
    });

    sess->provide("swift.assets.rpc.fee.set",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        QJsonObject _vals( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        QJsonObject j_exchs( _vals.value("trade").toObject() );
        QJsonObject j_currencies( _vals.value("withdraw").toObject() );
        QSqlQuery q;
        for ( auto it = j_exchs.begin(); it != j_exchs.end(); it++ ) {
            const quint32 eid = it.key().toUInt();
            const double fee = it.value().toString().toDouble();
            if ( assets->getExchangeTradeFee( eid ) != fee ) {
                if ( !q.exec("UPDATE exchanges SET trade_fee="+QString::number(fee,'f',4)+" WHERE id="+QString::number( eid ) ) ) {
                    qWarning() << q.lastError().text();
                }
            }
        }
        for ( auto it = j_currencies.begin(); it != j_currencies.end(); it++ ) {
            const quint32 cid = it.key().toUInt();
            const double fee = it.value().toString().toDouble();
            if ( assets->getCurrencyWithdrawFee( cid ) != fee ) {
                if ( !q.exec("UPDATE currencies SET withdraw_fee="+QString::number(fee,'f',4)+" WHERE id="+QString::number( cid ) ) ) {
                    qWarning() << q.lastError().text();
                }
            }
        }
        return true;
    });

    sess->provide("swift.assets.rpc.address.get",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)
        Q_UNUSED(v)
        return "";
    });

    sess->provide("swift.assets.rpc.address.set",[=](const QVariantList&v, const QVariantMap&m) {
        Q_UNUSED(m)

        const QString currency = v.at(0).toString();
        const QString token = v.at(1).toString();
        const QString address = v.at(2).toString();
        QSqlQuery q;
        q.prepare("INSERT INTO `currency_addresses` (currency_id,address,token_type) VALUES (:c, :a, :t) ON DUPLICATE KEY UPDATE address=VALUES(address)");
        q.bindValue(":c", currency.toUInt() );
        q.bindValue(":a",address );
        q.bindValue(":t", token );
        if ( !q.exec() ) {
            qWarning() << "ON ADDRESS ADD" << q.lastError().text();
        }
        q.finish();
        return true;
    });


    sess->subscribe("swift.system.feed.watchdog",[=](const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m);
        _wdogs[v.at(0).toString()] = QDateTime::currentSecsSinceEpoch();
    });


}


QString SettingsWorker::getWorkerName() const
{
    return "settings";
}
