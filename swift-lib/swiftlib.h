#ifndef SWIFTLIB_H
#define SWIFTLIB_H

#ifndef SWIFT_BASEDIR
#define SWIFT_BASEDIR "/opt/swiftbot"
#endif

#ifndef SWIFT_SETTINGS_FILENAME
#define SWIFT_SETTINGS_FILENAME "config.conf"
#endif

#define SETTINGS_WAMP_SERVER "WAMP_SERVER"
#define SETTINGS_WAMP_PORT "WAMP_PORT"
#define SETTINGS_WAMP_REALM "WAMP_REALM"
#define SETTINGS_WAMP_RECONNECT_LIMIT "WAMP_RECONNECT_LIMIT"
#define SETTINGS_WAMP_RECONNECT_TIMEOUT_SECS "WAMP_RECONNECT_TIMEOUT_SECS"

#define SETTINGS_MYSQL_HOST "MYSQL_HOST"
#define SETTINGS_MYSQL_PORT "MYSQL_PORT"
#define SETTINGS_MYSQL_DBNAME "MYSQL_DBNAME"
#define SETTINGS_MYSQL_USERNAME "MYSQL_USERNAME"
#define SETTINGS_MYSQL_USERPASSWORD "MYSQL_USERPASSWORD"

#include <QSettings>
#include <QSharedPointer>
#include <QFile>
#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariantList>
#include <QVariantMap>
#include <QTcpSocket>
#include <QDebug>
#include <sys/sysinfo.h>
#include <QTimer>
#include <QCoreApplication>
#include <QDateTime>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "wampclient.h"
#include "assetsstorage.h"
#include <QCryptographicHash>



class ApiRcpClient;

class SwiftLib : public QObject
{
    Q_OBJECT
public:
    explicit SwiftLib(QObject * parent = nullptr );
    static AssetsStorage* getAssets();
    // Settings getter
    static QSettings* getSettings();

    Q_ENUMS(WampTopics)

    enum WampSystemEvents {
        WorkerStarted,
        WorkerPaused,
        WorkerRestarting
    };
    enum WampTopics {
        ErrorsTopic,
        ApiRpcAsyncResults,
        LogsTopic
    };

    Q_ENUMS(WampExchangesRpcMethods)

    enum WampExchangesRpcMethods {
        CustomMethod,
        GetCurrencies,
        GetMarkets,
        GetOrderbooks,
        OrderPlace,
        OrderCancel,
        OrderGet,
        OrderReplace,
        WithdrawGetLimits,
        WithdrawList,
        WithdrawHistory,
        WithdrawCancel,
        WithdrawCreate,
        WithdrawInner,
        WithdrawGetFee,
        GetBalances,
        GetDeposits,
        GetDepositAddress,
        TradeHistory,
        TradeOpenOrders,
        TradeGetFee,
        TradeGetLimits
    };


    enum SystemModes {
        CollectStatsMode,
        PrepareBalancesMode,
        TradeAllWindowsMode,
        TradeExactWindowsMode,
        SystemSetupMode
    };

    Q_ENUMS(SystemModes)

    static double getExchangeFee(const quint32& id );

    static QString getRpcPath( const quint32& exchange_id, const WampExchangesRpcMethods& method );

    static QString getTopic( const WampTopics& topic );
    static QString getMethodName( const WampExchangesRpcMethods& method );

    static void setBalances( const QHash<quint32, double>& balances);
    static void setBalance( const QJsonObject& j_balance );
    static void setRequested( const QHash<QString, QHash<quint32, double>>& req );
    static double getBalance( const quint32& currency );
    static QHash<QString, QHash<quint32, double>> getRequested();

    static QString exchangeParamPath( const QString& exchange, const QString& param );
    static bool hasSettings(const QString& settingsname );

    static ApiRcpClient * getRpcClient();
    SwiftLib();

    static QSqlQuery createQuery( const QString& dbname = "default" );
    static void loadActualConfig();
    static QString getUserDbName();

};

#endif // SWIFTLIB_H
