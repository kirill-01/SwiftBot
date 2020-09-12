#ifndef ASSETSSTORAGE_H
#define ASSETSSTORAGE_H

#include <QObject>
#include <QSharedDataPointer>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlError>

class AssetsStorageData;

class AssetsStorage : public QObject
{
    Q_OBJECT
public:
    explicit AssetsStorage(QObject *parent = nullptr);
    AssetsStorage(const AssetsStorage &);
    AssetsStorage &operator=(const AssetsStorage &);
    ~AssetsStorage();


    QString getCoinName( const quint32& coin_id ) const;
    QString getMarketName(const quint32 &pair_id) const;
    quint32 getMarketBaseCurrency(const quint32& pair_id ) const;
    quint32 getMarketPriceCurrency(const quint32& pair_id ) const;
    quint32 getMarketAmountPrecision( const quint32& pair_id ) const;
    quint32 getMarketPricePrecision( const quint32& pair_id ) const;
    QString getExchangeName( const quint32& exchange_id ) const;
    QList<quint32> getExchangePairs( const quint32& exchange_id ) const;
    quint32 getExchangeId( const QString& exchange_name ) const;
    quint32 getCurrencyExchangeId(const quint32& currency_id ) const;
    QString getCurrencyExchangeName(const quint32& currency_id ) const;
    QString getCurrencyName( const quint32& currency_id  ) const;
    quint32 getCurrencyIdByName( const QString& currency_name, const quint32& exchange_id );
    quint32 getCurrencyCoin( const quint32& currency_id ) const;
    quint32 getTokenCurrencyIdByCoinExchange( const quint32& coin_id, const quint32& exchange_id ) const;

    QString getMarketExchangeName(const quint32& pair_id ) const;
    quint32 getMarketExchangeId(const quint32& pair_id, const bool& onlyvalid = true ) const;
    quint32 getMarketIdByName( const QString& market_name, const quint32& exchange_id );
    quint32 getMarketArbitragePairId( const quint32& pair_id ) const;
    QHash<quint32, QList<quint32>> getArbitragePairs() const;
    QStringList getCurrentSellOrdersHashes() const;
    QStringList getCurrentBuyOrdersHashes() const;

    QString getCurrencyBlockchain( const quint32& currency_id ) const;

    QString getAddressExchange( const QString& address ) const;

    QJsonObject getAddressInfo( const QString& address ) const;

    QList<quint32> getAllCurrencies( ) const;
    QList<quint32> getAllExchangeCurrencies( const QString& exchange_name ) const;
    QStringList getExchangesNames() const;
    double getBalance( const quint32& currency ) const;
    QHash<quint32,double> getBalances() const;

    bool isToken( const quint32& currency_id ) const;

    void setBalance( const QJsonObject& j_balance );

    QList<quint32> getCoinRelatedCurrencies( const quint32& currency_id ) const;
    bool isSeparatedApi( const quint32& exchange_id ) const;

    void setRequested( const QHash<QString, QHash<quint32, double>>& r );
    QHash<QString, QHash<quint32, double>> getRequested() const;
    QHash<quint32,QHash<QString,QString>> getAllAddresses() const;
    QString getDepositAddress( const quint32& currency_id, const QString& token_type="BTC" ) const;
    void setBalances( const QHash<quint32, double>& dat );
    quint32 getPrimaryMarket( const quint32& exchange_id ) const;
    double getExchangeDailyWithdrawLimit( const quint32& exchange_id ) const;
    double calculatateTransferPrice( const quint32& from_currency, const quint32& to_currency, const double& amount );
    QList<quint32> getExchangeCurrencies( const quint32& exchange_id ) const;
    QStringList getExchangeCurrenciesNames( const quint32& exchange_id ) const;
    QMap<QString,quint32> getExchangeOptions( const quint32& exchange_id ) const;

    double getExchangeTradeFee( const quint32& eid ) const;
    double getCurrencyWithdrawFee( const quint32& cid ) const;

    bool isExchangeHasApiFeeWithdraw( const quint32& eid ) const;

    bool isExchangeHasApiFeeTrade( const quint32& eid ) const;
    QString getArbitragePairName( const quint32& apid ) const;
    QString getArbitragePairBaseCurrencyName( const quint32& apid ) const;
    QString getArbitragePairMarketCurrencyName( const quint32& apid ) const;
signals:

public slots:
    void loadAssets();
private:
    QSharedDataPointer<AssetsStorageData> data;
};

#endif // ASSETSSTORAGE_H
