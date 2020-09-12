#include "logsworker.h"

LogsWorker::LogsWorker(QObject *parent) : SwiftWorker( parent ) {
    loadFromFile();
    QTimer::singleShot( 30000, this, &LogsWorker::saveToFile );
}

void LogsWorker::loadFromFile() {
    QFile logsfile( LOGS_FILE );
    if ( logsfile.open( QFile::ReadOnly ) ) {
        QJsonArray _itms( QJsonDocument::fromJson( logsfile.readAll().constData() ).array() );
        for( auto it = _itms.begin(); it != _itms.end(); it++ ) {
            _logs.push_back( Log( it->toObject() ) );
        }
    }
    QFile errsfile( ERRORS_FILE );
    if ( errsfile.open( QFile::ReadOnly ) ) {
        QJsonArray _itms( QJsonDocument::fromJson( logsfile.readAll().constData() ).array() );
        for( auto it = _itms.begin(); it != _itms.end(); it++ ) {
            _errors.push_back( Log( it->toObject() ) );
        }
    }
}

void LogsWorker::initWorker(Wamp::Session *sess)
{
    sess->provide("swift.system.rpc.logs.log", [&](const QVariantList&v, const QVariantMap&m) {
        //[SENDER,GROUP,MESSAGE]
        if ( v.size() != 3 ) {
            return true;
        }
        const QString sender( v.at(0).toString() );
        const QString group( v.at(1).toString() );
        const QString msg( v.at(2).toString() );
        const qint64 ts = QDateTime::currentSecsSinceEpoch();
        if ( group != "DEBUG" ) {
            _logs.push_back( Log(msg,sender,group,ts) );
        }
        // [SENDER,GROUP,TS,MESSAGE]
        _logs_queue.enqueue( Log(msg,sender,group,ts) );
        Q_UNUSED(m);
        return true;
    });
    sess->provide("swift.system.rpc.logs.err", [&](const QVariantList&v, const QVariantMap&m) {
        //[SENDER,GROUP,MESSAGE]
        if ( v.size() != 3 ) {
            return true;
        }
        const QString sender( v.at(0).toString() );
        const QString group( v.at(1).toString() );
        const QString msg( v.at(2).toString() );
        const qint64 ts = QDateTime::currentSecsSinceEpoch();
        _errors.push_back( Log(msg,sender,group,ts));
        // [SENDER,GROUP,TS,MESSAGE]
        _errs_queue.enqueue( Log(msg,sender,group,ts) );
        Q_UNUSED(m);
        return true;
    });

    sess->provide("swift.system.rpc.logs.last", [&](const QVariantList&v, const QVariantMap&m) {
        //[SIZE]
        Q_UNUSED(m);
        const quint32 max_items( v.isEmpty() ? 25 : v.at(0).toUInt() );
        quint32 itms = 0;
        QJsonArray _l;
        for( auto it = _logs.rbegin(); it != _logs.rend(); it++ ) {
            if ( itms <= max_items ) {
                _l.push_back( it->toJson() );
                itms++;
            }
        }
        const QString r( QJsonDocument(_l).toJson( QJsonDocument::Compact ) );
        return r;
    });

    sess->provide("swift.system.rpc.errs.last", [&](const QVariantList&v, const QVariantMap&m) {
        //[SIZE]
        Q_UNUSED(m);
        const quint32 max_items( v.isEmpty() ? 25 : v.at(0).toUInt() );
        quint32 itms = 0;
        QJsonArray _l;
        for( auto it = _errors.rbegin(); it != _errors.rend(); it++ ) {
            if ( itms <= max_items ) {
                _l.push_back( it->toJson() );
                itms++;
            }
        }
        const QString r( QJsonDocument(_l).toJson( QJsonDocument::Compact ) );
        return r;
    });


    processLogs();
    processErrs();
    qWarning() << "Logs worker inited";

}

QString LogsWorker::getWorkerName() const
{
    return "logs";
}

void LogsWorker::saveToFile() {
    QVector<Log> __logs(_logs);
    QVector<Log> __errors(_errors);
    QJsonArray _l;
    QJsonArray _e;
    for( auto it = __logs.begin(); it != __logs.end(); it++ ) {
        _l.push_back( it->toJson() );
    }
    for( auto it = __errors.begin(); it != __errors.end(); it++ ) {
        _e.push_back( it->toJson() );
    }
    QFile logsfile( LOGS_FILE );
    if ( logsfile.open( QFile::ReadWrite ) ) {
        logsfile.resize(0);
        logsfile.write( QJsonDocument( _l ).toJson( QJsonDocument::Compact ) );
        logsfile.close();
    }
    QFile errsfile( ERRORS_FILE );
    if ( errsfile.open( QFile::ReadWrite ) ) {
        errsfile.resize(0);
        errsfile.write( QJsonDocument( _e ).toJson( QJsonDocument::Compact ) );
        errsfile.close();
    }
    QTimer::singleShot( 30000, this, &LogsWorker::saveToFile );
}

void LogsWorker::processLogs() {
    if ( !_logs_queue.isEmpty() ) {
        while ( !_logs_queue.isEmpty()) {
            Log next( _logs_queue.dequeue() );
            const QJsonObject msg({{"s",next.sender},{"g",next.group},{"m",next.msg},{"ts",QString::number( next.ts )}});
            const QString m( QJsonDocument ( msg ).toJson( QJsonDocument::Compact ) );
            session->publish("swift.system.feed.logs", {m});
        }
    }
    QTimer::singleShot( 500, this, &LogsWorker::processLogs );
}

void LogsWorker::processErrs() {
    if ( !_errs_queue.isEmpty() ) {
        while ( !_errs_queue.isEmpty()) {
            Log next( _errs_queue.dequeue() );
            const QJsonObject msg({{"s",next.sender},{"g",next.group},{"m",next.msg},{"ts",QString::number( next.ts )}});
            const QString m( QJsonDocument ( msg ).toJson( QJsonDocument::Compact ) );
            session->publish("swift.system.feed.errs", {m});
        }
    }
    QTimer::singleShot( 500, this, &LogsWorker::processErrs );
}

Log::Log(const QString &m, const QString &s, const QString &g, const qint64 &d)
    : msg(m),sender(s),group(g),ts(d) {
}

QJsonObject Log::toJson() {
    return QJsonObject({{"m",msg},{"s",sender},{"g",group},{"ts",QString::number(ts)}});
}
