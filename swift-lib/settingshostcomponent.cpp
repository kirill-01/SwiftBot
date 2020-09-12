#include "settingshostcomponent.h"
#include "swiftlib.h"
SettingsHostComponent::SettingsHostComponent(QObject *parent)
    : QObject(parent),
      client( new WampClient(
                  SwiftLib::getSettings()->value(SETTINGS_WAMP_REALM, "vel").toString(),
                  SwiftLib::getSettings()->value(SETTINGS_WAMP_SERVER, "cc.finance").toString(),
                  SwiftLib::getSettings()->value("WAMP_PORT_WORKERS_SETTINGS", 8091).toInt() )
              ), heartbeattimer( new QTimer() )
{
    heartbeattimer->setInterval(2000);
    connect( client, &WampClient::clientConnected, [=](Wamp::Session*s ){
        registerRpcMethods();
        heartbeattimer->start();
    });
    connect( heartbeattimer, &QTimer::timeout, this, &SettingsHostComponent::sendHeartBeat );
    client->startClient();
}

void SettingsHostComponent::logInfo(const QString &message) {
    client->getSession()->publish("swiftbot.system.info", QVariantList({message}));
}

void SettingsHostComponent::logError(const QString &message) {
    client->getSession()->publish("swiftbot.system.errors", QVariantList({message}));
}

void SettingsHostComponent::sendHeartBeat() {
    QJsonObject j_rep;
    j_rep["component"] = "BOT";
   // client->getSession()->call("swiftbot.system.heartbeat", QVariantList({j_rep }));
}

void SettingsHostComponent::registerRpcMethods() {

}
