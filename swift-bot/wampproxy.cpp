#include "wampproxy.h"

WampProxy::WampProxy(QObject *parent) : QObject(parent), wamp_client( new WampClient(
                                                                          SwiftLib::getSettings()->value(SETTINGS_WAMP_REALM).toString(),
                                                                          "cc.finance",
                                                                          8081
                                                                          ) )
{
    connect( wamp_client, &WampClient::clientConnected, this, &WampProxy::onWampSessionStarted );

}

void WampProxy::onMoneyShiftState(const QJsonObject &j_state) {
    if ( wamp_client->getSession() != nullptr && wamp_client->getSession()->isJoined() ) {
        const QString _json( QJsonDocument(j_state).toJson(QJsonDocument::Compact) );
        wamp_client->getSession()->publish( "money.shift.status", QVariantList({ _json }) );
    }
}

void WampProxy::onTransferCandidats(const QJsonArray &candidats) {
    if ( wamp_client->getSession() != nullptr && wamp_client->getSession()->isJoined() ) {
        const QString candidats_json( QJsonDocument(candidats).toJson(QJsonDocument::Compact) );
        wamp_client->getSession()->publish( "transfer_candidats", QVariantList({ candidats_json }) );
    }
}

void WampProxy::onUserState(const QJsonObject &j_state) {
    if ( session != nullptr && session->isJoined() ) {
        const QString json( QJsonDocument(j_state).toJson(QJsonDocument::Compact) );
        session->publish( QString::number( WampClient::ch_userstate ), QVariantList({ json }) );
    }
}

void WampProxy::onCandidatsFiltered(const QList<ArbitrageCandidat> &_filtered_candidats) {
    if ( session != nullptr && session->isJoined() ) {
        QJsonArray _candidats_json;
        for( auto it = _filtered_candidats.begin(); it != _filtered_candidats.end(); it++ ) {
            _candidats_json.push_back( it->toJson() );
        }
        const QString candidats_json( QJsonDocument(_candidats_json).toJson(QJsonDocument::Compact) );
        session->publish( QString::number( WampClient::ch_candidats ), QVariantList({ candidats_json }) );
    }
}

void WampProxy::onWampSessionStarted(Wamp::Session *sess) {

    session = sess;
    session->provide("crypto.addresses.get.all", [&]( const QVariantList& v, const QVariantMap& m) {
        QJsonObject addresses;
        QSqlQuery q("SELECT * FROM currencies WHERE is_enabled=1 AND deposit_address IS NOT NULL AND deposit_address !=''");
        if ( q.exec() ) {
            while ( q.next() ) {
                addresses.insert( QString::number( q.value("id").toUInt()), q.value("deposit_address").toString() );
            }
        }
        Q_UNUSED(v)
        Q_UNUSED(m)
        const QString res(  QJsonDocument( addresses ).toJson( QJsonDocument::Compact ) );
        return res;
    });

    session->subscribe("swiftbot.orderbooks.proxy", [&]( const QVariantList& v, const QVariantMap& m) {
        emit orderbooks( v );
        Q_UNUSED(m)
    });


   // QTimer::singleShot( 15000, this, &WampProxy::sendReloadCommand );


    emit sessionStarted( session );
    subscrubeToAsync();
}

void WampProxy::sendReloadCommand() {
    wamp_client->getSession()->publish("monitor.reload", QVariantList({true}) );
}
