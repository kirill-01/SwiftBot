#include "assetsstorage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDebug>
#include "swiftlib.h"
class AssetsStorageData : public QSharedData
{
public:
    QHash<quint32,QList<quint32>> _arbitrage_pair_markets;

    QHash<quint32,QString> _exchange_name;
    QHash<quint32,QString> _market_name;
    QHash<quint32,QString> _currency_name;

    QHash<quint32,QString> _currency_token_type;
    QHash<quint32,quint32> _currency_exchange;
    QHash<quint32,quint32> _market_exchange;
    QHash<quint32,quint32> _all_market_exchange;
    QHash<quint32,quint32> _market_arbitrage_pair;
    QHash<quint32,QHash<QString,quint32>> _exchange_currencies;

    QHash<quint32, quint32> _currency_coin;
    QHash<quint32, QList<quint32>> _coin_currencies;

    QHash<quint32,QPair<quint32,quint32>> _market_currencies;

    QHash<quint32,QHash<QString,quint32>> _exchange_markets;

    QHash<quint32, QHash<QString,QString>> _currency_addresses;
    QHash<quint32, double> _currency_withdraw_fee;
    QStringList _orders_sell_hashes_local;
    QStringList _orders_buy_hashes_local;
    QHash<QString, QHash<quint32, double>> _requested;
    QHash<quint32, bool> _is_token_currency;
    QHash<quint32, QString> _deposit_addresses;

    QHash<quint32, double> _balances;
    QHash<quint32, double> _balances_available;
    QHash<quint32, QMap<QString,quint32>> _exchange_options;
    QHash<quint32,double> _exchange_daily_withdraw_limits;
    QMap<quint32,QString> _coinsname;
    QHash<quint32, bool> _exchange_api_separated;
    QMap<quint32,double> _exchange_trade_fee;
    QHash<quint32, QString> _arbitrage_pairs_names;
    QHash<quint32, QString> _arbitrage_pairs_base_coin_name;
    QHash<quint32, QString> _arbitrage_pairs_market_coin_name;
    QHash<quint32,bool> _exchange_api_fee_trade;
    QHash<quint32,bool> _exchange_api_fee_withdraw;

    QHash<quint32,quint32> _pair_amount_precision;
    QHash<quint32,quint32> _pair_price_precision;
};

AssetsStorage::AssetsStorage(QObject *parent) : QObject(parent), data(new AssetsStorageData)
{

}

AssetsStorage::AssetsStorage(const AssetsStorage &rhs) : QObject( rhs.parent() ), data(rhs.data)
{

}

AssetsStorage &AssetsStorage::operator=(const AssetsStorage &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

AssetsStorage::~AssetsStorage()
{

}

QString AssetsStorage::getCoinName(const quint32 &coin_id) const {
    return data->_coinsname.value( coin_id, "???");
}

QString AssetsStorage::getMarketName(const quint32 &pair_id) const {
    return data->_market_name.value( pair_id );
}


quint32 AssetsStorage::getMarketBaseCurrency(const quint32 &pair_id) const {
    return data->_market_currencies[pair_id].first;
}

quint32 AssetsStorage::getMarketPriceCurrency(const quint32 &pair_id) const {
    return data->_market_currencies[pair_id].second;
}

quint32 AssetsStorage::getMarketAmountPrecision(const quint32 &pair_id) const
{
    return data->_pair_amount_precision.value( pair_id, 8 );
}

quint32 AssetsStorage::getMarketPricePrecision(const quint32 &pair_id) const
{
    return data->_pair_price_precision.value( pair_id, 8 );
}


QString AssetsStorage::getExchangeName(const quint32 &exchange_id) const {
    return data->_exchange_name[ exchange_id ];
}

QList<quint32> AssetsStorage::getExchangePairs(const quint32 &exchange_id) const {
    return data->_exchange_markets[exchange_id].values();
}

quint32 AssetsStorage::getExchangeId(const QString &exchange_name) const {
    if ( data->_exchange_name.values().contains( exchange_name ) ) {
        return data->_exchange_name.key( exchange_name );
    }
    if ( data->_exchange_name.values().contains( exchange_name.toLower() ) ) {
        return data->_exchange_name.key( exchange_name.toLower() );
    }
    QStringList names( data->_exchange_name.values() );
    for( auto it = names.begin(); it != names.end(); it++ ) {
        if ( it->toLower() == exchange_name.toLower() ) {
            return data->_exchange_name.key( *it );
        }
    }
    return 0;
}

quint32 AssetsStorage::getCurrencyExchangeId(const quint32 &currency_id) const {
     return data->_currency_exchange[currency_id ];
}

QString AssetsStorage::getCurrencyExchangeName(const quint32 &currency_id) const {
    return data->_exchange_name[ data->_currency_exchange[currency_id ] ];
}

QString AssetsStorage::getCurrencyName(const quint32 &currency_id) const {
    return data->_currency_name.value( currency_id, "???" );
}

quint32 AssetsStorage::getCurrencyIdByName(const QString &currency_name, const quint32 &exchange_id) {
    QString cname( currency_name );
    if ( data->_exchange_currencies[ exchange_id ].contains( cname ) ) {
          return data->_exchange_currencies[ exchange_id ].value( cname, 0 );
    } else if ( data->_exchange_currencies[ exchange_id ].contains( cname.toUpper() ) ) {
        return data->_exchange_currencies[ exchange_id ].value( cname.toUpper(), 0 );
    } else if ( data->_exchange_currencies[ exchange_id ].contains( cname.toLower() ) ) {
        return data->_exchange_currencies[ exchange_id ].value( cname.toLower(), 0 );
    } else if ( data->_exchange_currencies[ exchange_id ].contains( cname.toUpper().replace("BTC","XBT") ) ) {
        return data->_exchange_currencies[ exchange_id ].value( cname.toUpper().replace("BTC","XBT"), 0 );
    } else {
        return 0;
    }

}

quint32 AssetsStorage::getCurrencyCoin(const quint32 &currency_id) const {
    return data->_currency_coin.value(currency_id);
}

quint32 AssetsStorage::getTokenCurrencyIdByCoinExchange(const quint32 &coin_id, const quint32 &exchange_id) const {
    const QHash<QString,quint32> _currencies( data->_exchange_currencies[exchange_id] );
    for ( auto it = _currencies.begin(); it != _currencies.end(); it++ ) {
        if ( getCurrencyCoin( it.value() ) == coin_id ) {
            if ( ( coin_id == 7 && isToken( it.value() ) ) || coin_id != 7 ) {
                return it.value();
            }
        }
    }
    return 0;
}

QString AssetsStorage::getMarketExchangeName(const quint32 &pair_id) const {
    return data->_exchange_name[ data->_market_exchange[pair_id ] ];
}

quint32 AssetsStorage::getMarketExchangeId(const quint32 &pair_id, const bool& onlyvalid) const {
    if ( onlyvalid ) {
        return data->_market_exchange.value( pair_id, 0 );
    } else {
        return data->_all_market_exchange.value( pair_id, 0 );
    }
}

quint32 AssetsStorage::getMarketIdByName(const QString &market_name, const quint32 &exchange_id)
{
    if ( data->_exchange_markets[exchange_id].contains( market_name ) ) {
        return data->_exchange_markets[ exchange_id ].value( market_name, 0 );
    } else if ( data->_exchange_markets[exchange_id].contains( market_name.toUpper() ) )  {
        return data->_exchange_markets[ exchange_id ].value( market_name.toUpper(), 0 );
    } else if ( data->_exchange_markets[exchange_id].contains( market_name.toLower() ) )  {
        return data->_exchange_markets[ exchange_id ].value( market_name.toLower(), 0 );
    } else {
        return 0;
    }

}

quint32 AssetsStorage::getMarketArbitragePairId(const quint32 &pair_id) const {
    const QHash<quint32, QList<quint32>> _p( getArbitragePairs() );
    for( auto it = _p.begin(); it != _p.end(); it++ ) {
        if( it.value().contains( pair_id ) ) {
            return it.key();
        }
    }
    return 0;
}

QHash<quint32, QList<quint32> > AssetsStorage::getArbitragePairs() const {
    return data->_arbitrage_pair_markets;
}

QStringList AssetsStorage::getCurrentSellOrdersHashes() const
{
    return data->_orders_sell_hashes_local;
}

QStringList AssetsStorage::getCurrentBuyOrdersHashes() const
{
    return data->_orders_buy_hashes_local;
}

QString AssetsStorage::getCurrencyBlockchain(const quint32 &currency_id) const {
    return data->_currency_token_type.value( currency_id, "" );
}

QString AssetsStorage::getAddressExchange(const QString &address) const {
    for( auto it = data->_currency_addresses.begin(); it != data->_currency_addresses.end(); it++ ) {
        const quint32 currency_id = it.key();
        const QHash<QString, QString> _addresses( data->_currency_addresses.value( currency_id ));
        for( auto i = _addresses.begin(); i != _addresses.end(); i++ ) {
            if ( i.value() == address ) {
                return getCurrencyExchangeName( currency_id );
            }
        }
    }
    return "UNKNOWN";
}

QJsonObject AssetsStorage::getAddressInfo(const QString &address) const {
    QJsonObject j_ret;
    j_ret["address"] = address;
    j_ret["exchange_name"] = getAddressExchange( address );
    j_ret["url"] = "https://blockchair.com/ru/search?q="+address;
    return j_ret;
}

QList<quint32> AssetsStorage::getAllCurrencies() const {
    return data->_currency_name.keys();
}

QList<quint32> AssetsStorage::getAllExchangeCurrencies(const QString &exchange_name) const
{
    return data->_exchange_currencies.value( getExchangeId( exchange_name ) ).values();
}

QStringList AssetsStorage::getExchangesNames() const {
    return data->_exchange_name.values();
}

double AssetsStorage::getBalance(const quint32 &currency) const {

    return data->_balances.value( currency, 0 );
}

QHash<quint32, double> AssetsStorage::getBalances() const {
    return data->_balances;
}

bool AssetsStorage::isToken(const quint32 &currency_id) const {
    return data->_is_token_currency[currency_id];
}

void AssetsStorage::setBalance(const QJsonObject &j_balance) {
     qWarning() << j_balance << "setting assets";
    data->_balances[ j_balance.value("currency_id").toString().toUInt() ] = j_balance.value("total").toString().toDouble();
    data->_balances_available[ j_balance.value("currency_id").toString().toUInt() ] = j_balance.value("available").toString().toDouble();
}

QList<quint32> AssetsStorage::getCoinRelatedCurrencies(const quint32 &currency_id) const {
    QList<quint32> _ret;
    const quint32 coin_id = data->_currency_coin[currency_id];
    for( auto it = data->_coin_currencies[coin_id].begin(); it != data->_coin_currencies[coin_id].end(); it++ ) {
        if ( *it != currency_id ) {
            _ret.push_back( *it );
        }
    }
    return _ret;
}

bool AssetsStorage::isSeparatedApi(const quint32 &exchange_id) const {
    return data->_exchange_api_separated.value( exchange_id, false );
}

void AssetsStorage::setRequested(const QHash<QString, QHash<quint32, double> > &r) {
    data->_requested = r;
}

QHash<QString, QHash<quint32, double> > AssetsStorage::getRequested() const {
    return data->_requested;
}

QHash<quint32, QHash<QString, QString> > AssetsStorage::getAllAddresses() const {
    return data->_currency_addresses;
}

QString AssetsStorage::getDepositAddress(const quint32 &currency_id, const QString& token_type) const {
    if ( data->_currency_addresses.contains( currency_id ) ) {
        return data->_currency_addresses[currency_id].value( token_type,"#" );
    } return "#";
}


void AssetsStorage::loadAssets() {
    data->_coinsname[1] = "XBT";
    data->_coinsname[2] = "ETH";
    data->_coinsname[7] = "USD";
    data->_coinsname[10] = "XRP";


    QSqlQuery q( SwiftLib::createQuery( SwiftLib::getUserDbName() ) );
    if ( q.exec("SELECT * FROM exchanges") ) {
        while( q.next() ) {
            const quint32 eid = q.value("id").toUInt();
            const bool is_api_separated = q.value("api_separated_pairs").toUInt() == 1;
            data->_exchange_api_separated[ eid ] = is_api_separated;
            data->_exchange_trade_fee[ eid ] = q.value("trade_fee").toDouble();
            data->_exchange_api_fee_trade[eid] = q.value("api_fee_trade").toUInt() == 1;
            data->_exchange_api_fee_withdraw[eid] = q.value("api_fee_withdraw").toUInt() == 1;
        }
    }
    if ( q.exec("SELECT * FROM arbitrage_pairs") ) {
        while( q.next() ) {
            const quint32 apid = q.value("id").toUInt();
            const QString apname = q.value("name").toString();
            data->_arbitrage_pairs_names[ apid ] = apname;
            data->_arbitrage_pairs_base_coin_name[ apid ] = data->_coinsname.value( q.value("base_coin_id").toUInt() );
            data->_arbitrage_pairs_market_coin_name[ apid ] = data->_coinsname.value( q.value("price_coin_id").toUInt() );;
        }
    }


    if ( q.exec("SELECT c.*, x.name as exchange_name FROM currencies c LEFT JOIN exchanges x ON c.exchange_id=x.id WHERE c.is_enabled=1") ) {
         while( q.next() ) {
            const quint32 _id = q.value("id").toUInt();
            const quint32 _exchange_id = q.value("exchange_id").toUInt();
            const QString _name = q.value("name").toString();
            const QString _exchange_name = q.value("exchange_name").toString();
            data->_currency_name[_id] = _name;
            data->_currency_exchange[_id] = _exchange_id;
            data->_exchange_name[_exchange_id] = _exchange_name;
            data->_currency_withdraw_fee[_id] = q.value("withdraw_fee").toDouble();
         }
    }
    if ( q.exec("SELECT m.*, e.name as exchange_name, e.is_enabled as exchvalid, m.is_enabled as marketvalid FROM pairs m LEFT JOIN exchanges e ON e.id = m.exchange_id WHERE m.is_enabled=1") ) {
        while( q.next() ) {
            const quint32 pair_id = q.value("id").toUInt();
            const QString name = q.value("name").toString();
            const quint32 exchange_id = q.value("exchange_id").toUInt();
            const quint32 arbitrage_pair = q.value("arbitrage_pair_id").toUInt();
            const quint32 base_currency = q.value("base_currency_id").toUInt();
            const quint32 market_currency = q.value("market_currency_id").toUInt();
            data->_pair_amount_precision[ pair_id ] = q.value("amount_precision").toUInt();
            data->_pair_price_precision[ pair_id ] = q.value("price_precision").toUInt();
            if ( q.value("exchvalid").toUInt() == 1 ) {
                data->_market_currencies[ pair_id ] = QPair<quint32,quint32>({ base_currency, market_currency });
                data->_exchange_markets[ q.value("exchange_id").toUInt() ].insert( q.value("name").toString(), q.value("id").toUInt() );
            } else {

            }
            if ( q.value("marketvalid").toUInt() == 1 ) {
                data->_market_exchange[ pair_id ] = exchange_id;
            } else {
                data->_all_market_exchange[ pair_id ] = exchange_id;
            }
            if( arbitrage_pair > 0 ) {
                data->_arbitrage_pair_markets[ arbitrage_pair ].push_back( pair_id );
                data->_market_arbitrage_pair[ pair_id ] = arbitrage_pair;
            }
            data->_market_name[ pair_id ] = name;




        }
    }
    if ( q.exec("SELECT * FROM currencies c LEFT JOIN exchanges e ON c.exchange_id=e.id WHERE c.is_enabled=1") ) {
        while( q.next() ) {
            const quint32 currency_id = q.value("id").toUInt();
            const QString name = q.value("name").toString();
            const quint32 coin_id = q.value("coin_id").toUInt();
            data->_currency_coin[currency_id] = coin_id;
            data->_coin_currencies[ coin_id ].push_back( currency_id );
            if ( coin_id == 1 ) {
                data->_is_token_currency[ currency_id ] = true;
            } else if ( coin_id == 7 && ( name.toUpper() == "USDT" || name.toUpper() == "USDT20" || name.toUpper() == "UST" ) ) {
                data->_is_token_currency[ currency_id ] = true;
            } else if ( coin_id == 10 ) {
                data->_is_token_currency[ currency_id ] = true;
            } else {
                data->_is_token_currency[ currency_id ] = false;
            }
            data->_exchange_currencies[ q.value("exchange_id").toUInt() ].insert( q.value("name").toString(), q.value("id").toUInt() );
        }
    }
    const QString dbname( SwiftLib::getSettings()->value("MYSQL_DBNAME").toString().trimmed() );
    if ( q.exec("SELECT * FROM `"+dbname+"`.`currency_addresses`") ) {
        while( q.next() ) {
            const quint32 currency_id = q.value("currency_id").toUInt();
            const QString address = q.value("address").toString();
            const QString token_type = q.value("token_type").toString().toUpper();
            data->_currency_addresses[currency_id].insert( token_type, address );
        }
    }
    q.finish();
}

void AssetsStorage::setBalances(const QHash<quint32, double> &dat) {
    data->_balances = dat;
}

quint32 AssetsStorage::getPrimaryMarket(const quint32 &exchange_id) const {
    const QHash<QString,quint32> _mkrs( data->_exchange_markets[exchange_id] );
    for ( auto it = _mkrs.begin(); it != _mkrs.end(); it++ ) {
        if ( getCurrencyCoin( getMarketBaseCurrency( it.value() ) ) == 1 && getCurrencyCoin( getMarketPriceCurrency( it.value() ) ) == 7 ) {
            return it.value();
        }
    }
    return 0;
}

double AssetsStorage::getExchangeDailyWithdrawLimit(const quint32 &exchange_id) const {
    return data->_exchange_daily_withdraw_limits.value( exchange_id, 0 );
}

double AssetsStorage::calculatateTransferPrice(const quint32 &from_currency, const quint32 &to_currency, const double &amount) {
    const quint32 from_exchange = getCurrencyExchangeId( from_currency ) ;

    const quint32 coin_id = getCurrencyCoin( from_currency );
    if ( coin_id == 1 ) {
        if ( from_exchange == 3 ) {
            return 0.0003;
        } else {
            return 0.0005;
        }

    } else {
        if ( isToken( from_currency ) && isToken( to_currency ) ) {
            // USDT-USDT transfer ?
            if ( coin_id == 7 ) {
                return 5.00;
            } else if (  coin_id == 10 ) {
                return 0.1;
            } else if ( coin_id == 1 ) {
                if ( from_exchange == 3 ) {
                    return 0.0003;
                } else {
                    return 0.0005;
                }
            }
        }
        else {
            double fee_ret = 5;
            if ( !isToken( from_currency ) ) {

                double order_rate = 1.025;
                if ( from_exchange == 3 ) {
                    order_rate = 1.045;
                } else if ( from_exchange == 9 ) {
                    order_rate = 1.10;
                }
                double order_price = amount * order_rate;
                const double order_out_amount = order_price - ( order_price * SwiftLib::getExchangeFee( from_exchange ) );
                fee_ret += (amount - order_out_amount);
            }
            if ( !isToken( to_currency ) ) {
                double order_rate = 1.008;
                if ( from_exchange == 3 ) {
                    order_rate = 0.95;
                }
                double order_price = amount * order_rate;
                fee_ret +=  order_price - ( order_price * SwiftLib::getExchangeFee( from_exchange ) );
            }
            return fee_ret;
        }
    }
    return 0;
}

QList<quint32> AssetsStorage::getExchangeCurrencies(const quint32 &exchange_id) const {
    return data->_exchange_currencies.value( exchange_id ).values();
}

QStringList AssetsStorage::getExchangeCurrenciesNames(const quint32 &exchange_id) const {
    return data->_exchange_currencies.value( exchange_id ).keys();
}

QMap<QString, quint32> AssetsStorage::getExchangeOptions(const quint32 &exchange_id) const {
    return data->_exchange_options.value( exchange_id );
}

double AssetsStorage::getExchangeTradeFee(const quint32 &eid) const {
    return data->_exchange_trade_fee.value( eid, 0.0018 );
}

double AssetsStorage::getCurrencyWithdrawFee(const quint32 &cid) const {
    return data->_currency_withdraw_fee.value( cid, 0.1 );
}

bool AssetsStorage::isExchangeHasApiFeeWithdraw(const quint32 &eid) const {
    return data->_exchange_api_fee_withdraw.value( eid, false );
}

bool AssetsStorage::isExchangeHasApiFeeTrade(const quint32 &eid) const {
    return data->_exchange_api_fee_trade.value( eid, false );
}

QString AssetsStorage::getArbitragePairName(const quint32 &apid) const {
    return data->_arbitrage_pairs_names.value( apid );
}

QString AssetsStorage::getArbitragePairBaseCurrencyName(const quint32 &apid) const {
    return data->_arbitrage_pairs_base_coin_name.value( apid );
}

QString AssetsStorage::getArbitragePairMarketCurrencyName(const quint32 &apid) const {
    return data->_arbitrage_pairs_market_coin_name.value( apid );
}
