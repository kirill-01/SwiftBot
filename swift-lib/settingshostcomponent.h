#ifndef SETTINGSHOSTCOMPONENT_H
#define SETTINGSHOSTCOMPONENT_H

#include <QObject>
#include "wampclient.h"

class SettingsHostComponent : public QObject
{
    Q_OBJECT
public:
    explicit SettingsHostComponent(QObject *parent = nullptr);

signals:
    void componentShutdownRequested();
    void componentStartRequested();
public slots:
    void logInfo( const QString& message );
    void logError( const QString& message );
    void sendHeartBeat();
    void registerRpcMethods();
private:
    QTimer * heartbeattimer;
    WampClient * client;
};

#endif // SETTINGSHOSTCOMPONENT_H
