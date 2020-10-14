#include <QCoreApplication>
#include "../swift-lib/swiftlib.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QNetworkInterface>

#define APP_NAME "Swift exchange API node"
#define APP_VERSION "2.0.2059"

#include <QLockFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QCommandLineParser>
#include <QProcess>
#include <QSharedPointer>
// #include "apiemulator.h"


#include <QFile>

#include <QThread>
#include "procwrapper.h"

#include "swiftapiclient.h"

#include "swiftapiclientbittrex.h"
#include "swiftapiparserbittrex.h"

#include "swiftapiclientcexio.h"
#include "swiftapiparsercexio.h"

#include "swiftapiclientkucoin.h"
#include "swiftapiparserkucoin.h"

#include "swiftapiclientlbank.h"
#include "swiftapiparserlbank.h"

#include "swiftapiclientlivecoin.h"
#include "swiftapiparserlivecoin.h"

#include "swiftapiclientexmo.h"
#include "swiftapiparserexmo.h"

#include "swiftapiclientlakebtc.h"
#include "swiftapiparserlakebtc.h"

#include "swiftapiclienthitbtc.h"
#include "swiftapiparserhitbtc.h"

#include "swiftapiclientbinance.h"
#include "swiftapiparserbinance.h"

#include "swiftapiclientbitfinex.h"
#include "swiftapiparserbitfinex.h"

#include "swiftapiclientkraken.h"
#include "swiftapiparserkraken.h"


#include "swiftapiclientzb.h"
#include "swiftapiparserzb.h"



#include "swiftapiclientokcoin.h"
#include "swiftapiparserokcoin.h"

#include "swiftapiclienthuobi.h"
#include "swiftapiparserhuobi.h"

#include "swiftapiclientidcm.h"
#include "swiftapiparseridcm.h"

#include <QHostInfo>


#define LOCKFILE_EXCHANGE_NAME "swift-arbitrage-exchanges"

void output( const QString& text ) {
    qInfo() << APP_NAME << APP_VERSION << ":" << text;
}

void warn( const QString& text ) {
    qWarning() << APP_NAME << APP_VERSION << "[ERROR] :" << text;
}


void updateConfig(const QString& conf ) {

    QSettings * settings = SwiftLib::getSettings();
    QJsonObject received_json( QJsonDocument::fromJson( conf.toUtf8()).object() );
    QJsonArray j_exch_params( received_json.value("exchanges_params").toArray() );
    QJsonArray j_main_params( received_json.value("main_params").toArray() );

    for( auto it = j_exch_params.begin(); it != j_exch_params.end(); it++ ) {
        QJsonObject j_val( it->toObject() );
        QString exch( j_val.value("exchange").toString( ) );
        QString key( j_val.value("key").toString( ) );
        QString secret( j_val.value("secret").toString( ) );
        QString username( j_val.value("username").toString( ) );
        const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
        const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
        const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
        settings->setValue( keypath, key );
        settings->setValue( secretpath, secret );
        settings->setValue( userpath, username );
    }
    for( auto it = j_main_params.begin(); it != j_main_params.end(); it++ ) {
        QJsonObject j_val( it->toObject() );
        settings->setValue( j_val.value("param").toString(), j_val.value("value").toString() );
    }
    settings->sync();
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName( APP_NAME );
    QCoreApplication::setApplicationVersion( APP_VERSION );

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
        output("Success connection to MySQL db");
        SwiftLib::loadActualConfig();
    }

    QCommandLineParser parser;
    parser.setApplicationDescription("SwiftArbitrage exchange API node");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption targetExchangeApiOption(QStringList({"e","exchange"}), "Exchange to start API client" );
    parser.addOption(targetExchangeApiOption);
    parser.process(a);

    QVector<QSharedPointer<ProcWrapper>> _procs;
    QStringList exchanges( SwiftLib::getAssets()->getExchangesNames() );

    WampClient * wclient = new WampClient(
                SwiftLib::getSettings()->value(SETTINGS_WAMP_REALM, "kaa").toString(),
                SwiftLib::getSettings()->value(SETTINGS_WAMP_SERVER, "localhost").toString(),
                SwiftLib::getSettings()->value("WAMP_PORT_APINODES", 8082).toInt() );

    if ( !parser.isSet( targetExchangeApiOption ) ) {
        QObject::connect( wclient, &WampClient::clientConnected, [&]( Wamp::Session * session ) {

            session->publish("swift.system.feed.messages", {"Swift APIManager","Started successfull"});

            session->provide("swift.system.rpc.module.status.apinodes", [&](const QVariantList&v, const QVariantMap&m){
                Q_UNUSED(m)
                Q_UNUSED(v)
                QString hostip;
                const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
                for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
                    if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
                         hostip = address.toString();
                } 
                const QJsonArray j_ret({QString::number( 1 ), hostip, APP_VERSION, "{}" });
                const QString _r( QJsonDocument(j_ret ).toJson( QJsonDocument::Compact ) );
                return _r;
            });

            session->provide("swift.system.rpc.module.cmd.apinodes", [&](const QVariantList&v, const QVariantMap&m){
                //[CMD]
                Q_UNUSED(m)
                const QString cmd( v.at(0).toString() );
                if ( cmd == "pause" ) {

                } else if ( cmd == "start" ) {

                } else if ( cmd == "restart" ) {
                    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
                    a.quit();
                }
                return true;
            });
        } );
    }

        if ( parser.isSet( targetExchangeApiOption ) ) {
            const QStringList args = parser.positionalArguments();
            if ( !args.isEmpty() ) {
                QString exchange_name = args.at(0);
                if ( exchange_name == "bittrex" ) {
                    SwiftApiClientBittrex * client = new SwiftApiClientBittrex(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserBittrex * parser = new SwiftApiParserBittrex();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse);
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed);
                } else if ( exchange_name == "cexio" ) {
                    SwiftApiClientCexio * client = new SwiftApiClientCexio(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession );
                    SwiftApiParserCexio * parser = new SwiftApiParserCexio();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "kucoin" ) {
                    SwiftApiClientKuCoin * client = new SwiftApiClientKuCoin(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserKuCoin * parser = new SwiftApiParserKuCoin();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "livecoin" ) {
                    SwiftApiClientLivecoin * client = new SwiftApiClientLivecoin(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserLivecoin * parser = new SwiftApiParserLivecoin();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                }  else if ( exchange_name == "lbank" ) {
                    SwiftApiClientLbank * client = new SwiftApiClientLbank(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserLbank * parser = new SwiftApiParserLbank();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "exmo" ) {
                    SwiftApiClientExmo * client = new SwiftApiClientExmo(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserExmo * parser = new SwiftApiParserExmo();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "lakebtc" ) {
                    SwiftApiClientLakeBTC* client = new SwiftApiClientLakeBTC(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserLakebtc * parser = new SwiftApiParserLakebtc();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                }  else if ( exchange_name == "hitbtc" ) {
                    SwiftApiClientHitbtc * client = new SwiftApiClientHitbtc(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserHitbtc * parser = new SwiftApiParserHitbtc();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "binance" ) {
                    SwiftApiClientBinance * client = new SwiftApiClientBinance(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserBinance * parser = new SwiftApiParserBinance();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "bitfinex" ) {
                    SwiftApiClientBitfinex * client = new SwiftApiClientBitfinex(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserBitfinex * parser = new SwiftApiParserBitfinex();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "kraken" ) {
                    SwiftApiClientKraken * client = new SwiftApiClientKraken(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserKraken* parser = new SwiftApiParserKraken();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "okcoin" ) {
                    SwiftApiClientOkcoin * client = new SwiftApiClientOkcoin(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserOkcoin* parser = new SwiftApiParserOkcoin();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "zb" ) {
                    SwiftApiClientZB * client = new SwiftApiClientZB(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserZB* parser = new SwiftApiParserZB();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "huobi" ) {
                    SwiftApiClientHuobi * client = new SwiftApiClientHuobi(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserHuobi* parser = new SwiftApiParserHuobi();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else if ( exchange_name == "idcm" ) {
                    SwiftApiClientIDCM * client = new SwiftApiClientIDCM(nullptr);
                    QObject::connect( wclient, &WampClient::clientConnected, client, &SwiftApiClient::onWampSession);
                    SwiftApiParserIDCM* parser = new SwiftApiParserIDCM();
                    QObject::connect( client, &SwiftApiClient::parseApiResponse, parser, &SwiftApiParser::parseResponse );
                    QObject::connect( parser,  &SwiftApiParser::resultParsed, client, &SwiftApiClient::onApiResponseParsed );
                } else {
                    qWarning() << "[ERROR] Exchange not exists or not supported." << exchange_name;
                }
                QObject::connect( wclient, &WampClient::clientdiconnected,[&a](){
                    qWarning() << "Exiting";
                    a.quit();
                } );
            }
        } else {
            QStringList exchsenabled;
            QStringList cppapis;
            QStringList jsapis;
            const QStringList enexchanges( SwiftLib::getSettings()->value("EXCHANGES").toString().split(",") );
            QSqlQuery q("SELECT * FROM `exchanges` WHERE is_enabled=1");
            if ( q.exec( ) ) {
                while ( q.next() ) {
                    exchsenabled.push_back(q.value("name").toString() );
                }
            }
            for ( auto it = exchsenabled.begin(); it != exchsenabled.end(); it++ ) {
                const QString exchname( *it );
                if ( enexchanges.contains( exchname ) ) {
                    _procs.push_back( QSharedPointer<ProcWrapper>( new ProcWrapper("/opt/swiftbot/bin/swift-arbitrage-api-node", {"-e", exchname })) );
                }
            }
            for ( auto it = _procs.begin(); it != _procs.end(); it++ ) {
                QObject::connect( &a, &QCoreApplication::aboutToQuit, it->data(), &ProcWrapper::stop );
            }
        }
    wclient->startClient();
    return a.exec();
}
