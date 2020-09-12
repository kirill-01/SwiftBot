#ifndef WAMPCLIENT_H
#define WAMPCLIENT_H
#include "wamp.h"
#include <QMutex>
#include <QTimer>
#include <QLocalSocket>


class WampClient : public QObject
{
    Q_OBJECT
public:
    WampClient(const QString & realmname, const QString & servername, const quint16& serverport, bool debug=false, QObject *parent = nullptr);
    WampClient( const QString& realmname );
    explicit WampClient( QObject * parent = nullptr );
    Wamp::Session * getSession() const;
    void setOrderbooksSubscriber();
    void subscribe( const QString& topic );
signals:

    void clientConnected( Wamp::Session * session );
    void clientdiconnected();
    void clientJoined();
    void asyncResult( const QJsonObject& result );
    void isExited();
    void orderbooksDataReceived( const QJsonObject& jdata );
    void messageReceived( const QString& topic, const QString& message );
    void systemText( const QString& text );
public slots:
    void connectRealm( const QString& realmname );
    void publishMessage( const QString& topic, const QJsonObject& j );
    void startClient();
    void onJoined();
    void onConnected();
    void onDisconnected( const QString& realm );

    void closeConnections();
private:
    bool debug;
    QDateTime started_at;
    QTimer * sysinf_timer;
    QString p_realmname;
    QString p_servername;

    quint16 p_serverport;
    QLocalSocket m_socket;
    QTcpSocket m_webSocket;
    Wamp::Session *session;
    quint16 reconnect_limit;
    QString sessionname;
    bool is_connected;
    QMutex mutex;
};

#endif // WAMPCLIENT_H
