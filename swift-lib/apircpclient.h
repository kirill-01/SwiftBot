#ifndef APIRCPCLIENT_H
#define APIRCPCLIENT_H

#include <QObject>
#include "asyncrcpcallerinterface.h"
class ApiRcpClient : public AsyncRcpCallerInterface
{
    Q_OBJECT
public:
    explicit ApiRcpClient( QObject * parent = nullptr);
    void useClient( Wamp::Session * sess ) {
            session = sess;
    }
signals:
    void orderbooks( const QJsonObject& j_result );
    void open_orders( const QJsonObject& j_result );
public slots:
    void onMethodFinished( const quint64& uuid );
};

#endif // APIRCPCLIENT_H
