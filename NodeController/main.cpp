#include <QCoreApplication>
#include "../swift-lib/swiftlib.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>

#define APP_NAME "Swift exchange API node"
#define APP_VERSION "0.1.1"

#include <QLockFile>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QCommandLineParser>
#include <QProcess>
#include <QSharedPointer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QSettings>

#include "nodeconfig.h"
#define NODE_CONFIG_DEFAULT_LOCATION "/crossbar/.crossbar/config.json"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    NodeConfig node_config(NODE_CONFIG_DEFAULT_LOCATION);

    node_config.addRealm("vel");
    node_config.addRealm("kaa");
    node_config.addRealm("sv");
    node_config.addRealm("orderbooks");

    node_config.addRole("storage");
    node_config.addRole("web");


    node_config.addRole("controller");

    QJsonObject router_component;
    router_component["id"] = "apinodes-vel";
    router_component["type"] = "class";
    router_component["classname"] = "ccfinance.Connector";
    router_component["realm"] = "vel";
    router_component["role"] = "controller";

    node_config.addComponent( router_component );
    router_component["id"] = "apinodes-kaa";
    router_component["realm"] = "kaa";
    node_config.addComponent( router_component );
    router_component["id"] = "apinodes-sv";
    router_component["realm"] = "sv";
    node_config.addComponent( router_component );
    router_component["id"] = "apinodes-orderbooks";
    router_component["realm"] = "orderbooks";
    node_config.addComponent( router_component );

    QJsonObject storage;
    storage["publish"] = true;
    storage["register"] = true;

    QJsonObject readonly;
    readonly["publish"] = false;
    readonly["register"] = false;
    readonly["subscribe"] = true;
    readonly["call"] = true;

    QJsonObject controllerRole;
    controllerRole["publish"] = true;
    controllerRole["register"] = true;
    controllerRole["subscribe"] = true;
    controllerRole["call"] = true;

    node_config.addRolePermission("storage", "swiftbot.storage", "prefix", storage, QJsonObject(), true );
    node_config.addRolePermission("web", "swiftbot.web", "prefix", readonly, QJsonObject(), true );
    node_config.addRolePermission("controller", "", "prefix", controllerRole, QJsonObject(), false );


    QJsonObject j_transport;
    j_transport["type"] = "unix";
    j_transport["path"] = "/opt/swiftbot/sockets/local_components";
    node_config.addTransport("socket_local", "websocket", j_transport );

    QJsonObject j_transport_ws;
    QJsonObject endpoint_tcp;
    endpoint_tcp["type"] = "tcp";
    endpoint_tcp["port"] = 8080;

    j_transport_ws["type"] = "universal";
    j_transport_ws["endpoint"] = endpoint_tcp;
    QJsonObject rawsockets;
    rawsockets["serializers"] = QJsonArray({"cbor", "msgpack", "ubjson", "json"});
    j_transport_ws["rawsocket"] = rawsockets;

    QJsonObject ws_ws;
    ws_ws["type"] = "websocket";
    ws_ws["serializers"] = QJsonArray({"cbor", "msgpack", "ubjson", "json"});

    QJsonObject ws_authws;
    ws_authws["type"] = "websocket";
    QJsonObject staticAuth;
    staticAuth["type"] = "static";
    staticAuth["users"] = node_config.getUsers();
    ws_authws["auth"] = staticAuth;

            QJsonObject wwws;
            wwws["ws"] = ws_ws;
            wwws["auth_ws"] = ws_authws;
    j_transport_ws["websocket"] = wwws;

    node_config.addTransport("universal_remote", "universal", j_transport_ws );

    node_config.writeConfig();
    return a.exec();
}
