#ifndef USERHISTORYWORKER_H
#define USERHISTORYWORKER_H

#include <QObject>
#include "../swift-lib/swiftlib.h"

class UserHistoryWorker : public QObject
{
    Q_OBJECT
public:
    explicit UserHistoryWorker(QObject *parent = nullptr);

signals:
public slots:
    void onWampSession( Wamp::Session * sess ) {
        session = sess;

    }
    void createSnapshot() {

    }
private:
    Wamp::Session * session;
};

#endif // USERHISTORYWORKER_H
