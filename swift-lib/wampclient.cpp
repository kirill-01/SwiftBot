#include "wampclient.h"
#include "swiftlib.h"

WampClient::WampClient(const QString &realmname, const QString &servername, const quint16 &serverport, bool _debug, QObject *parent):
    QObject(parent),
    debug(_debug),
    started_at(QDateTime::currentDateTime()),
    sysinf_timer(new QTimer), p_realmname(realmname),
    p_servername(servername), p_serverport(serverport),session(nullptr), reconnect_limit(0),is_connected(false)
{

    connect(&m_webSocket, &QTcpSocket::disconnected, this, &WampClient::closeConnections );
    connect(&m_webSocket, &QTcpSocket::aboutToClose, this, &WampClient::closeConnections );


}

WampClient::WampClient(const QString &realmname) : QObject(nullptr),debug(SwiftLib::getSettings()->value("WAMP_DEBUG",0)==1), p_realmname(realmname),  p_servername(""),p_serverport(0) ,is_connected(false) {

}

WampClient::WampClient(QObject *parent) : QObject(parent), debug(0), p_realmname(""),  p_servername("localhost"),p_serverport(8081) , session(nullptr), is_connected(false) {
    connect(&m_webSocket, &QTcpSocket::disconnected, this, &WampClient::closeConnections );
    connect(&m_webSocket, &QTcpSocket::aboutToClose, this, &WampClient::closeConnections );
}

Wamp::Session *WampClient::getSession() const {
    return session;
}

void WampClient::connectRealm(const QString &realmname) {
    p_realmname = realmname;
    if ( session != nullptr && session->isJoined() ) {
        closeConnections();
    } else {
        startClient();
    }
    emit systemText( "Connecting to realm: " + realmname );
}


void WampClient::publishMessage(const QString &topic, const QJsonObject &j) {
    if ( session != nullptr && session->isJoined() ) {
        const QString res( QJsonDocument( j ).toJson( QJsonDocument::Compact ) );
        session->publish( topic, QVariantList({res}));
    }
}

void WampClient::startClient() {
    if (debug) {
        qInfo() << "Starting WAMP client node :" << p_realmname << p_servername << p_serverport;
    }

    if ( p_realmname == "orderbooks" && p_servername == "" ) {
        if ( m_socket.isOpen() ) {
            m_socket.close();
        }
        if ( session != nullptr ) {
            session = nullptr;
        }
        session = new Wamp::Session( m_socket, Wamp::Session::MessageFormat::Msgpack, SwiftLib::getSettings()->value("WAMP_DEBUG", 0).toUInt() == 1);
        connect(&m_socket, &QLocalSocket::connected, this, &WampClient::onConnected);
        connect(&m_socket, &QLocalSocket::disconnected, this, &WampClient::closeConnections);
        connect( session, &Wamp::Session::left, this, &WampClient::onDisconnected );
        if (!is_connected) {
            m_socket.connectToServer("/opt/swiftbot/socket1",QLocalSocket::ReadWrite);
        }
    } else {

        if ( m_webSocket.isOpen() ) {
            m_webSocket.close();

        }
        if ( session != nullptr ) {
            disconnect(&m_webSocket, &QTcpSocket::connected, this, &WampClient::onConnected);
            disconnect( this, &WampClient::clientJoined, this, &WampClient::onJoined );
            //disconnect( session, &Wamp::Session::left, this, &WampClient::onDisconnected );
        }

        session = new Wamp::Session( m_webSocket, Wamp::Session::MessageFormat::Msgpack, SwiftLib::getSettings()->value("WAMP_DEBUG", 0).toUInt() == 1);
        connect(&m_webSocket, &QTcpSocket::connected, this, &WampClient::onConnected);
        connect( this, &WampClient::clientJoined, this, &WampClient::onJoined );
        connect( session, &Wamp::Session::left, this, &WampClient::onDisconnected );
        if (!is_connected) {
            m_webSocket.connectToHost( p_servername , p_serverport);
        }
    }
}

void WampClient::onJoined() {
    // const QString async_topic( SwiftLib::getTopic( SwiftLib::ApiRpcAsyncResults ) );
}

void WampClient::onConnected() {
    is_connected=true;
    reconnect_limit = 0;
    started_at = QDateTime::currentDateTime();
    if (debug ) {
        qDebug() << "WAMP connected to " << p_servername << p_serverport;
    }

    QObject::connect(session, &Wamp::Session::joined, [&](qint64 s) {

        if (debug ) {
            qWarning() << "Session joined to realm" << p_realmname << "with session ID " << s;
        }
        sessionname = QString::number( s );
        is_connected=true;

        emit clientJoined();
        emit clientConnected( session );
     });

    // При начале сессии
    connect(session, &Wamp::Session::started, [&]() {
        if (debug ) {
            qWarning() << "Session started on realm" << p_realmname;
        }
        session->join(p_realmname);
    });

    // При завершении сессии инциируем отключение
    connect( session, &Wamp::Session::left, [&] ( const QString& reason ){
        if (debug ) {
            qWarning() << "Session left: " << reason ;
        }
        qWarning() << reason;
        emit clientdiconnected();
    });

    session->start();


}

void WampClient::onDisconnected( const QString& realm ) {
    if ( is_connected ) {
        is_connected=false;
        qWarning() << "WAMP client disconnected." << "Realm/Reason" << realm;
        const quint32 waitsecs = 2;
        if ( SwiftLib::getSettings()->value( SETTINGS_WAMP_RECONNECT_LIMIT, 25 ).toUInt() >= reconnect_limit ) {
            reconnect_limit++;
            qWarning() << "Reconnecting after" << waitsecs << "seconds";
            if ( !is_connected ) {
                QTimer::singleShot( waitsecs * 1000, this, &WampClient::startClient );
            }
        } else {
            qWarning() << "Maxmum connections attemps limit";
            emit isExited();
        }
    }

}

void WampClient::closeConnections() {
    if ( is_connected ) {
        if ( session != nullptr ) {
            if ( session->isJoined() ) {
                session->leave();
            }
        }
        session->deleteLater();
        qWarning() << "WAMP CLOSED!";
        onDisconnected(p_realmname);
    }
}
