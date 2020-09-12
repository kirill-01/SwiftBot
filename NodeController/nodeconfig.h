#ifndef NODECONFIG_H
#define NODECONFIG_H

#include <QObject>
#include <QProcess>
#include <QSharedPointer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QSettings>
#include <QDebug>

class NodeConfig : public QObject
{
    Q_OBJECT
public:
    explicit NodeConfig(QObject *parent = nullptr);
    NodeConfig(const QString& filepath, QObject *parent = nullptr ) : QObject( parent ), filename( filepath ), _file( filename ) {
        _file.open( QIODevice::ReadWrite );
        _config = QJsonObject( QJsonDocument::fromJson( _file.readAll().constData() ).object() );
        if ( _config.contains("workers") && _config.value("workers").isArray() ) {
            const QJsonArray workers( _config.value("workers").toArray() );
            for( auto it = workers.begin(); it != workers.end(); it ++ ) {
                if ( it->toObject().value("type").toString() == "router" ) {
                    _router = it->toObject();
                    if ( !_router.contains("options") ) {
                        QJsonObject router_options;
                        router_options["pythonpath"] = QJsonArray({"/opt/swiftbot/crossbar/"});
                        _router["options"] = router_options;
                    }
                }
            }
        }
    }

    void writeConfig() {
        _router["realms"] = getRealmsArray();
        _router["transports"] = getTransportsArray();
        _router["components"] = getComponentsArray();
        _config["workers"] = QJsonArray({_router});
        const QString config_content( QJsonDocument( _config ).toJson( QJsonDocument::Indented ) );
        _file.resize(0);
        _file.write( config_content.toUtf8().constData() );
        _file.close();
    }
    void addRealm( const QString& realm_name ) {
        if ( !_realms.contains( realm_name ) ) {
            _realms.insert( realm_name, getDefaultRealmObject( realm_name ) );
        }
    }

    void addUser( const QString& username, const QString& secret, const QString& role = "authenticated") {
        QJsonObject j_user;
        j_user["secret"] = secret;
        j_user["role"] = role;
        _users[username] = j_user;
    }

    QJsonObject getUsers() const {
        return  _users;
    }

    void addTransport( const QString& id, const QJsonObject& transport ) {
        _transports[ id ] = transport;
    }
    void addTransport( const QString& id, const QString& type, const QJsonObject& endpoint, const QJsonArray& serializers = QJsonArray(),  const QJsonObject& options = QJsonObject() ) {
        QJsonObject _tr;
        _tr["type"] = type;
        _tr["endpoint"] = endpoint;
        if ( !options.isEmpty() ) {
            _tr["options"] = options;
        }
        if ( !serializers.isEmpty() ) {
            _tr["serializers"] = serializers;
        }
        _transports[ id ] =  _tr;
    }

    QJsonObject createClassComponent( const QString& classname, const QString& realmname, const QString& role ) const {
        QJsonObject j;
        j["type"] = "class";
        j["classname"] = classname;
        j["realm"] = realmname;
        j["role"] = role;
        return  j;
    }

    void addRole( const QString& name ) {
        QJsonObject j;
        j["name"] = name;
        j["permissions"] = QJsonArray();
        _roles.insert( name, j );
    }
    void addRolePermission( const QString& role_name, const QString& uri, const QString& match, const QJsonObject& allow, const QJsonObject& disclose, const bool& cache ) {
        QJsonObject j;
        j["uri"] = uri;
        j["match"] = match;
        j["allow"] = allow;
        j["disclose"] = disclose;
        j["cache"] = cache;
        if( !_roles.contains( role_name ) ) {
            addRole( role_name );
        }
        QJsonObject role( _roles.value(role_name) );
        QJsonArray permissions( role.value("permissions").toArray() );
        for( auto it = permissions.begin(); it != permissions.end(); it++ ) {
            if ( it->toObject().value("uri").toString() == uri ) {
                permissions.removeAt( it.i );
            }
        }
        permissions.push_back( j );
        role["permissions"] = permissions;
        _roles[role_name] = role;
    }


    void addComponent( const QJsonObject & j_component ) {
        _components.push_back( j_component );
    }
private:
    QJsonArray getRealmsArray() {
        QJsonArray j_ret;
        for( auto it = _realms.begin(); it != _realms.end(); it++  ) {
            QJsonObject realm( it.value() );
            QJsonArray realmRoles;
            for( auto it = _roles.begin(); it != _roles.end(); it++ ) {
                realmRoles.push_back( it.value() );
            }
            realm["roles"] = realmRoles;

            j_ret.push_back( realm );
        }
        return j_ret;
    }

    QJsonArray getTransportsArray() {
        QJsonArray j_ret;
        for( auto it = _transports.begin(); it != _transports.end(); it++  ) {
            j_ret.push_back( it.value() );
        }
        return j_ret;
    }

    QJsonArray getComponentsArray() {
      return  _components;

    }
    QJsonObject getDefaultRealmObject(const QString& realm_name ) const {
        QJsonObject r;
        r["name"] = realm_name;
        r["roles"] = QJsonArray();
        QJsonObject j_store;
        j_store["type"] = "memory";
        QJsonArray j_uris;
        QJsonObject uri_logs;
        uri_logs["uri"] = "swiftbot.system.logs";
        uri_logs["limit"] = 100;
        QJsonObject uri_errors;
        uri_logs["uri"] = "swiftbot.system.errors";
        uri_logs["limit"] = 100;
        j_uris.push_back( uri_errors );
        j_store["event-history"] = j_uris;
        r["store"] = j_store;
        return r;
    }
    QJsonObject getAnonymousRole() const {
        QJsonObject r;
        r["name"] = "anonymous";
        r["permissions"] = QJsonArray();
        return r;
    }
    QJsonObject _router;

    QHash<QString, QJsonObject> _realms;
    QHash<QString, QJsonObject> _transports;
    QJsonArray _components;
    QHash<QString, QJsonObject> _roles;
    QJsonObject _users;

QJsonObject _config;
QString filename;
QFile _file;
};

#endif // NODECONFIG_H
