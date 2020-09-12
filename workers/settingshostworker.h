#ifndef SETTINGSHOSTWORKER_H
#define SETTINGSHOSTWORKER_H
#include "wampclient.h"
#include <QObject>
#include <QSettings>
#include <QProcess>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include "swiftlib.h"

class SettingsHostWorker : public QObject
{
    Q_OBJECT
public:
    explicit SettingsHostWorker(QObject *parent = nullptr);
    void setJsonConfig( const QString& conf );
    QString getJsonConfig();
    Wamp::Session * session;
    bool service_active;
    QString getMethodsStatus();
    QString getDbName() const;
    quint32 serviceMode() const {
        return _current_mode;
    }
    QNetworkAccessManager * netMan;


signals:
    void markMethodSuccess( const QString& method );
    void markMethodFailed( const QString& method );
    void newParam( const QString& param, const QString& value );
public slots:
    QJsonObject getMemUsage();
    void defineServiceMode();
    void onMethodSuccess( const QString& method );
    void onMethodFailed( const QString& method );

    void onNewParam( const QString& param, const QString& value );
    void loadParams();
    void loadAssets();
    void onWampSession( Wamp::Session * s );
    void getWithdrawFeesFromApi();
    void getTradeFeesFromApi();
private:
    QList<quint64> _async_waiters;
    QHash<quint64, QString> _parsers;
    QJsonObject getExchangesFees();
    QJsonObject getCurrenciesFees();
    AssetsStorage * assets;
    QJsonObject j_assets_cached;
    bool _balances_collected;
    bool _windows_data_collected;
    bool _has_small_balances;
    quint32 _current_mode;

    QString getParam( const QString& param );
    QString getParamUnlocked( const QString& param );
    void saveParam( const QString& param, const QString& value );
    QHash<QString,quint64> _success_methods;
    QHash<QString,quint64> _error_methods;
    QMutex mutex;
    QHash<QString,QString> _user_actions;

    QHash<QString,QString> _cached_params;
    QList<quint32> _active_windows;
    QVector<QVector<quint32>> _used_chains;
    QString dbname;
};

#endif // SETTINGSHOSTWORKER_H
