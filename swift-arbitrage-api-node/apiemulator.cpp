#include "apiemulator.h"

ApiEmulator::ApiEmulator(const QString& exchname, QObject *parent) : SwiftApiClient(parent), exchnm( exchname ), assets( SwiftLib::getAssets() )
{
 exchid = assets->getExchangeId( exchname );
 connect( this, &ApiEmulator::resultParsed, this, &SwiftApiClient::onApiResponseParsed );
}


QString ApiEmulator::getExchangeName() const
{
    return exchnm;
}

QString ApiEmulator::getApiVersionString()
{
    return "EMULATION";
}

void ApiEmulator::getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::getMarkets(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::orderPlace(const QJsonObject &j_params, const quint64 &async_uuid)
{
     const double starting_btc = SwiftLib::getSettings()->value("EMULATE_BTC_START", 2 ).toDouble();
     const double starting_usd = SwiftLib::getSettings()->value("EMULATE_USD_START", 15000 ).toDouble();
     const double sell_available = starting_btc - _balances_in_orders_minus.value(assets->getMarketBaseCurrency( j_params.value("market_id").toString().toUInt() ),0 ) + _balances_in_orders_plus.value(assets->getMarketBaseCurrency( j_params.value("market_id").toString().toUInt() ), 0);
    const double buy_available = starting_usd - _balances_in_orders_minus.value(assets->getMarketPriceCurrency( j_params.value("market_id").toString().toUInt() ),0 ) + _balances_in_orders_plus.value(assets->getMarketPriceCurrency( j_params.value("market_id").toString().toUInt() ), 0);
    QJsonObject j_order( j_params );
    j_order["remote_id"] = getExchangeName()+QString::number( QDateTime::currentMSecsSinceEpoch() )+j_params.value("type").toString();
    const quint32 basecurr =  assets->getMarketBaseCurrency( j_params.value("market_id").toString().toUInt() );
    const quint32 marketcurr = assets->getMarketPriceCurrency( j_params.value("market_id").toString().toUInt() );
    const QString type = j_params.value("type").toString();
    j_order["status"] = "2";
    j_order["base_currency_id"] = QString::number( basecurr );
    j_order["market_currency_id"] = QString::number( marketcurr );
    j_order["amount_left"] = "0";
    j_order["success"] = true;
    j_order["async_uuid"] = QString::number( async_uuid );
    if (type == "sell" ) {
        if (  sell_available - j_params.value("amount").toString().toDouble() > 0 ) {
            _balances_in_orders_minus[ assets->getMarketBaseCurrency( j_params.value("market_id").toString().toUInt() )] += j_params.value("amount").toString().toDouble();
            _balances_in_orders_plus[ assets->getMarketPriceCurrency( j_params.value("market_id").toString().toUInt() )] += j_params.value("price").toString().toDouble();
        } else {
            j_order["result"] = false;
            j_order["status"] = "4";
            j_order["amount_left"] = j_order["amount"];
        }
    } else if(type== "buy" ) {
        if (  buy_available - j_params.value("price").toString().toDouble() > 0 ) {
            _balances_in_orders_minus[ assets->getMarketPriceCurrency( j_params.value("market_id").toString().toUInt() )] += j_params.value("price").toString().toDouble();
            _balances_in_orders_plus[ assets->getMarketBaseCurrency( j_params.value("market_id").toString().toUInt() )] += j_params.value("amount").toString().toDouble();
        } else {
            j_order["result"] = false;
            j_order["status"] = "4";
            j_order["amount_left"] = j_order["amount"];
        }
    }
     _placed_orders.push_back( j_order );
     emit resultParsed( async_uuid, j_order );
}

void ApiEmulator::orderCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::orderGet(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::orderReplace(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::withdrawList(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid)
{
    const quint32 from_currency_id( j_params.value("currency_id").toString().toUInt() );
    const quint32 coin_id = assets->getCurrencyCoin( from_currency_id );
    double current_balance =0;
    if ( coin_id == 7 ) {
        const double starting_balance = SwiftLib::getSettings()->value("EMULATE_USD_START", 15000 ).toDouble();
        current_balance = starting_balance - _balances_in_orders_minus.value( from_currency_id, 0 ) + _balances_in_orders_plus.value( from_currency_id, 0 );
    } else if ( coin_id == 1 ) {
        const double starting_balance = SwiftLib::getSettings()->value("EMULATE_USD_START", 15000 ).toDouble();
        current_balance = starting_balance - _balances_in_orders_minus.value( from_currency_id, 0 ) + _balances_in_orders_plus.value( from_currency_id, 0 );
    } else {
        return;
    }

    const double transfer_amount( j_params.value("amount").toString().toDouble() );
    if( current_balance < transfer_amount ) {
        return;
    }
    QSqlQuery q( SwiftLib::createQuery("apiemulator") );
    quint32 curid = 0;
    if ( q.exec("SELECT currency_id FROM currency_addresses WHERE address='"+j_params.value("address").toString()+"'") ) {
        if ( q.next() ) {
           curid = q.value("currency_id").toUInt();
        }
    }
    q.prepare("INSERT INTO fake_transfers (from_exchange_id,from_currency_id,amount,to_exchange_id,to_currency_id,complete_at,done) VALUES (:from_exchange_id,:from_currency_id,:amount,:to_exchange_id,:to_currency_id,:complete_at,:done)");
    q.bindValue(":to_exchange_id", assets->getCurrencyExchangeId( curid ) );
    q.bindValue(":to_currency_id", curid );
    q.bindValue(":amount", transfer_amount);
    q.bindValue(":done", 0 );
    q.bindValue(":complete_at", QDateTime::currentDateTime().addSecs( 60 * 30 ) );
    q.bindValue(":from_exchange_id", getExchangeId() );
    q.bindValue(":from_currency_id", from_currency_id );
    if ( !q.exec() ) {
        qWarning() << q.lastError() << q.executedQuery();
    } else {
       _balances_in_orders_minus[  j_params.value("currency_id").toString().toUInt()  ] +=  j_params.value("amount").toString().toDouble();
       q.prepare("INSERT INTO `withdraws` \
       (`local_id`, \
       `remote_id`, \
       `currency_id`, \
       `amount`, \
       `address`, \
       `created_at`, \
       `txhash`, \
       `status`, \
       `fee`) \
       VALUES \
       (:local_id, \
       :remote_id, \
       :currency_id, \
       :amount, \
       :address, \
       :created_at, \
       :txhash, \
       :status, \
       :fee) ON DUPLICATE KEY UPDATE status=VALUES(status),txhash=VALUES(txhash)");

           q.bindValue(":local_id", getExchangeName()+QString::number( QDateTime::currentMSecsSinceEpoch() )+"withdraw" );
           q.bindValue(":remote_id", getExchangeName()+QString::number( QDateTime::currentMSecsSinceEpoch() )+"withdraw_rem" );
           q.bindValue(":currency_id", from_currency_id );
           q.bindValue(":amount", transfer_amount );
           q.bindValue(":address", j_params.value("address").toString("#") );
           q.bindValue(":created_at", QDateTime::currentDateTime() );
           q.bindValue(":txhash","###" );
           q.bindValue(":status", "pending" );
           q.bindValue(":fee", assets->calculatateTransferPrice( from_currency_id, curid, transfer_amount ) );
           if ( !q.exec() ) {
               qWarning() << q.lastError().text();
           }
       q.finish();

       q.prepare("INSERT INTO `deposits` \
       (`local_id`, \
       `remote_id`, \
       `currency_id`, \
       `amount`, \
       `address`, \
       `created_at`, \
       `txhash`, \
       `status`, \
       `fee`) \
       VALUES \
       (:local_id, \
       :remote_id, \
       :currency_id, \
       :amount, \
       :address, \
       :created_at, \
       :txhash, \
       :status, \
       :fee) ON DUPLICATE KEY UPDATE status=VALUES(status),txhash=VALUES(txhash)");

           q.bindValue(":local_id", getExchangeName()+QString::number( QDateTime::currentMSecsSinceEpoch() )+"deposit" );
           q.bindValue(":remote_id", getExchangeName()+QString::number( QDateTime::currentMSecsSinceEpoch() )+"deposit_rem" );
           q.bindValue(":currency_id", curid );
           q.bindValue(":amount", transfer_amount );
           q.bindValue(":address", j_params.value("address").toString("#") );
           q.bindValue(":created_at", QDateTime::currentDateTime() );
           q.bindValue(":txhash","###" );
           q.bindValue(":status", "pending" );
           q.bindValue(":fee", assets->calculatateTransferPrice( from_currency_id, curid, transfer_amount ) );
           if ( !q.exec() ) {
               qWarning() << q.lastError().text();
           }
       q.finish();
    }

    QJsonObject ret( j_params );
    ret["success"] = true;
    ret["async_uuid"] = QString::number( async_uuid );
    ret["remote_id"] = QString( QCryptographicHash::hash( QJsonDocument( j_params ).toJson( QJsonDocument::Compact ), QCryptographicHash::Md5).toHex());
    registerAsyncResult( async_uuid, ret );
}

void ApiEmulator::withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;
    ret["success"] = true;
    ret["async_uuid"] = QString::number( async_uuid );

    QJsonObject jo;
    jo["amount"] = QString::number( 0, 'f', 8 );
    jo["taker"] = QString::number( SwiftLib::getExchangeFee( getExchangeId() ), 'f', 8 );
    ret["fees"] = jo;
    registerAsyncResult( async_uuid, ret );
}

void ApiEmulator::getBalances(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QList<quint32> _currs( assets->getAllExchangeCurrencies( getExchangeName() ) );
    QJsonArray j_ret;
    for( auto it = _currs.begin(); it != _currs.end(); it++ ) {
        QJsonObject j_i;
        if ( ( exchid == 1 && *it == 3 ) ||  ( exchid == 7 && *it == 23 ) ) {

        } else {
            j_i["currency_id"] = QString::number( *it );
            if ( assets->getCurrencyCoin( *it ) == 1 ) {
                const double starting_btc = SwiftLib::getSettings()->value("EMULATE_BTC_START", 2 ).toDouble();

                j_i["total"] = QString::number( starting_btc - _balances_in_orders_minus.value(*it,0 ) + _balances_in_orders_plus.value(*it, 0), 'f', 8 );
                j_i["available"] = QString::number( starting_btc - _balances_in_orders_minus.value(*it,0 ) + _balances_in_orders_plus.value(*it, 0), 'f', 8 );
                j_i["orders"] = QString::number( 0, 'f', 8 );
            } else if ( assets->getCurrencyCoin( *it ) == 7 ) {
                const double starting_usd = SwiftLib::getSettings()->value("EMULATE_USD_START", 15000 ).toDouble();
                j_i["total"] = QString::number( starting_usd - _balances_in_orders_minus.value(*it,0 ) + _balances_in_orders_plus.value(*it, 0), 'f', 8 );
                j_i["available"] = QString::number( starting_usd - _balances_in_orders_minus.value(*it,0 ) + _balances_in_orders_plus.value(*it, 0), 'f', 8 );
                j_i["orders"] = QString::number( 0, 'f', 8 );
            }
            j_ret.push_back( j_i );
        }


    }
    QJsonObject ret;
    ret["success"] = true;
    ret["exchange_id"] = QString::number( getExchangeId() );
    ret["async_uuid"] = QString::number( async_uuid );
    ret["balances"] = j_ret;
    emit resultParsed( async_uuid, ret );
}

void ApiEmulator::getDeposits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QJsonObject ret;

    QSqlQuery q( SwiftLib::createQuery("apiemulator") );
    q.prepare("SELECT * FROM fake_transfers WHERE to_exchange_id=:eid AND done=0 AND complete_at<now()");
    q.bindValue(":eid", getExchangeId() );
    if ( q.exec() ) {
        while ( q.next() ) {
            const quint32 currency_id = q.value("to_currency_id").toUInt();
            const double amount = q.value("amount").toDouble();
            _balances_in_orders_plus[currency_id] += amount;
            QSqlQuery qq("UPDATE withdraws SET status='done' WHERE currency_id="+QString::number( q.value("from_currency_id").toUInt() ) );
            qq.exec();
            qq.exec( "UPDATE deposits SET status='done' WHERE currency_id="+QString::number( q.value("to_currency_id").toUInt() )  );
        }
    }
    q.exec("UPDATE fake_transfers SET done=1 WHERE to_exchange_id="+QString::number(getExchangeId())+" AND done=0 AND complete_at<=now()");
    q.finish();
    ret["success"] = true;
    ret["exchange_id"] = QString::number( getExchangeId() );
    ret["async_uuid"] = QString::number( async_uuid );
    ret["deposits"] = QJsonArray();
    emit resultParsed( async_uuid, ret );
}

void ApiEmulator::getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QMutexLocker locker(&m);
    QJsonObject j_res;
    j_res["success"] = true;
    j_res["async_uuid"] = QString::number( async_uuid );
    j_res["orders"] = _placed_orders;
    QTimer::singleShot( 500, [=](){
        emit resultParsed( async_uuid, j_res );
    });
}

void ApiEmulator::tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    QMutexLocker locker(&m);
    QJsonObject j_res;
    j_res["success"] = true;
    j_res["async_uuid"] = QString::number( async_uuid );
    j_res["orders"] = _open_orders;
    QTimer::singleShot( 500, [=](){
        emit resultParsed( async_uuid, j_res );
    });
}

void ApiEmulator::tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}

void ApiEmulator::tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid)
{
    Q_UNUSED(j_params)
    Q_UNUSED(async_uuid)
}
