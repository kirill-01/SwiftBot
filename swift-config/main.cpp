#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QDir>

#define CONFIG_FILENAME "/opt/swiftbot/config.conf"
#define API_JS_BASEDIR "/opt/swiftbot/js/"
#include <QDebug>
#include <QFile>

#include <QTextStream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QSettings settings( CONFIG_FILENAME, QSettings::IniFormat );
    const QString wampserv( settings.value("WAMP_SERVER").toString() );
    const QString wamprealm( settings.value("WAMP_REALM").toString() );
    const QStringList exchanges( settings.value("EXCHANGES").toString().split(",") );
    for ( auto it = exchanges.begin(); it!=exchanges.end(); it++ ) {
        const QString exch( *it );
        if ( QDir(API_JS_BASEDIR+exch).exists() ) {
            const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
            const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
            const QString additionalpath( QStringList({"api_nodes", exch, "username"}).join("/") );
            if ( !settings.contains( keypath ) ) {
                settings.setValue( keypath, "" );
                settings.setValue( secretpath, "" );
                settings.setValue( additionalpath, "" );
                settings.sync();
            }
            const QString fname( API_JS_BASEDIR+exch+"/.env" );
            QFile envFile( fname );
            envFile.open( QIODevice::WriteOnly | QIODevice::Text  );
            envFile.resize(0);
            QTextStream stream( &envFile );
            stream << "wserver=wss://ws.cc.finance/ws\n";
            stream << "realm="+wamprealm+"\n";
            stream << "apikey="+settings.value(keypath,"").toString()+"\n";
            stream << "apisecret="+settings.value(secretpath,"").toString()+"\n";
            stream << "apiadditional="+settings.value(additionalpath,"").toString()+"\n";
            envFile.close();
        }
    }
    return 0;
}
