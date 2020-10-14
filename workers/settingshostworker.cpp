#include "wampclient.h"
#include "settingshostworker.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QFile>

#include <QHostInfo>
#include <QSet>

#include "sys/types.h"
#include "sys/sysinfo.h"
#include <QNetworkInterface>
#define APP_VERSION "2.0.2059"

SettingsHostWorker::SettingsHostWorker(QObject *parent) : QObject(parent), service_active(true),assets(SwiftLib::getAssets()),_current_mode(0)
{
    dbname = SwiftLib::getSettings()->value("MYSQL_DBNAME", "swiftbot").toString().trimmed();
    QObject::connect( this, &SettingsHostWorker::markMethodFailed, this, &SettingsHostWorker::onMethodFailed );
    QObject::connect( this, &SettingsHostWorker::markMethodSuccess, this, &SettingsHostWorker::onMethodSuccess );
    QObject::connect( this, &SettingsHostWorker::newParam, this, &SettingsHostWorker::onNewParam, Qt::QueuedConnection );
    loadParams();
    QTimer::singleShot( 1200, this, &SettingsHostWorker::loadAssets );
}

void SettingsHostWorker::setJsonConfig(const QString &conf) {
    const QJsonObject received_json( QJsonDocument::fromJson( conf.toUtf8()).object() );
    const QJsonArray j_exch_params( received_json.value("exchanges_params").toArray() );
    const QJsonArray j_main_params( received_json.value("main_params").toArray() );

    for( auto it = j_main_params.begin(); it != j_main_params.end(); it++ ) {
        QJsonObject j_val( it->toObject() );
        saveParam( j_val.value("param").toString(), j_val.value("value").toString() );
    }

    for( auto it = j_exch_params.begin(); it != j_exch_params.end(); it++ ) {
        QJsonObject j_val( it->toObject() );
        QString exch( j_val.value("exchange").toString().toLower() );
        QString key( j_val.value("key").toString() );
        QString secret( j_val.value("secret").toString( ) );
        QString username( j_val.value("username").toString( ) );
        QString withdraw( j_val.value("withdraw").toString() );
        const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
        const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
        const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
        const QString withdrawpath( QStringList({"api_nodes", exch, "withdraw"}).join("/") );
        saveParam( keypath, key );
        saveParam( secretpath, secret );
        saveParam( userpath, username );
        saveParam( withdrawpath, withdraw );
    }
}

QString SettingsHostWorker::getJsonConfig()  {
    QHash<QString, QString> _params;

    const QStringList exchanges( getParam("EXCHANGES").split(",") );
    QJsonObject j_settings;
    QJsonArray exch_params;
    for ( auto it = exchanges.begin(); it!=exchanges.end(); it++ ) {
        const QString exch( *it );
        QJsonObject e_settings;
        const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
        const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
        const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
        const QString withdtrawpath( QStringList({"api_nodes", exch, "withdraw"}).join("/") );
        e_settings["exchange"] = exch;
        e_settings["key"] = getParam( keypath );
        e_settings["secret"] = getParam( secretpath );
        e_settings["username"] = getParam(userpath);
        e_settings["withdraw"] = getParam( withdtrawpath );
        exch_params.push_back( e_settings );
    }
    j_settings["exchanges_params"] = exch_params;

    QJsonArray main_params;
    const QStringList params( _params.keys() );
    for ( auto it = params.begin(); it != params.end(); it++ ) {
        const QString key = *it;
        const QString value = getParam(key);
        QJsonObject j_itm;
        j_itm["param"] = key;
        j_itm["value"] = value;
        main_params.push_back( j_itm );
    }
    j_settings["main_params"] = main_params;

    const QString str( QJsonDocument( j_settings ).toJson( QJsonDocument::Compact ) );
    return str;
}

QString SettingsHostWorker::getMethodsStatus() {
    QJsonObject j_ret;
    QJsonObject j_s;
    QJsonObject j_e;
    const QHash<QString,quint64> _sm( _success_methods );
    const QHash<QString,quint64> _em( _error_methods );
    for ( auto it = _sm.begin(); it != _sm.end(); it++  ){
        j_s[it.key()] = QString::number( it.value() );
    }
    for ( auto it = _em.begin(); it != _em.end(); it++  ){
        j_e[ it.key() ] =  QString::number( it.value() );
    }
    j_ret["success"] = j_s;
    j_ret["error"] = j_e;
    return QJsonDocument( j_ret ).toJson( QJsonDocument::Compact );
}

QString SettingsHostWorker::getDbName() const {
    return dbname.trimmed();
}

QJsonObject SettingsHostWorker::getMemUsage() {
    QJsonObject j_r;
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    j_r["total_ram"] = QString::number( memInfo.totalram / 1000000 );
    j_r["free_ram"] = QString::number( memInfo.freeram / 1000000 );
    const quint64 used = memInfo.totalram - memInfo.freeram;
    const double used_pcnt = static_cast<double>( used / 1000000 ) / static_cast<double>(memInfo.totalram / 1000000 ) * 100;
    j_r["used_ram"] = QString::number( used / 1000000 );
    j_r["used_pcnt"] = QString::number( used_pcnt, 'f', 2 );
    return j_r;
}

void SettingsHostWorker::defineServiceMode() {

    _current_mode = SwiftLib::getSettings()->value("SYSTEM_MODE", 4).toUInt();

    return;
    if ( _balances_collected && _windows_data_collected ) {
        if ( _has_small_balances ) {
            _current_mode = static_cast<quint32>(SwiftLib::SystemModes::TradeExactWindowsMode);
        } else {
            _current_mode = static_cast<quint32>(SwiftLib::SystemModes::TradeAllWindowsMode);
        }
    } else {

        _current_mode = static_cast<quint32>(SwiftLib::SystemModes::CollectStatsMode);
    }
}

void SettingsHostWorker::onMethodSuccess(const QString &method) {
    _success_methods[method] = QDateTime::currentSecsSinceEpoch();
    if ( _error_methods.contains( method ) ) {
        _error_methods.remove( method );
    }
}

void SettingsHostWorker::onMethodFailed(const QString &method) {
        _error_methods[method] = QDateTime::currentSecsSinceEpoch();
    if ( _success_methods.contains( method ) ) {
        _success_methods.remove( method );
    }
}

void SettingsHostWorker::onNewParam(const QString &param, const QString &value) {
    saveParam( param.trimmed(), value.trimmed() );
}

void SettingsHostWorker::loadParams() {
    QSqlQuery q;
    if ( q.exec("SELECT * FROM `settings_storage`") ) {
        while ( q.next() ) {
            _cached_params.insert( q.value("param").toString(), q.value("value").toString() );
        }
    } else {
        qWarning() << "ON GET PARAM" << q.lastError().text();
    }
    QTimer::singleShot( 30000, this, &SettingsHostWorker::loadParams );
}

void SettingsHostWorker::loadAssets() {
    QJsonObject j_assets;
    QJsonArray a_coins;
    QJsonArray a_exchanges;
    QJsonArray a_pairs;
    QJsonArray a_arb_pairs;
    QJsonArray a_currencies;

    QSqlQuery q("SELECT * FROM coins");
    if ( q.exec() ) {
        while( q.next() ) {
            QJsonObject jitm;
            jitm["id"] = QString::number( q.value("id").toUInt() );
            jitm["name"] = q.value("name").toString();
            jitm["full_name"] =  q.value("full_name").toString();
            jitm["is_enabled"] = QString::number( q.value("is_enabled").toUInt() );
            a_coins.push_back( jitm );
        }
    }
    q.finish();

    j_assets["coins"] = a_coins;
    QSqlQuery q2("SELECT * FROM pairs");
    if ( q2.exec() ) {
        while( q2.next() ) {
            QJsonObject jitm;
            jitm["id"] = QString::number( q2.value("id").toUInt() );
            jitm["name"] = q2.value("name").toString();
            jitm["exchange_id"] = QString::number( q2.value("exchange_id").toUInt() );
            jitm["arbitrage_pair_id"] = QString::number( q2.value("arbitrage_pair_id").toUInt() );
            jitm["is_enabled"] = QString::number( q2.value("is_enabled").toUInt() );
            jitm["base_currency_id"] = QString::number( q2.value("base_currency_id").toUInt() );
            jitm["market_currency_id"] = QString::number( q2.value("market_currency_id").toUInt() );
            a_pairs.push_back( jitm );
        }
    }
    q2.finish();

    QSqlQuery q3("SELECT * FROM arbitrage_pairs");
    if ( q3.exec() ) {
        while( q3.next() ) {
            QJsonObject jitm;
            jitm["id"] = QString::number( q3.value("id").toUInt() );
            jitm["base_coin_id"] = QString::number( q3.value("base_coin_id").toUInt() );
            jitm["price_coin_id"] = QString::number( q3.value("price_coin_id").toUInt() );
            jitm["name"] = q3.value("name").toString();
            jitm["is_enabled"] = QString::number( q3.value("is_enabled").toUInt() );
            a_arb_pairs.push_back( jitm );
        }
    }
    q3.finish();


    QSqlQuery q4("SELECT * FROM exchanges");
    if ( q4.exec() ) {
        while( q4.next() ) {
            QJsonObject jitm;
            jitm["id"] = QString::number( q4.value("id").toUInt() );
            jitm["name"] = q4.value("name").toString();
            jitm["is_enabled"] = QString::number( q4.value("is_enabled").toUInt() );
            jitm["fee"] = QString::number( q4.value("trade_fee").toDouble(), 'f', 4 );
            a_exchanges.push_back( jitm );
        }
    }
    q4.finish();

    QSqlQuery q5("SELECT * FROM currencies");
    if ( q5.exec() ) {
        while( q5.next() ) {
            QJsonObject jitm;
            jitm["id"] = QString::number( q5.value("id").toUInt() );
            jitm["coin_id"] = QString::number( q5.value("coin_id").toUInt() );
            jitm["exchange_id"] = QString::number( q5.value("exchange_id").toUInt() );
            jitm["name"] = q5.value("name").toString();
            jitm["is_enabled"] = QString::number( q5.value("is_enabled").toUInt() );
            jitm["fee"] = QString::number( q5.value("withdraw_fee").toDouble(), 'f', 4 );
            a_currencies.push_back( jitm );
        }
    }
    q5.finish();

    j_assets["exchanges"] = a_exchanges;
    j_assets["pairs"] = a_pairs;
    j_assets["arbitrage_pairs"] = a_arb_pairs;
    j_assets["currencies"] = a_currencies;
    j_assets_cached = j_assets;
    QTimer::singleShot( 120000, this, &SettingsHostWorker::loadAssets );
}

void SettingsHostWorker::onWampSession(Wamp::Session *s) {


    session = s;
    session->provide("swiftbot.components.settings.status", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        Q_UNUSED(v)
        QString hostip;
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
                 hostip = address.toString();
        }
        QJsonObject j_ret;
        j_ret["status"] = "running";
        j_ret["name"] = "worker_settings";
        j_ret["version"] = APP_VERSION;
        j_ret["location"] = hostip;
        j_ret["memory"] = getMemUsage();
        const QString str(QJsonDocument(j_ret).toJson( QJsonDocument::Compact ) );
        return str;
    });

    session->subscribe("swiftbot.api.rpc.asyncresults",[&]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        if ( _async_waiters.isEmpty() ) {
            return ;
        }
        QJsonObject j_res( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );
        const quint64 async_uid( j_res.value("async_uuid").toString().toULongLong() );
        if ( _async_waiters.contains( async_uid ) ) {
            QSqlQuery q;
            if ( _parsers.value( async_uid ) == "bittrex_withdraw_fee" || _parsers.value( async_uid ) == "binance_withdraw_fee" ) {
             if ( j_res.contains("currencies") && !j_res.value("currencies").toArray().isEmpty() ) {
                 const QJsonArray j_curs( j_res.value("currencies").toArray() );
                 for( auto it = j_curs.begin(); it != j_curs.end(); it++ ) {
                     const quint32 cid = it->toObject().value("id").toString().toUInt();
                     const double fee = it->toObject().value("withdraw_fee").toString().toDouble();
                     if ( assets->getCurrencyWithdrawFee( cid ) != fee ) {
                         if ( !q.exec("UPDATE currencies SET withdraw_fee="+QString::number(fee,'f',4)+" WHERE id="+QString::number( cid ) ) ) {
                             qWarning() << q.lastError().text();
                         }
                     }
                 }
             }
            } else if ( _parsers.value( async_uid ) == "binance_trade_fee" ) {
                const QJsonArray j_pairs( j_res.value("fees").toArray() );
                for( auto it = j_pairs.begin(); it != j_pairs.end(); it++ ) {
                    const quint32 pid = it->toObject().value("id").toString().toUInt();
                    const double fee = it->toObject().value("fee").toString().toDouble();
                    if ( assets->getExchangeTradeFee(pid) != fee ) {
                        if ( !q.exec("UPDATE exchanges SET trade_fee="+QString::number(fee,'f',4)+" WHERE id="+QString::number( SwiftLib::getAssets()->getExchangeId( "binance" ) ) ) ) {
                            qWarning() << q.lastError().text();
                        }
                    }
                }
            }
            _async_waiters.removeAll( async_uid );
            _parsers.remove( async_uid );
        }

    });
    session->provide("swiftbot.components.settings.command", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        const QString cmd = v.at(0).toString();
        if ( cmd == "restart") {
            qApp->quit();
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        }
        return true;
    });

    session->provide("swiftbot.system.baseassets", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QString ass( QJsonDocument( j_assets_cached ).toJson( QJsonDocument::Compact ) );
        return ass;
    });


    session->provide("swiftbot.components.actions.insert", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        if ( !_user_actions.contains( v.at(0).toString() ) ) {
            _user_actions.insert( v.at(0).toString(), v.at(1).toString() );
        }
        return true;
    });
getMemUsage();
    session->provide("swiftbot.components.actions.get", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        Q_UNUSED(v)
        QJsonArray j_items;
        if ( !_user_actions.isEmpty() ) {
            for( auto it = _user_actions.begin(); it != _user_actions.end(); it++ ) {
                if ( it.key() == "api_credential_required_lbank" ) {
                    QJsonObject j_itm;
                    j_itm["id"] = it.key();
                    j_itm["exchange"] ="LBank";
                    j_itm["title"] = "Не установлен API ключ";
                    j_itm["ts"] = "(Время неизвестно)";
                    j_itm["text"] = "Не удается получить доступ к API биржи. Обновите сведения о ключах в настройках или отключите биржу от системы.";
                    j_itm["url"] ="";
                    j_itm["code"] = "400100";
                    j_items.push_back( j_itm );
                } else {
                    // qWarning() << it.value().toUtf8() << it.key().toUtf8();
                    QJsonObject j_itm( QJsonDocument::fromJson( it.value().toUtf8() ).object() );
                    j_itm["id"] = it.key();
                    j_items.push_back( j_itm );
                }
            }
        }
        const QString str( QJsonDocument( j_items ).toJson( QJsonDocument::Compact ) );
        return str;
    });

    session->provide("swiftbot.settings.add.address", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)

        const QString currency = v.at(0).toString();
        const QString token = v.at(1).toString();
        const QString address = v.at(2).toString();
        QSqlQuery q;
        q.prepare("INSERT INTO `"+getDbName()+"`.`currency_addresses` (currency_id,address,token_type) VALUES (:c, :a, :t) ON DUPLICATE KEY UPDATE address=VALUES(address)");
        q.bindValue(":c", currency.toUInt() );
        q.bindValue(":a",address );
        q.bindValue(":t", token );
        if ( !q.exec() ) {
            qWarning() << "ON ADDRESS ADD" << q.lastError().text();
        }
        q.finish();
        return true;
    });

    session->provide("get.config.service", [this](const QVariantList&v, const QVariantMap&m) {
          Q_UNUSED(m)
          Q_UNUSED(v)
        const QString cfg( getJsonConfig() );
        return cfg;
    });

    session->provide("set.config.service", [this](const QVariantList&v, const QVariantMap&m) {
        const QString str( v.at(0).toString() );
        setJsonConfig( str );
        Q_UNUSED(m)
        return 1;
    });



    session->provide("swiftbot.system.apistate.method.fail", [&](const QVariantList&v, const QVariantMap&m ){
        const QString& method = v.at(0).toString();
        emit markMethodFailed( method );
        return 0;
        Q_UNUSED(m)
    });

    session->provide("swiftbot.system.apistate.method.success", [&](const QVariantList&v, const QVariantMap&m ){
        const QString& method = v.at(0).toString();
        emit markMethodSuccess( method );
        return 0;
        Q_UNUSED(m)
    });

    session->provide("swiftbot.system.apistate.status", [&](const QVariantList&v, const QVariantMap&m ){
         const QString r( getMethodsStatus() );
         return r;
         Q_UNUSED(m)
         Q_UNUSED(v)
    });

    session->provide("firebase.append.token", [&](const QVariantList&v, const QVariantMap&m ){
        const QString token = v.at(0).toString();
        const QString realm = v.at(1).toString();
        qWarning() << token << realm;
        QSqlQuery q;
        if ( !q.exec("INSERT INTO `users_tokens` (`token`,`realm`) VALUES ('"+token.toUtf8()+"','"+realm.toUtf8()+"') ON DUPLICATE KEY UPDATE `realm`='"+realm.toUtf8()+"'") ) {
            qWarning() << "PARAM SAVE ERROR:" << q.lastError().text();
        } else {
            // session->publish("swiftbot.events.settings.valuechanged", QVariantList({param,value}));
        }
        q.finish();

        return QString("");
          Q_UNUSED(m)
    });

    session->provide("firebase.sendmessage", [&](const QVariantList&v, const QVariantMap&m ){
        QSqlQuery q;
        QJsonArray tokens;
        if ( q.exec("SELECT * FROM `users_tokens` WHERE `realm`='"+SwiftLib::getSettings()->value("WAMP_REALM").toString()+"'") ) {
            while( q.next() ) {
                tokens.push_back( q.value("token").toString() );
            }
        } else {
            qWarning() << "ERROR:" << q.lastError().text();
        }
        q.finish();
        if ( tokens.isEmpty() ) {
            return QString("No tokens on realm");
        }
        QNetworkRequest r(QUrl( "https://fcm.googleapis.com/fcm/send" ));
        r.setRawHeader("Content-Type","application/json");
        r.setRawHeader("Authorization", "key=AAAACpgLOv0:APA91bE4FfkJ88PTOCuN7KhEY9DlAj6ScB3oGB6TdZD4JX-7sTjVPb5ux2XkcPt_xX2wjIYh5MamYDlhKZp7Zy5VlLp6cbWRspquC8Jy8_tRVvWc6JferOZY1K1hb07wmqyp8T0od3VK");
        QJsonObject j_params;
        j_params["registration_ids"] = tokens;
        QJsonObject j_msg;
        j_msg["title"] = v.at(0).toString();
        j_msg["body"] = v.at(1).toString();
        j_params["notification"] = j_msg;
        netMan->post( r, QJsonDocument( j_params ).toJson( QJsonDocument::Compact ) );
        return QString("");
        Q_UNUSED(m)
    });


    session->provide("get.config.value", [&](const QVariantList&v, const QVariantMap&m ){
        if ( !v.isEmpty() ) {
            const QString paramsname = v.at(0).toString();
            const QString value = getParamUnlocked( paramsname );
            Q_UNUSED(m)
            return value;
        } else {
            return QString("");
        }
    });



    session->provide("get.config.exchanges", [&]( const QVariantList&v, const QVariantMap&m ){
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QStringList exchanges( getParam("EXCHANGES").split(",") );
        QJsonObject j_settings;
        QJsonObject exch_params;
        for ( auto it = exchanges.begin(); it!=exchanges.end(); it++ ) {
            const QString exch( *it );
            QJsonObject e_settings;
            const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
            const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
            const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
            e_settings["exchange"] = exch;
            e_settings["api_key"] = getParamUnlocked( keypath );
            e_settings["api_secret"] = getParamUnlocked( secretpath );
            e_settings["api_user"] = getParamUnlocked(userpath);
            exch_params[exch] = e_settings;
        }
        const QString settings( QJsonDocument( exch_params ).toJson( QJsonDocument::Compact ) );
        return settings;
    });


    session->provide("windows.get.hourtop", [&](const QVariantList&v, const QVariantMap&m){
       Q_UNUSED(v);
       Q_UNUSED(m);
       QSqlQuery q("SELECT window_id, count(*) as cc, avg(spread_percent) as sp FROM window_history WHERE spread_percent>0 AND td>=(now() - interval 1 hour) GROUP by window_id ORDER BY sp DESC, cc DESC LIMIT 10");
       QStringList ids;
       if ( q.exec() ) {
           while ( q.next() ) {
               if ( q.value("sp").toDouble() > 0 ) {
                ids.push_back( QString::number( q.value("window_id").toUInt() ) );
               }
           }
       }
       return ids.join(",");

    });
    session->provide("set.config.values", [=](const QVariantList&v, const QVariantMap&m ){
        Q_UNUSED(m)
        if ( !v.isEmpty() ) {
            const QString params = v.at(0).toString();
            const QJsonObject j_params( QJsonDocument::fromJson( params.toUtf8() ).object() );
            if ( !j_params.isEmpty() ) {
                for( auto it = j_params.begin(); it != j_params.end(); it++ ) {
                    const QString paramsname = it.key();
                    const QString value = it.value().toString();
                    emit newParam( paramsname, value );
                }
            }
            return true;
        } else {
            return true;
        }
    });
    session->provide("get.config.values", [=](const QVariantList&v, const QVariantMap&m ){
        Q_UNUSED(m)
        QJsonObject _ret;
        if ( !v.isEmpty() ) {
            const QStringList  params( v.at(0).toString().split(",") );
            if ( !params.isEmpty() ) {
                for( auto it = params.begin(); it != params.end(); it++ ) {
                    const QString param = *it;
                    _ret[ param ] = getParam( param );
                }
            }
        }
        const QString r( QJsonDocument( _ret ).toJson( QJsonDocument::Compact) );
        return r;
    });

    session->provide("get.config.fees", [=](const QVariantList&v, const QVariantMap&m ){
        Q_UNUSED(m)
        Q_UNUSED(v)
        QJsonObject _ret;
        _ret["trade"] = getExchangesFees();
        _ret["withdraw"] = getCurrenciesFees();
        const QString r( QJsonDocument( _ret ).toJson( QJsonDocument::Compact) );
        return r;
    });

    session->provide("set.config.fees", [=](const QVariantList&v, const QVariantMap&m ){
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

    session->provide("set.config.value", [&](const QVariantList&v, const QVariantMap&m ){
        Q_UNUSED(m)
        if ( !v.isEmpty() ) {
            const QString paramsname = v.at(0).toString();
            const QString value = v.at(1).toString();
            emit newParam( paramsname, value );
            return true;
        } else {
            return true;
        }
    });

    QTimer::singleShot(5000, this, &SettingsHostWorker::getWithdrawFeesFromApi );
    QTimer::singleShot(3000, this, &SettingsHostWorker::getTradeFeesFromApi );
}

void SettingsHostWorker::getWithdrawFeesFromApi() {
    if ( session != nullptr && session->isJoined() ) {
        const QStringList exchs_names( assets->getExchangesNames() );
        for( auto it = exchs_names.begin(); it != exchs_names.end(); it++ ) {
            const quint32 eid = assets->getExchangeId( *it );
            if ( assets->isExchangeHasApiFeeWithdraw( eid ) ) {
                if ( *it == "bittrex" ) {
                    const QString rpcpath( "swiftbot.api.rpc."+ QString::number( eid ).toUtf8() +".dict.currencies" );
                    quint64 asuid = session->call( rpcpath ).toULongLong();
                    if ( asuid > 0 ) {
                        _async_waiters.push_back( asuid );
                        _parsers[ asuid ] = "bittrex_withdraw_fee";
                    }
                } else if ( *it == "binance") {
                    const QString rpcpath( "swiftbot.api.rpc."+ QString::number( eid ).toUtf8() +".withdraw.fee" );
                    quint64 asuid = session->call( rpcpath ).toULongLong();
                    if ( asuid > 0 ) {
                        _async_waiters.push_back( asuid );
                        _parsers[ asuid ] = "binance_withdraw_fee";
                    }
                }
            }
        }
    }
}

void SettingsHostWorker::getTradeFeesFromApi() {
    if ( session != nullptr && session->isJoined() ) {
        const QStringList exchs_names( assets->getExchangesNames() );
        for( auto it = exchs_names.begin(); it != exchs_names.end(); it++ ) {
            const quint32 eid = assets->getExchangeId( *it );
            if ( assets->isExchangeHasApiFeeTrade( eid ) ) {
                if ( *it == "binance" ) {
                    const QString rpcpath( "swiftbot.api.rpc."+ QString::number( eid ).toUtf8() +".trade.fee" );
                    quint64 asuid = session->call( rpcpath ).toULongLong();
                    if ( asuid > 0 ) {
                        _async_waiters.push_back( asuid );
                        _parsers[ asuid ] = "binance_trade_fee";
                    }
                }
            }
        }
    }
}

QJsonObject SettingsHostWorker::getExchangesFees() {
    QJsonObject j_ret;
    const QStringList exchs_names( assets->getExchangesNames() );
    for( auto it = exchs_names.begin(); it != exchs_names.end(); it++ ) {
        const quint32 eid = assets->getExchangeId( *it );
        const double trade_fee = assets->getExchangeTradeFee( eid );
        j_ret[ QString::number( eid ) ] = QString::number( trade_fee, 'f', 4 );
    }
    return j_ret;
}

QJsonObject SettingsHostWorker::getCurrenciesFees() {
    QJsonObject j_ret;
    const QList<quint32> cids( assets->getAllCurrencies() );
    for( auto it = cids.begin(); it != cids.end(); it++ ) {
        const quint32 cid = *it;
        const double withdraw_fee = assets->getCurrencyWithdrawFee( cid );
        j_ret[ QString::number( cid ) ] = QString::number( withdraw_fee, 'f', 4 );
    }
    return j_ret;
}

QString SettingsHostWorker::getParam(const QString &param) {
    QMutexLocker lock( &mutex );
    if ( _cached_params.contains( param ) ) {
        return _cached_params.value( param );
    } else {
        QString val( SwiftLib::getSettings()->value( param, "0" ).toString() );
        return val;
    }
}

QString SettingsHostWorker::getParamUnlocked(const QString &param) {
    if ( !_cached_params.contains( param ) ) {
        _cached_params[param] = SwiftLib::getSettings()->value( param, "" ).toString( );
    }
    return _cached_params.value( param );
}



void SettingsHostWorker::saveParam(const QString &param, const QString &value) {
    if ( !_cached_params.contains( param ) || _cached_params.value( param ) != value ) {
        QSqlQuery q;
        if ( !q.exec("INSERT INTO `"+getDbName()+"`.`settings_storage` (`param`,`value`) VALUES ('"+param.toUtf8()+"','"+value.toUtf8()+"') ON DUPLICATE KEY UPDATE `value`='"+value.toUtf8()+"'") ) {
            qWarning() << "PARAM SAVE ERROR:" << q.lastError().text() << param << value;
        } else {
            _cached_params[param] = value;
            // session->publish("swiftbot.events.settings.valuechanged", QVariantList({param,value}));
        }
        q.finish();
    }
    SwiftLib::getSettings()->setValue(param, value );
}
