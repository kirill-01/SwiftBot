#include "swiftapiparserlbank.h"

QString SwiftApiParserLbank::getExchangeName() const
{
    return "lbank";
}

void SwiftApiParserLbank::parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data)
{
    const QJsonObject j_resp( QJsonDocument::fromJson( data ).object() );
    if ( j_resp.value("result").toBool(false) == false ) {
        QJsonObject j_ret;
        j_ret["success"] = false;
        j_ret["async_uuid"] = QString::number( uuid );
        emit resultParsed( uuid, j_ret );
    } else {
        if ( method == SwiftApiClient::AsyncMethods::GetBalances ) {
            QJsonObject j_ret;
            j_ret["success"] = true;
            const QJsonObject j_dat( j_resp.value("data").toObject() );
            const QJsonObject j_freeze( j_dat.value("freeze").toObject() );
            const QJsonObject j_total( j_dat.value("asset").toObject() );
            const QJsonObject j_available( j_dat.value("free").toObject() );
            QJsonArray itms;
            for ( auto it = j_total.begin(); it != j_total.end(); it++ ) {
                QJsonObject j_itm;
                const quint32 cid = SwiftLib::getAssets()->getCurrencyIdByName( it.key(), getExchangeId() );
                if ( cid > 0 ) {
                    j_itm["currency_id"] = QString::number( SwiftLib::getAssets()->getCurrencyIdByName( it.key(), getExchangeId() ));
                    j_itm["total"] = QString::number( it.value().toString().toDouble(), 'f', 8);
                    j_itm["available"] = QString::number( j_available.value( it.key() ).toString().toDouble(), 'f', 8);
                    j_itm["reserved"] = QString::number( j_freeze.value( it.key() ).toString().toDouble(), 'f', 8);
                    itms.push_back( j_itm );
                }
            }
            j_ret["balances"] = itms;
            j_ret["async_uuid"] = QString::number( uuid );
            emit resultParsed( uuid, j_ret );
        } else {
            qWarning() << data;
        }
    }


}
