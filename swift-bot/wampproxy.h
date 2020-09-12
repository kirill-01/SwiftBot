#ifndef WAMPPROXY_H
#define WAMPPROXY_H

#include <QObject>
#include "../swift-lib/swiftlib.h"
#include "../swift-lib/wampclient.h"
#include "../swift-lib/apircpclient.h"
#include "arbitragecandidat.h"

class WampProxy : public QObject
{
    Q_OBJECT
public:
    explicit WampProxy(QObject *parent = nullptr);
    ApiRcpClient * rpc;
    WampClient * wamp_client;
    Wamp::Session * session;
signals:
    void orderbooks( const QVariantList& vars );
    void balances( const QJsonObject& obj );
    void sessionStarted( Wamp::Session* session );
    void closed();

public slots:
    void subscrubeToAsync() {
        session->subscribe("swiftbot.components.heartbeat",[&]( const QVariantList& v, const QVariantMap& m) {

        });

        session->provide("swiftbot.components.bot.rpc",[&]( const QVariantList& v, const QVariantMap& m) {
            return true;
        });

        const QString topicasync( SwiftLib::getTopic( SwiftLib::WampTopics::ApiRpcAsyncResults ) );
        session->subscribe(topicasync, [&]( const QVariantList& v, const QVariantMap& m) {
            const QJsonObject _res( QJsonDocument::fromJson( v.at(0).toString().toUtf8() ).object() );

             if ( _res.contains("balances") && _res.value("balances").isArray() ) {
                const QJsonArray _bals( _res.value("balances").toArray() );
                if ( !_bals.isEmpty() ) {
                    for( auto it = _bals.begin(); it != _bals.end(); it++ ) {
                        SwiftLib::setBalance( it->toObject( ) );
                    }
                }
            }
            Q_UNUSED(m)
        });

    }

   void onMoneyShiftState( const QJsonObject& j_state );
    void onTransferCandidats( const QJsonArray& candidats );
    void onUserState( const QJsonObject& j_state );
    void onCandidatsFiltered( const QList<ArbitrageCandidat>& _filtered_candidats );
    void onWampSessionStarted( Wamp::Session * sess );
    void sendReloadCommand();
private:

};

#endif // WAMPPROXY_H
