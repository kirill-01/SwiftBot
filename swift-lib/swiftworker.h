#ifndef SWIFTWORKER_H
#define SWIFTWORKER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QMutex>
#include "wampclient.h"
#include "swiftlib.h"
#include <QNetworkInterface>
#include <QProcess>

#ifndef APP_NAME
#define APP_NAME "Swift worker"
#endif
#ifndef APP_VERSION
#define APP_VERSION "2.0.2016"
#endif
class SwiftWorker : public QObject
{
    Q_OBJECT
public:
    QString getConfigValue( const QString& param ) {
        if ( session != nullptr && session->isJoined() ) {
            const QString res = session->call("swift.assets.rpc.settings.get",{param}).toStringList().at(0);
            return res;
        } else {
            return "0";
        }
    }
    void insertLog( const QString& group, const QString& message ) {
       // swift.system.rpc.logs.log [SENDER,GROUP,MESSAGE]
        if ( session != nullptr && session->isJoined() ) {
            session->call("swift.system.rpc.logs.log", {getWorkerName(), group, message });
        }
    }
    void insertError( const QString& group, const QString& message ) {
       // swift.system.rpc.logs.error [SENDER,GROUP,MESSAGE]
        if ( session != nullptr && session->isJoined() ) {
            session->call("swift.system.rpc.logs.error", {getWorkerName(), group, message });
        }
    }
    explicit SwiftWorker(QObject *parent = nullptr);

    virtual QStringList listenParams() {
        return QStringList({});
    }
    virtual void onParamChanged( const QString& param, const QString& value ) {
        Q_UNUSED(param)
        Q_UNUSED(value)
    }
    virtual void initWorker( Wamp::Session * sess )=0;
    virtual QString getWorkerName() const=0;
    void setStatus( const quint32& stat );
    quint32 getStatus() const;
    QSqlDatabase _ch;
    Wamp::Session * getSession() {
        return session;
    }

    Wamp::Session * session;
    void setIsInited();
signals:
    void restartRequested();
public slots:
    void watchdog();
    void onRestart();
    void onWampSession( Wamp::Session * sess );
private:
    quint32 status;
    QString hostip;
    QTimer * wdtimer;


};

#endif // SWIFTWORKER_H
