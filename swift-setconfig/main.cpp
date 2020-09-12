#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QDir>

#define CONFIG_FILENAME "/opt/swiftbot/config.conf"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QTextStream>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSettings settings( CONFIG_FILENAME, QSettings::IniFormat );
    QCommandLineParser parser;
    parser.setApplicationDescription("SwiftBot config tool");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);

    if ( parser.positionalArguments().count() == 1 ) {
        const QString argument( parser.positionalArguments().at(0) );
        QJsonObject received_json( QJsonDocument::fromJson( argument.toUtf8()).object() );
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
            settings.setValue( keypath, key );
            settings.setValue( secretpath, secret );
            settings.setValue( userpath, username );
        }
        for( auto it = j_main_params.begin(); it != j_main_params.end(); it++ ) {
            QJsonObject j_val( it->toObject() );
            settings.setValue( j_val.value("param").toString(), j_val.value("value").toString() );
        }
        settings.sync();
        return 0;
    } else {
        printf("%s", "Required json string as argument");
        return 1;
    }
}
