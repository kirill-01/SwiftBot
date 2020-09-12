#include "swiftworker.h"

SwiftWorker::SwiftWorker(QObject *parent) : QObject(parent),session(nullptr),wdtimer( new QTimer(this) )
{
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost)
             hostip = address.toString();
    }
    status = 1;
    QObject::connect( this, &SwiftWorker::restartRequested, this, &SwiftWorker::onRestart );
    QObject::connect( wdtimer, &QTimer::timeout, this, &SwiftWorker::watchdog );
    wdtimer->setInterval( 25000 );
    wdtimer->start();

}

void SwiftWorker::setStatus(const quint32 &stat) {
    status = stat;
}

quint32 SwiftWorker::getStatus() const {
    return status;
}

void SwiftWorker::setIsInited() {
    if ( session != nullptr && session->isJoined() ) {
        const QString json_data( QJsonDocument( QJsonObject({{"module",getWorkerName()},{"ts",QString::number(QDateTime::currentSecsSinceEpoch())}}) ).toJson( QJsonDocument::Compact ) );
        session->publish("swift.system.feed.events", {quint32(SwiftLib::WampSystemEvents::WorkerStarted),json_data });
    }
}

void SwiftWorker::watchdog() {
    if ( session != nullptr && session->isJoined() ) {
        session->publish("swift.system.feed.watchdog", { getWorkerName() });
    } else {
        emit restartRequested();
    }
}

void SwiftWorker::onRestart() {
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    if ( session != nullptr && session->isJoined() ) {
        const QString json_data( QJsonDocument( QJsonObject({{"module",getWorkerName()},{"ts",QString::number(QDateTime::currentSecsSinceEpoch())}}) ).toJson( QJsonDocument::Compact ) );
        session->publish("swift.system.feed.events", {quint32(SwiftLib::WampSystemEvents::WorkerRestarting),json_data });
    }
    qApp->quit();
}

void SwiftWorker::onWampSession(Wamp::Session *sess) {
    if ( session == nullptr ) {
        session = sess;
    }
    // API v2.1
    session->provide("swift.system.rpc.module.status."+getWorkerName(),[&](const QVariantList&v, const QVariantMap&m) {
        //[STATUS,HOST,VERSION,JSONDATA]
        Q_UNUSED(m)
        Q_UNUSED(v)
        const QJsonArray j_ret({QString::number( status ), hostip, APP_VERSION, "{}" });
        const QString _r( QJsonDocument(j_ret ).toJson( QJsonDocument::Compact ) );
        return _r;
    });

    session->provide("swift.system.rpc.module.cmd."+getWorkerName(),[&](const QVariantList&v, const QVariantMap&m) {
        //[CMD]
        Q_UNUSED(m)
        const QString cmd( v.at(0).toString() );
        if ( cmd == "pause" ) {
            setStatus( 0 );
        } else if ( cmd == "start" ) {
            setStatus( 1 );
        } else if ( cmd == "restart" ) {
            emit restartRequested();
            setStatus( 0 );
        }
        return true;
    });

    session->subscribe("swift.assets.feed.settings",[&](const QVariantList&v, const QVariantMap&m) {
        //[PARAM,VALUE]
        if ( !v.isEmpty() ) {
            const QString param( v.at(0).toString() );
            if ( listenParams().contains( param ) ) {
                onParamChanged( param, v.at(1).toString() );
            }
        }
    });

    _ch = QSqlDatabase::addDatabase("QMYSQL", "clickhouse-"+getWorkerName() );
    _ch.setHostName( SwiftLib::getSettings()->value(SETTINGS_MYSQL_HOST ).toString() );
    _ch.setPort( 9004 );

    _ch.setUserName( "default" );
    _ch.setPassword( "clickhouse" );
    _ch.setDatabaseName( "default" );
    if ( !_ch.open() ) {
        qWarning() << _ch.lastError().text();
    } else {
        initWorker( session );
        setIsInited();
    }
}
