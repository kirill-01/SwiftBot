#ifndef SWIFTAPICLIENTHUOBI_H
#define SWIFTAPICLIENTHUOBI_H

#include <QObject>
#include <swiftapiclient.h>

class SwiftApiClientHuobi : public SwiftApiClient
{
    Q_OBJECT
public:
    SwiftApiClientHuobi(QObject* parent = nullptr);

    // SwiftApiClient interface
public:
    QString getExchangeName() const override;
    QString getApiVersionString() override;
    void getCurrencies(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getMarkets(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getOrderbooks(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderPlace(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderCancel(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderGet(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void orderReplace(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawGetLimits(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawList(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawHistory(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawCancel(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawCreate(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawInner(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void withdrawGetFee(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getBalances(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getDeposits(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void getDepositAddress(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeHistory(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeOpenOrders(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeGetFee(const QJsonObject &j_params, const quint64 &async_uuid) override;
    void tradeGetLimits(const QJsonObject &j_params, const quint64 &async_uuid) override;


private:
    QString rest_url = "https://api.huobi.pro";

    QByteArray buildSignature(const QByteArray &method, const QByteArray &api_path, const QByteArray &ordered_params) const;


    void buildGetRequest_withSignature(const QString &api_path, QMap<QString, QString> &params, const quint64 &uuid);
    void buildPostRequest_withSignature(const QString& api_path, QUrlQuery &body, const quint64 &uuid);

    // SwiftApiClient interface
public:
    QString parseStatus( const QString& stat );
    void customParser(const SwiftApiClient::AsyncMethods &method, const QJsonObject &j_result)  override;
    void customMethod(const QJsonObject &j_params, const quint64 &async_uuid) override;
};

#endif // SWIFTAPICLIENTHUOBI_H
