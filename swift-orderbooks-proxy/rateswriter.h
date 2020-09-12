#ifndef RATESWRITER_H
#define RATESWRITER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMutex>

class RatesWriter : public QObject
{
    Q_OBJECT
public:
    explicit RatesWriter(QObject *parent = nullptr);

signals:

public slots:
    void onRates( const QMap<quint32,double> &rates );
    void onThreadStarted();
private:
    QSqlDatabase _db;
    QSqlDatabase _ch;
    QMutex m;
    QMap<quint32, double> _last_inserted_values;
};

#endif // RATESWRITER_H
