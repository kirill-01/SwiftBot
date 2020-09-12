#include <QCoreApplication>
#include <QCommandLineParser>
#include "../swift-lib/swiftlib.h"
#include "../swift-lib/wampclient.h"
#include <stdlib.h>
#include "../swift-lib/apircpclient.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("SwiftBot command line interface");
    parser.addHelpOption();
    parser.addVersionOption();


    parser.process(a);

    if ( parser.positionalArguments().size() == 4 ) {
        const QString hostname( parser.positionalArguments().at(0) );
        const quint32 methodName( parser.positionalArguments().at(2).toUInt() );
        const quint32 targetExchange( parser.positionalArguments().at(1).toUInt() );
        const QString argsRaw( parser.positionalArguments().at(3) );

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
        db.setPort( static_cast<quint16>( SwiftLib::getSettings()->value(SETTINGS_MYSQL_PORT ).toUInt() ) );
        db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
        db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
        if ( SwiftLib::getSettings()->value("IS_EMULATION", 0 ).toUInt() == 1 ) {
            db.setDatabaseName( "test_db" );
        } else {
            db.setDatabaseName( SwiftLib::getSettings()->value("MYSQL_DBNAME").toString() );
        }
        if ( !db.open() ) {
            qWarning() << db.lastError().text();
            return 1;
        }

        WampClient * wampClient = new WampClient(hostname, "cc.finance", 8081 );
        ApiRcpClient * apiclient = new ApiRcpClient();
        QObject::connect( wampClient, &WampClient::clientConnected, apiclient,  &AsyncRcpCallerInterface::onWampSession, Qt::QueuedConnection );

        QObject::connect( apiclient, &AsyncRcpCallerInterface::readyToStart, [targetExchange,methodName, hostname, apiclient,&a ]() {
            const SwiftLib::WampExchangesRpcMethods met = static_cast<SwiftLib::WampExchangesRpcMethods>(methodName);
            QJsonObject j_res ( apiclient->waitResult(  SwiftLib::getRpcPath( targetExchange, met ) ) );
            printf( "%s", QJsonDocument(  j_res ).toJson( QJsonDocument::Indented ).constData() );
            printf("\n");
            a.exit(0);
        });
        wampClient->startClient();
    } else if ( parser.positionalArguments().size() == 3 ) {
        const QString realm( parser.positionalArguments().at(0) );
        const QString rpcpath( parser.positionalArguments().at(1) );
        const QString argsRaw( parser.positionalArguments().at(2) );

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
        db.setPort( static_cast<quint16>( SwiftLib::getSettings()->value(SETTINGS_MYSQL_PORT ).toUInt() ) );
        db.setUserName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERNAME ).toString() );
        db.setPassword( SwiftLib::getSettings()->value(SETTINGS_MYSQL_USERPASSWORD ).toString() );
        if ( SwiftLib::getSettings()->value("IS_EMULATION", 0 ).toUInt() == 1 ) {
            db.setDatabaseName( "test_db" );
        } else {
            db.setDatabaseName( SwiftLib::getSettings()->value("MYSQL_DBNAME").toString() );
        }
        if ( !db.open() ) {
            qWarning() << db.lastError().text();
            return 1;
        }

        WampClient * wampClient = new WampClient(realm, "cc.finance", 8081 );
        ApiRcpClient * apiclient = new ApiRcpClient();
        QObject::connect( wampClient, &WampClient::clientConnected, apiclient,  &AsyncRcpCallerInterface::onWampSession );
        const QJsonObject j_params( QJsonDocument::fromJson( argsRaw.toUtf8() ).object() );
        QObject::connect( apiclient, &AsyncRcpCallerInterface::readyToStart, [rpcpath, apiclient,j_params,&a ]() {
            QJsonObject j_res ( apiclient->waitResult( rpcpath, j_params ) );
            printf( "%s", QJsonDocument(  j_res ).toJson( QJsonDocument::Indented ).constData() );
            printf("\n");
            a.exit(0);
        });
        wampClient->startClient();
    } else  {
       WampClient * wampClient = new WampClient("test1", "localhost", 8081, true );
       wampClient->startClient();
       WampClient * wampClient2 = new WampClient("test1", "localhost", 8082, true );
       wampClient2->startClient();
    }


    return a.exec();
}
