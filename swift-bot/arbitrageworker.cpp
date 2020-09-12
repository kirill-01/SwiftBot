#include "arbitrageworker.h"



QStringList ArbitrageWorker::listenParams()
{
    return QStringList({"BTCUSDPAIR","ETHUSDPAIR","MIN_WINDOW_SPREAD","MIN_ORDER_SIZE","MIN_WINDOW_SPREAD","AMOUNT_PRECISION","MAX_ORDER_SIZE","ORDER_STEP_SIZE","RATE_PRECISION"});
}

void ArbitrageWorker::onParamChanged(const QString &param, const QString &value)
{
    manager->updateParam( param, value );
    // RESTART MANAGER ?
}

void ArbitrageWorker::initWorker(Wamp::Session *sess)
{
    manager->onWampSession( sess );
}

QString ArbitrageWorker::getWorkerName() const
{
    return "arbitrage";
}
