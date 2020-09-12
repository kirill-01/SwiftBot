#ifndef DBWRITER_H
#define DBWRITER_H

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include <QMutex>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

class DbWriter : public QObject
{
    Q_OBJECT
public:
    explicit DbWriter(QObject *parent = nullptr);

signals:
    void storedOrders( const QJsonArray& j_arr );
public slots:
    void onSaveOrder( const QJsonObject &jitm );
    void onSaveOrderState( const QJsonObject &jitm );
    void onThreadStarted();
    void loadTodayOrders();
    void deleteOrderRec( const QJsonObject&j);
private:
    QSqlQuery saveOrder;
    QSqlDatabase _db;
    QMutex m;
};

#endif // DBWRITER_H
