#ifndef LOGSWORKER_H
#define LOGSWORKER_H

#include <QObject>
#include "swiftworker.h"
#include <QQueue>
#include <QFile>

#define LOGS_FILE "/opt/swiftbot/logs.log"
#define ERRORS_FILE "/opt/swiftbot/errors.log"

struct Log {
    Log( const QString&m,const QString&s,const QString&g, const qint64&d);
    Log( const QJsonObject j_obj ) {
        msg = j_obj.value("m").toString();
        sender = j_obj.value("s").toString();
        group = j_obj.value("g").toString();
        ts = j_obj.value("ts").toString().toUInt();
    }
    QString msg;
    QString sender;
    QString group;
    qint64 ts;
    QJsonObject toJson();
};

class LogsWorker : public SwiftWorker
{
    Q_OBJECT
public:
    explicit LogsWorker(QObject *parent = nullptr);
public:
    void loadFromFile();

    void initWorker(Wamp::Session *sess) override;
    QString getWorkerName() const override;
    QVector<Log> _logs;
    QVector<Log> _errors;

    QQueue<Log> _logs_queue;
    QQueue<Log> _errs_queue;
public slots:
    void saveToFile();
    void processLogs();
    void processErrs();
};

#endif // LOGSWORKER_H
