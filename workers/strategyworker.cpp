#include "strategyworker.h"
#include <QNetworkInterface>
#define APP_VERSION "2.0.1956"
#include <QProcess>

StrategyWorker::StrategyWorker(QObject *parent) : QObject(parent),
    assets( SwiftLib::getAssets() ),
    session(nullptr),
    max_finance_percent_to_use(100),
    target_arbitrage_pair(0), target_window_id(0)

{
    target_window_id = 91;
    target_arbitrage_pair = SwiftLib::getSettings()->value("STRATEGY_LOCKER_ARBITRAGE_PAIR", "2").toString().toUInt();
    max_finance_percent_to_use = SwiftLib::getSettings()->value("STRATEGY_LOCKER_MAX_FINANCE", "100.00").toString().toDouble();
}

void StrategyWorker::onWampSession(Wamp::Session *s)
{
    session = s;
    QJsonObject msg;
    msg["text"] = "STRATEGY_MODULE_STARTED";
    msg["cssClass"] = "ordersuccess";
    msg["closeTimeout"] = "1000";
    msg["position"] = "top";
    session->publishMessage("swiftbot.system.messages", msg);

    session->provide("swiftbot.components.strategy.status", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        Q_UNUSED(v)
        QString hostip;
        const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
        for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost && address != QHostAddress("10.9.0.1") )
                 hostip = address.toString();
        }
        QJsonObject j_ret;
        j_ret["status"] = "running";
        j_ret["name"] = "worker_strategy";
        j_ret["location"] = hostip;
        j_ret["version"] = APP_VERSION;
        const QString str(QJsonDocument(j_ret).toJson( QJsonDocument::Compact ) );
        return str;
    });

    session->provide("swiftbot.components.strategy.command", [&](const QVariantList&v, const QVariantMap&m){
        Q_UNUSED(m)
        const QString cmd = v.at(0).toString();
        if ( cmd == "restart") {
            qApp->exit();
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
        }
        return true;
    });

    getBalancesInfo();
}
