#include "addresswatcher.h"

AddressWatcher::AddressWatcher(const QString& address, QObject *parent) : QObject(parent), _address(address), netman( new QNetworkAccessManager() ), t(new QTimer())
{
    t->setInterval(20000);
    connect( t, &QTimer::timeout, this, &AddressWatcher::sendRequest );
    connect( netman, &QNetworkAccessManager::finished, this, &AddressWatcher::onResponse );
    t->start();
}
