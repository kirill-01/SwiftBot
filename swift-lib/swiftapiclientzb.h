#ifndef SWIFTAPICLIENTZB_H
#define SWIFTAPICLIENTZB_H

#include <QObject>
#include <swiftapiclient.h>

class SwiftApiClientZB : public SwiftApiClient
{
    Q_OBJECT
public:
    SwiftApiClientZB(QObject* parent = nullptr);

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

    void buildAuthRequest(const quint64 &uuid, const QString& method_url, const QMap<QString, QString>& params);

    const QString publicPrefix = "http://api.zb.live/data/v1";
    const QString privatePrefix = "https://trade.zb.com/api";
    bool isParamExist(const QStringList &params, const QJsonObject &j_params, QString& error);
    bool isUintValid(quint32 id, QString &error, const QString &id_name = "market_id");
    bool isPriceValid(double price, QString &error);
    bool isAmountValid(double amount, QString &error);
    bool isSideValid(const QString &side, QString &error);
    QString buildSignature(const QString &params) const;


};

#endif // SWIFTAPICLIENTZB_H
