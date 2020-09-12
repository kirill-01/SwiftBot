#include "asyncrcpcallerinterface.h"

AsyncRcpCallerInterface::AsyncRcpCallerInterface(QObject *parent) : QObject(parent), session(nullptr)
{

}

void AsyncRcpCallerInterface::processAsyncResult(const quint64 &uuid, const QJsonObject &j_result) {
    _async_results.insert( uuid, j_result );
    emit asyncFinished( uuid );
}

QJsonObject AsyncRcpCallerInterface::waitResult(const QString &rpc_topic, const QJsonObject &params) {
    if ( session != nullptr && session->isJoined() ) {
        QMutexLocker locker( &mutex );
        const QString res( QJsonDocument(params).toJson( QJsonDocument::Compact ));
        const quint64 uuid = session->call( rpc_topic, {res} ).toULongLong();
        _async_waiting.push_back( uuid );
        bool is_connected_loop = false;
         QTimer * timeout = new QTimer(this);
         timeout->setInterval( 10000 );
         timeout->setSingleShot( true );
        QEventLoop * evloop = new QEventLoop();
        if ( !is_connected_loop ) {
            connect( timeout, &QTimer::timeout, evloop, &QEventLoop::quit );

            connect( this, &AsyncRcpCallerInterface::asyncFinished, [=](const quint64& res_uuid ){
                if ( res_uuid == uuid) {
                    if ( evloop->isRunning() ) {
                        evloop->quit();
                    }
                }
            });
            is_connected_loop = true;
            timeout->start();
        }
        evloop->exec();
        is_connected_loop = false;
        if ( _async_results.contains( uuid ) ) {
            QJsonObject jr( _async_results.value( uuid ) );
            return jr;
        } else {
            QJsonObject jr;
            jr["success"] = false;
            jr["error"] = "RPC call timeout";
            jr["async_uuid"] = QString::number( uuid );
            return jr;
        }
        timeout->deleteLater();
        evloop->deleteLater();
    } else {
        QJsonObject jr;
        jr["success"] = false;
        jr["error"] = "RPC client not connected!";
        return jr;
    }
    QJsonObject jr;
    jr["success"] = false;
    jr["error"] = "RPC client not connected!";
    return jr;
}

void AsyncRcpCallerInterface::asyncCall(const QString &rpc_topic, const SwiftApiClient::AsyncMethods &method, const QJsonObject &params) {
    if ( session != nullptr && session->isJoined() ) {
        const QString res( QJsonDocument(params).toJson( QJsonDocument::Compact ) );
        const quint64 uuid = session->call( rpc_topic, {res} ).toULongLong();
        _async_methods.insert( uuid, method );
        _async_waiting.push_back( uuid );
    }
}

SwiftApiClient::AsyncMethods AsyncRcpCallerInterface::getMethodByUuid(const quint64 &uuid) {
    return _async_methods.value( uuid );
}

QJsonObject AsyncRcpCallerInterface::getResultByUuid(const quint64 &uuid) {
    return _async_results.value( uuid );
}

void AsyncRcpCallerInterface::finishRemote(const quint64 &uuid) {
    _async_methods.remove( uuid );
    _async_results.remove( uuid );
    _async_waiting.removeOne( uuid );
}

void AsyncRcpCallerInterface::onWampSession(Wamp::Session *sess) {
    session = sess;
    session->subscribe( SwiftLib::getTopic( SwiftLib::WampTopics::ApiRpcAsyncResults ), [=]( const QVariantList&v, const QVariantMap&m ) {
        Q_UNUSED(m)
        const QJsonObject r( QJsonDocument::fromJson( v.at(0).toString().toUtf8()).object() );
        const quint64 uuid = r.value("async_uuid").toString().toULongLong();
        if ( _async_waiting.contains( r.value("async_uuid").toString().toULongLong() ) ) {
            processAsyncResult( uuid, r );
        }
    });
    emit readyToStart();
}

void AsyncRcpCallerInterface::onWampAsyncResult(const QJsonObject &result) {
    const quint64 uuid = result.value("async_uuid").toString().toULongLong();
    if( _async_waiting.contains( uuid ) ) {
        processAsyncResult( uuid, result );
    }
}
