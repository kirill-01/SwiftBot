#ifndef PROCWRAPPER_H
#define PROCWRAPPER_H

#include <QObject>
#include <QProcess>
#include <QSharedPointer>
#include <QDebug>
#include <QTimer>

class ProcWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ProcWrapper(const QString& command, const QStringList& args = QStringList({}), QObject *parent = nullptr);
    QString getStatus();
signals:
 void procFinished( const quint32& exitcode );
 void isexited();
public slots:
     void stop();
     void restart();

    void onProcFinished( const quint32& exitcode );
    void start();
    void onStateChanged( QProcess::ProcessState state );
private:
    QString exchange_name;
    QString cmd;
    QStringList arg;
    QSharedPointer<QProcess> _proc;
    bool was_started;
    quint32 restarts_count;
};

#endif // PROCWRAPPER_H
