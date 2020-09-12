#ifndef APITEST_H
#define APITEST_H

#include <QObject>
#include "../swift-lib/swiftapiclient.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>

class ApiTest : public QObject
{
    Q_OBJECT
public:

    explicit ApiTest(const SwiftApiClient::AsyncMethods &method, const QJsonObject& params, QObject *parent = nullptr) :
        QObject( parent ),
        j_params( params ),
        p_method( method ) {

    }


signals:
    void testStarted( const SwiftApiClient::AsyncMethods &method );
    void testSuccess( const SwiftApiClient::AsyncMethods &method );
    void testFailed( const SwiftApiClient::AsyncMethods &method );
public slots:
    void onAsyncResult(const SwiftApiClient::AsyncMethods &method, const QJsonObject& result ) {

    }
    void startTest() {
        emit testStarted( p_method );
    }
private:
    QJsonObject j_params;
    SwiftApiClient::AsyncMethods p_method;

};

#endif // APITEST_H
