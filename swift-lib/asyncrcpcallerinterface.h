#ifndef ASYNCRCPCALLERINTERFACE_H
#define ASYNCRCPCALLERINTERFACE_H

#include <QObject>
#include "wamp.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QEventLoop>
#include <QMutex>
#include "swiftapiclient.h"

class AsyncRcpCallerInterface : public QObject
{
    Q_OBJECT
public:
    explicit AsyncRcpCallerInterface(QObject *parent = nullptr);
    void processAsyncResult( const quint64& uuid, const QJsonObject& j_result );

    QJsonObject waitResult( const QString& rpc_topic, const QJsonObject& params=QJsonObject() );

    void asyncCall( const QString& rpc_topic, const SwiftApiClient::AsyncMethods& method, const QJsonObject& params=QJsonObject() );

    SwiftApiClient::AsyncMethods getMethodByUuid( const quint64& uuid );

    QJsonObject getResultByUuid( const quint64& uuid );

    void finishRemote( const quint64& uuid );
    Wamp::Session * session;
    QMutex mutex;
    QList<quint64> _async_waiting;
    QHash<quint64, QJsonObject> _async_results;
    QHash<quint64, SwiftApiClient::AsyncMethods> _async_methods;
public slots:
    void onWampSession( Wamp::Session * sess );
    void onWampAsyncResult( const QJsonObject& result );
signals:
    void asyncFinished( const quint64& uuid );
    void readyToStart();
private:


};

#endif // ASYNCRCPCALLERINTERFACE_H
