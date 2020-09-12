#include "orderparams.h"


OrderParams::OrderParams(QObject *parent) : QObject(parent), local_id(""),remote_id(""),amount(0),
    amount_left(0),rate(0),price(0), exchange_id(0),type(3),market_id(0),base_currency_id(0),price_currency_id(0),status(0),arb_window(0)
{

}

void OrderParams::setOrderParams(const quint32 &_market_id, const double &_amount, const double &_rate, const quint32 &_type) {
    market_id = _market_id;
    exchange_id = SwiftLib::getAssets()->getMarketExchangeId( _market_id );
    amount = _amount;
    amount_left = _amount;
    rate = _rate;
    type = _type;
    price = _rate * amount;
    base_currency_id = SwiftLib::getAssets()->getMarketBaseCurrency( _market_id );
    price_currency_id = SwiftLib::getAssets()->getMarketPriceCurrency( _market_id );
    fee = amount * rate * SwiftLib::getExchangeFee( exchange_id );
    local_id = createLocalId();
    arb_window = 0;
    status = 0;
}

QString OrderParams::getRateString() const {
    return QString::number( rate, 'f', 8 );
}

QString OrderParams::getAmountString() const {
    return QString::number( amount,'f', 8 );
}

QString OrderParams::getRpcPath() const {
    return SwiftLib::getRpcPath( exchange_id, SwiftLib::WampExchangesRpcMethods::OrderPlace );
}

QString OrderParams::getLocalId() const {
    return local_id;
}

OrderParams::OrderParams(const QJsonObject &j_obj, QObject *parent) : QObject(parent) {
    type = j_obj.value("type").toString() == "sell" ? 0 : j_obj.value("type").toString() == "buy" ? 1 : 3;
    local_id = j_obj.value("local_id").toString("");
    remote_id = j_obj.value("remote_id").toString("");
    amount = j_obj.value("amount").toString().toDouble();
    amount_left = j_obj.value("amount_left").toString().toDouble();
    rate = j_obj.value("rate").toString().toDouble();
    price = j_obj.value("price").toString().toDouble();
    exchange_id = j_obj.value("exchange_id").toString().toUInt();
    market_id = j_obj.value("market_id").toString().toUInt();
    status = j_obj.value("status").toString().toUInt();
    fee = SwiftLib::getExchangeFee( exchange_id );
    base_currency_id = SwiftLib::getAssets()->getMarketBaseCurrency( market_id );
    price_currency_id = SwiftLib::getAssets()->getMarketPriceCurrency( market_id );
    arb_window = j_obj.value("arb_window").toString("0").toUInt();
}

OrderParams::OrderParams(const QSqlRecord &record, QObject *parent) : QObject( parent ) {
    type = record.value("type").toUInt();
    local_id = record.value("local_id").toString();
    remote_id = record.value("remote_id").toString();
    amount = record.value("amount").toDouble();
    amount_left = record.value("amount_left").toDouble();
    rate = record.value("rate").toDouble();
    price = record.value("price").toDouble();
    exchange_id = SwiftLib::getAssets()->getMarketExchangeId( record.value("pair_id").toUInt() );
    market_id = record.value("pair_id").toUInt();
    status = record.value("status").toUInt();
    fee = record.value("fee").toDouble();
    base_currency_id = 0;
    price_currency_id =0;
    arb_window = record.value("arb_window").toUInt();
}

bool OrderParams::isValid() const {
    return type < 3 && market_id > 0 && rate > 0 && amount > 0;
}

QJsonObject OrderParams::toJson() {
    QJsonObject j_obj;
    j_obj["remote_id"] = remote_id;
    j_obj["local_id"] = local_id != "" ? local_id : createLocalId();
    j_obj["market_id"] = QString::number( market_id );
    j_obj["exchange_id"] = QString::number( exchange_id );
    j_obj["rate"] = QString::number( rate, 'f', 8 );
    j_obj["amount"] = QString::number( amount, 'f', 8 );
    j_obj["amount_left"] = QString::number( amount_left, 'f', 8 );
    j_obj["price"] = QString::number( price, 'f', 8 );
    j_obj["fee"] = QString::number( fee, 'f', 8 );
    j_obj["type"] = type == 0 ? "sell" : type == 1 ? "buy" : "";
    j_obj["status"] = QString::number( status );
    j_obj["arb_window"] = QString::number( arb_window );
    return j_obj;
}

QString OrderParams::createLocalId() {
    if ( SwiftLib::getAssets()->getExchangeName( exchange_id ) == "kraken" ) {
        return QString::number( QDateTime::currentSecsSinceEpoch() );
    }
    return QCryptographicHash::hash( preHash().toUtf8(), QCryptographicHash::Sha1 ).toHex().left(34);
}

QString OrderParams::preHash() const {
    QStringList ret({
                        QString::number( exchange_id ),
                        QString::number( market_id ),
                        QString::number( amount, 'f', 8 ),
                        QString::number( rate, 'f', 8 ),
                        QString::number( fee, 'f', 8 ),
                        QString::number( price, 'f', 8 ),
                        QString::number( base_currency_id ),
                        QString::number( price_currency_id ),
                        QString::number( arb_window ),
                        type == 0 ? "sell" : "buy",
                        QString::number( QDateTime::currentMSecsSinceEpoch() )
                    });
    return ret.join(":#");
}
