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
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QHash<QString, QString> _params;
    _params["ORDERBOOKS_CHECK_INTERVAL"] = "Интервал проверки доступных кандидатов";
    _params["ORDERBOOKS_VALID_TIME"] = "Срок жизни котировок";
    _params["ORDERBOOKS_SKIP_TOP"] = "Отступ от края стакана";
    _params["ORDERBOOKS_DEPTH"] = "Глубина стакана";
    _params["ORDERBOOKS_MIN_SPREAD"] = "Минимальный спред";
    _params["ARBITRAGE_MIN_PROFIT_RAW"] = "Минимальный профит";
    _params["ARBITRAGE_MIN_ORDER_SIZE"] = "Минимальный размер ордера";
    _params["ARBITRAGE_MAX_ORDER_SIZE"] = "Максимальный размер ордера";
    _params["BALANCER"] = "Балансировщик";
    _params["IS_RUNNING"] = "Активен";
    _params["UPDATE_BETA"] = "";
    _params["EXCHANGES_CPP"] = "";
    _params["EXCHANGES_CCX"] = "";
    _params["UPDATE_RELEASE"] = "";
    _params["BALANCES_GET_INTERVAL"] = "";
    _params["ORDERSMANAGER_ENABLED"] = "";
    _params["FILTERORDERBOOKS_ENABLED"] = "";
    _params["USERSTATUS_ENABLED"] = "";
    _params["API_DEBUG"] = "";
    _params["ORDERS_API_REFRESH_INTERVAL"] ="";
    _params["BITTREX_AUTOCONVERT_USDT_USD"] ="";
    _params["KRAKEN_AUTOCONVERT_USDT_USD"] ="";
    _params["BTC_MIN_WITHDRAW"] ="";
    _params["BTC_MAX_WITHDRAW"] ="";
    _params["USD_MIN_WITHDRAW"] ="";
    _params["USD_MAX_WITHDRAW"] ="";
    _params["IS_EMULATION"] = "";
    _params["EMULATE_BTC_START"] = "";
    _params["EMULATE_USD_START"] = "";
    _params["ORDER_CLOSE_AFTER"] = "";
    _params["ARBITRAGE_AGGREGATE_AMOUNTS"] = "";

    QSettings settings( CONFIG_FILENAME, QSettings::IniFormat );
    const QStringList exchanges( settings.value("EXCHANGES").toString().split(",") );
    QJsonObject j_settings;
    j_settings["exchanges_list"] = QJsonArray::fromStringList( exchanges );

    QJsonArray exch_params;
    for ( auto it = exchanges.begin(); it!=exchanges.end(); it++ ) {
        const QString exch( *it );
        QJsonObject e_settings;
        const QString keypath( QStringList({"api_nodes", exch, "api_key"}).join("/") );
        const QString secretpath( QStringList({"api_nodes", exch, "api_secret"}).join("/") );
        const QString userpath( QStringList({"api_nodes", exch, "username"}).join("/") );
        e_settings["exchange"] = exch;
        e_settings["key"] = settings.value(keypath).toString();
        e_settings["secret"] = settings.value(secretpath).toString();
        e_settings["username"] = settings.value(userpath).toString();
        exch_params.push_back( e_settings );
    }
    j_settings["exchanges_params"] = exch_params;
    QJsonArray main_params;
    for ( auto it = _params.begin(); it != _params.end(); it++ ) {
        const QString key = it.key();
        const QString description = it.value();
        const QString value = settings.value(key).toString();
        QJsonObject j_itm;
        j_itm["param"] = key;
        // j_itm["description"] = description;
        j_itm["value"] = value;
        main_params.push_back( j_itm );
    }
    j_settings["main_params"] = main_params;

    const QString retstr( QJsonDocument( j_settings ).toJson( QJsonDocument::Compact ) );
    printf("%s", retstr.toStdString().data() );
    return 0;
}
