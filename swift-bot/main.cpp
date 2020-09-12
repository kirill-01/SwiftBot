#include <QCoreApplication>
#include "swiftlib.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <orderbookskeeper.h>

#define APP_NAME "Swift-bot-2"
#define APP_VERSION "2.0.2044"
#include <QThread>
#include <QHash>
#include <QMap>
#include <QHostInfo>
#include <QProcess>
#include "arbitragepair.h"
#include "arbitragemanager.h"
#include <QProcess>
#include <QNetworkInterface>
#include "arbitrageworker.h"

void output( const QString& text ) {
    qInfo() << APP_NAME << APP_VERSION << ":" << text;
}

void warn( const QString& text ) {
    qWarning() << APP_NAME << APP_VERSION << "[ERROR] :" << text;
}

void updateConfig() {

    const QString dbname = SwiftLib::getSettings()->value("MYSQL_DBNAME", "swiftbot").toString().trimmed();
    QSqlQuery q("SELECT * FROM `"+dbname.toUtf8()+"`.`settings_storage`");
    if ( q.exec() ) {
        QSettings newsettings("/opt/swiftbot/config2.conf", QSettings::IniFormat );
        while ( q.next() ) {
            newsettings.setValue( q.value("param").toString(), q.value("value").toString() );
        }
        newsettings.sync();
        qWarning() << "Settings updated from db";
    }
}


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    a.setApplicationName( APP_NAME );
    a.setApplicationVersion( APP_VERSION );
    qRegisterMetaType<QHash<quint32,QMap<double,double>>>("QHash<quint32,QMap<double,double>>");


    if ( !SwiftLib::hasSettings( SETTINGS_WAMP_SERVER ) ) {
        warn( "Please set correct WAMP server settings" );
        return 1;
    }

    if ( !SwiftLib::hasSettings( SETTINGS_MYSQL_HOST ) || !SwiftLib::hasSettings( SETTINGS_MYSQL_USERNAME ) || !SwiftLib::hasSettings( SETTINGS_MYSQL_USERPASSWORD ) || !SwiftLib::hasSettings( SETTINGS_MYSQL_DBNAME ) ) {
        warn( "Please set correct MYSQL server settings" );
        return 1;
    }
    const QString dbname( SwiftLib::getUserDbName() );
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
    db.setPort( 3306 );

    db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
    db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
    db.setDatabaseName( SwiftLib::getUserDbName() );
    if ( !db.open() ) {
        warn(db.lastError().text());
        return 1;
    } else {
        output("Success connection to MySQL db: "+dbname );
        QList<QSqlQuery> _tables;
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`exchanges` (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT,`name` VARCHAR(45) NOT NULL,`is_enabled` SMALLINT NOT NULL DEFAULT 0, PRIMARY KEY (`id`),UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE,UNIQUE INDEX `name_UNIQUE` (`name` ASC) VISIBLE); ") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`pairs` (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT,`base_currency_id` INT UNSIGNED NOT NULL DEFAULT 0,`market_currency_id` INT NOT NULL DEFAULT 0, `exchange_id` INT UNSIGNED NOT NULL DEFAULT 0,`is_enabled` SMALLINT UNSIGNED NOT NULL DEFAULT 0,`arbitrage_pair_id` INT UNSIGNED NOT NULL DEFAULT 0,PRIMARY KEY (`id`),UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE); ") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`currencies` (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT,`exchange_id` INT UNSIGNED NOT NULL DEFAULT 0,`coin_id` INT UNSIGNED NOT NULL DEFAULT 0,`name` VARCHAR(45) NOT NULL DEFAULT 'UNKNWN',`is_enabled` SMALLINT UNSIGNED NOT NULL DEFAULT 0,PRIMARY KEY (`id`),UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE);") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`arbitrage_pairs` (`id` INT UNSIGNED NOT NULL AUTO_INCREMENT,`base_coin_id` INT UNSIGNED NOT NULL DEFAULT 0,`price_coin_id` INT NOT NULL DEFAULT 0, `name` VARCHAR(45) NOT NULL DEFAULT 'UNKNWN', `is_enabled` SMALLINT UNSIGNED NOT NULL DEFAULT 0,PRIMARY KEY (`id`),UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE);") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`rates` (`pair_id` INT UNSIGNED NOT NULL,`rate` DOUBLE(16,8) UNSIGNED NULL,`ts` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,`id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,PRIMARY KEY (`id`),UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE);") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`currency_addresses` (`currency_id` int(11) NOT NULL, `address` varchar(255) NOT NULL,`token_type` varchar(45) NOT NULL DEFAULT 'BTC',PRIMARY KEY (`currency_id`,`token_type`),UNIQUE KEY `address_UNIQUE` (`address`)) ENGINE=InnoDB;") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`orders` (  `local_id` VARCHAR(255) NOT NULL,  `remote_id` VARCHAR(255) NULL DEFAULT 0,  `market_id` INT NULL DEFAULT 0,  `amount` DOUBLE(24,8) NOT NULL DEFAULT 0,  `amount_left` DOUBLE(24,8) NOT NULL DEFAULT 0,  `type` SMALLINT NOT NULL DEFAULT 0,  `rate` DOUBLE(24,8) NOT NULL DEFAULT 0,  `price` DOUBLE(24,8) NOT NULL DEFAULT 0,  `fee` DOUBLE(24,8) NOT NULL DEFAULT 0,  `status` SMALLINT NOT NULL DEFAULT 0,  `arb_window` INT NOT NULL DEFAULT 0,  `ts` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,  PRIMARY KEY (`local_id`),  UNIQUE INDEX `local_id_UNIQUE` (`local_id` ASC) VISIBLE);") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`deposits` (  `local_id` VARCHAR(255) NOT NULL DEFAULT '#',  `remote_id` VARCHAR(255) NOT NULL DEFAULT '#',  `currency_id` INT NOT NULL DEFAULT 0,  `amount` DOUBLE(24,8) NOT NULL DEFAULT 0,  `address` VARCHAR(255) NOT NULL DEFAULT '#',  `created_at` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,  `txhash` VARCHAR(255) NOT NULL DEFAULT '#',  `status` VARCHAR(45) NOT NULL DEFAULT 'error',  `fee` DOUBLE(24,8) NOT NULL DEFAULT 0,  PRIMARY KEY (`local_id`),  UNIQUE INDEX `local_id_UNIQUE` (`local_id` ASC) VISIBLE); ALTER TABLE `deposits` ADD COLUMN `id` INT NOT NULL AUTO_INCREMENT AFTER `fee`,DROP PRIMARY KEY,ADD PRIMARY KEY (`id`),ADD UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE;") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`withdraws` (  `local_id` VARCHAR(255) NOT NULL DEFAULT '#',  `remote_id` VARCHAR(255) NOT NULL DEFAULT '#',  `currency_id` INT NOT NULL DEFAULT 0,  `amount` DOUBLE(24,8) NOT NULL DEFAULT 0,  `address` VARCHAR(255) NOT NULL DEFAULT '#',  `created_at` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,  `txhash` VARCHAR(255) NOT NULL DEFAULT '#',  `status` VARCHAR(45) NOT NULL DEFAULT 'error',  `fee` DOUBLE(24,8) NOT NULL DEFAULT 0,  PRIMARY KEY (`local_id`),  UNIQUE INDEX `local_id_UNIQUE` (`local_id` ASC) VISIBLE); ALTER TABLE `withdraws` ADD COLUMN `id` INT NOT NULL AUTO_INCREMENT AFTER `fee`,DROP PRIMARY KEY,ADD PRIMARY KEY (`id`),ADD UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE;") );
        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`currency_requested` (  `currency_id` INT NOT NULL,  `requested` DOUBLE(24,8) NOT NULL,  `ts` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,  PRIMARY KEY (`currency_id`)); ALTER TABLE `swiftbot`.`currency_requested` ADD UNIQUE INDEX `index1` (`requested` ASC, `ts` ASC, `currency_id` ASC),DROP PRIMARY KEY;") );


        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`windows` ( `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,  `buy_market` INT UNSIGNED NOT NULL,  `sell_market` INT UNSIGNED NOT NULL,  `buy_exchange` INT UNSIGNED NOT NULL,  `sell_exchange` INT UNSIGNED NOT NULL,`is_enabled` TINYINT UNSIGNED NOT NULL DEFAULT 1,`arbitrage_pair_id` INT UNSIGNED NOT NULL,PRIMARY KEY (`id`),UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE);") );

        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`users_tokens` (`token` VARCHAR(255) NOT NULL, `realm` VARCHAR(255) NOT NULL,PRIMARY KEY (`token`),UNIQUE INDEX `token_UNIQUE` (`token` ASC) VISIBLE);") );

        _tables.push_back( QSqlQuery("CREATE TABLE IF NOT EXISTS `"+dbname+"`.`settings_storage` (`param` VARCHAR(255) NOT NULL,`value` VARCHAR(255) NOT NULL,PRIMARY KEY (`param`),UNIQUE INDEX `param_UNIQUE` (`param` ASC) VISIBLE);") );
        for( auto it = _tables.begin(); it != _tables.end(); it++ ) {
            if ( !it->exec() ) {
                qWarning() << it->lastError().text();
            }
        }


    }

    ArbitrageWorker * arbitrage_worker = new ArbitrageWorker();

    OrderbooksKeeper * orderbooks_keeper = new OrderbooksKeeper();

    QThread * orderbooks_keeper_thread = new QThread();
    orderbooks_keeper->moveToThread( orderbooks_keeper_thread );
    QObject::connect( orderbooks_keeper_thread, &QThread::started, orderbooks_keeper, &OrderbooksKeeper::startKeeper );


    qWarning() << "WAMP REALM: "+SwiftLib::getSettings()->value(SETTINGS_WAMP_REALM).toString();
    WampClient * client = new WampClient(
                SwiftLib::getSettings()->value(SETTINGS_WAMP_REALM).toString(),
                SwiftLib::getSettings()->value(SETTINGS_WAMP_SERVER, "localhost").toString(),
                SwiftLib::getSettings()->value("WAMP_PORT_BOT", 8083).toInt() );



    QObject::connect( client, &WampClient::clientConnected, arbitrage_worker, &ArbitrageWorker::onWampSession );

    static QHash<quint32, QMap<double,double>> __cached_asks;
    static QHash<quint32, QMap<double,double>> __cached_bids;

    QObject::connect( orderbooks_keeper, &OrderbooksKeeper::orderbooks, arbitrage_worker->manager, &ArbitrageManager::calculateOrderbooks );


    QObject::connect( orderbooks_keeper, &OrderbooksKeeper::orderbooks, [](const QHash<quint32, QMap<double, double> > &asks, const QHash<quint32, QMap<double, double> > &bids){
        for ( auto it = asks.begin(); it != asks.end(); it++ ) {
            __cached_asks[ it.key() ] = it.value();
        }
        for ( auto it = bids.begin(); it != bids.end(); it++ ) {
            __cached_bids[ it.key() ] = it.value();
        }
    });

    orderbooks_keeper_thread->start();
    QObject::connect( client, &WampClient::clientConnected, []( Wamp::Session * session ) {
        session->provide("swiftbot.orderbooks.cache", [](const QVariantList&v, const QVariantMap&m) {
            Q_UNUSED(m)
            QJsonObject j;
            QJsonArray as;
            QJsonArray bi;
            if ( !v.isEmpty() ) {
                quint32 req_pair = v.at(0).toUInt();
                for(auto it = __cached_asks.begin(); it != __cached_asks.end(); it++ ) {
                    const quint32 pairid( it.key() );
                    QMap<double,double> _itms( it.value() );
                    for( auto iit = _itms.begin(); iit != _itms.end(); iit++ ) {
                        if ( req_pair == pairid ) {
                            QJsonArray itm;
                            itm << QString::number( pairid ) << QString::number( iit.key(), 'f', 8 ) << QString::number( iit.value(), 'f', 8 );
                            as.push_back( itm );
                        }
                    }
                }
                for(auto it = __cached_bids.begin(); it != __cached_bids.end(); it++ ) {
                    const quint32 pairid( it.key() );
                    QMap<double,double> _itms( it.value() );
                    for( auto iit = _itms.begin(); iit != _itms.end(); iit++ ) {
                        if ( req_pair == pairid ) {
                            QJsonArray itm;
                            itm << QString::number( pairid ) << QString::number( iit.key(), 'f', 8 ) << QString::number( iit.value(), 'f', 8 );
                            bi.push_back( itm );
                        }
                    }
                }
            }
            j["a"] = as;
            j["b"] = bi;
            QString re( QJsonDocument (j).toJson( QJsonDocument::Compact ) );
            return re;
        });

        session->subscribe("swiftbot.events.settings.valuechanged", [&](const QVariantList&v, const QVariantMap&m) {
            Q_UNUSED(m)
            if ( v.size() == 2 ) {
                SwiftLib::getSettings()->setValue( v.at(0).toString(), v.at(1).toString() );
            }
        });

        session->provide("swiftbot.system.update", [=](const QVariantList&v, const QVariantMap&m){
            QProcess::startDetached("/opt/swiftbot/bin/update.sh");
            QJsonObject msg;
            msg["text"] = "Запущено обновление системы";
            msg["cssClass"] = "ordersuccess";
            msg["closeTimeout"] = "10000";
            msg["position"] = "top";
            if ( session != nullptr && session->isJoined() ) {
                session->publishMessage("swiftbot.system.messages", msg);
            }
            Q_UNUSED(m)
            Q_UNUSED(v)
            return 1;
        });

        session->provide("swiftbot.components.swiftbot.status", [&](const QVariantList&v, const QVariantMap&m){
            QJsonObject j_ret;
            j_ret["status"] = "running";
            QString hostip;
            const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
            for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
                if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost && address != QHostAddress("10.9.0.1") )
                     hostip = address.toString();
            }
            QJsonObject options;
            options["ETHUSDPAIR"] =  SwiftLib::getSettings()->value("ETHUSDPAIR", "0" ).toString();
            options["BTCUSDPAIR"] =  SwiftLib::getSettings()->value("BTCUSDPAIR", "0" ).toString();
            j_ret["options"] = options;
            j_ret["name"] = "swift_bot";
            j_ret["version"] = QString( APP_VERSION );
            j_ret["location"] = hostip;
            const QString str(QJsonDocument(j_ret).toJson( QJsonDocument::Compact ) );
            return str;
            Q_UNUSED(v)
            Q_UNUSED(m)
        });

        session->provide("swiftbot.components.swiftbot.command", [&](const QVariantList&v, const QVariantMap&m){
            if ( v.size() > 0 ) {
                const QString cmd = v.at(0).toString();
                if ( cmd == "restart") {
                    QProcess::startDetached("systemctl", {"restart", "swift-orderbooks-proxy"});
                    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
                    qApp->quit();
                    return true;
                } else if ( cmd == "reloadconfig" ) {
                    updateConfig();
                    return true;
                }
            }
            Q_UNUSED(m)
            return true;
        });

        QJsonObject msg;
        msg["text"] = "MODULE_SWIFTBOT_STARTED";
        msg["cssClass"] = "ordersuccess";
        msg["closeTimeout"] = "1000";
        msg["position"] = "top";
        session->publishMessage("swiftbot.system.messages", msg);
    } );


    client->startClient();
    return a.exec();
}
