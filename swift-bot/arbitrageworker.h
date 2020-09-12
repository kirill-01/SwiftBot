#ifndef ARBITRAGEWORKER_H
#define ARBITRAGEWORKER_H

#include <QObject>
#include "swiftworker.h"
#include "arbitragemanager.h"
class ArbitrageWorker : public SwiftWorker
{
    Q_OBJECT
public:
    ArbitrageWorker( QObject * parent = nullptr ) : SwiftWorker(parent), manager( new ArbitrageManager(this) ) {

    }

    // SwiftWorker interface
public:
    QStringList listenParams() override;
    void onParamChanged(const QString &param, const QString &value) override;
    void initWorker(Wamp::Session *sess) override;
    QString getWorkerName() const override;
    ArbitrageManager * manager;
};

#endif // ARBITRAGEWORKER_H
