#include "apircpclient.h"

ApiRcpClient::ApiRcpClient(QObject *parent) : AsyncRcpCallerInterface(parent) {
    connect( this, &AsyncRcpCallerInterface::asyncFinished, this, &ApiRcpClient::onMethodFinished );
}

void ApiRcpClient::onMethodFinished(const quint64 &uuid) {

    if ( _async_waiting.contains( uuid ) && _async_results.contains( uuid ) ) {
        const QJsonObject j_result( getResultByUuid( uuid ) );
        const SwiftApiClient::AsyncMethods method = getMethodByUuid( uuid );
        if ( method == SwiftApiClient::AsyncMethods::GetOrderbooks ) {
            emit orderbooks( j_result );
        }
    }

}
