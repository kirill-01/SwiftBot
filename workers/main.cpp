#include <QCoreApplication>
#include <QLockFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QCommandLineParser>
#include <QProcess>
#include <QSharedPointer>

#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlResult>
#include "swiftlib.h"
#include "ordersworker.h"
#include "orderworker.h"
#include "userhistoryworker.h"
// #include "balancesworker.h"
#include "balanceworker.h"
// #include "settingshostworker.h"
#include "settingsworker.h"
#include "logsworker.h"
#include "dbwriter.h"
#include "strategyworker.h"
#include <QThread>
#include <QHostInfo>
#include <QNetworkInterface>

void output( const QString& text ) {
    qInfo() << APP_NAME  << ":" << text;
}

void warn( const QString& text ) {
    qWarning() << APP_NAME << "[ERROR] :" << text;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName( APP_NAME );
    QCommandLineParser parser;
    parser.setApplicationDescription("SwiftArbitrage exchange API node");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption workerTargetOption(QStringList({"w","worker"}), "Worker name to start" );
    parser.addOption(workerTargetOption);

    parser.process(a);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
    db.setPort( 3306 );

    db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
    db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
    db.setDatabaseName( SwiftLib::getUserDbName() );
    if ( !db.open() ) {
        warn(db.lastError().text());
        return 1;
    }
    SwiftLib::loadActualConfig();

    if ( parser.isSet( workerTargetOption ) ) {
        const QStringList args = parser.positionalArguments();
        if ( !args.isEmpty() ) {
            QString worker_name = args.at(0);
            QString portparam("WAMP_PORT_WORKERS_"+worker_name.toUpper() );
            if ( worker_name == "orders" ) {
                DbWriter * writer = new DbWriter();

                OrderWorker * worker = new OrderWorker();
                QObject::connect( writer, &DbWriter::storedOrders, worker, &OrderWorker::onOrdersFromDb, Qt::QueuedConnection );
                QObject::connect( worker, &OrderWorker::saveOrder, writer, &DbWriter::onSaveOrder, Qt::QueuedConnection );
                QObject::connect( worker, &OrderWorker::saveOrderState, writer, &DbWriter::onSaveOrderState );

                worker->startClient();
                writer->onThreadStarted();
            } else {
                WampClient * wamp = new WampClient(
                            SwiftLib::getSettings()->value(SETTINGS_WAMP_REALM, "swiftbot").toString(),
                            SwiftLib::getSettings()->value(SETTINGS_WAMP_SERVER, "localhost").toString(),
                            SwiftLib::getSettings()->value(portparam, 8082).toInt() );
                if ( worker_name == "balances" ) {
                    BalanceWorker * worker = new BalanceWorker();
                    QObject::connect( wamp, &WampClient::clientConnected, worker, &SwiftWorker::onWampSession );
                } else if ( worker_name == "settingshost" || worker_name == "settings" ) {
                    SettingsWorker * worker= new SettingsWorker();
                    QObject::connect( wamp, &WampClient::clientConnected, worker, &SwiftWorker::onWampSession );
                } else if ( worker_name == "logs" ) {
                    LogsWorker * worker= new LogsWorker();
                    QObject::connect( wamp, &WampClient::clientConnected, worker, &SwiftWorker::onWampSession );
                }
                wamp->startClient();
                qWarning() << "Worker started: " << worker_name;
            }
        }
    } else {
        return 1;
    }
    return a.exec();
}
