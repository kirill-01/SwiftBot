#ifndef SWIFTAPICLIENTOKCOIN_H
#define SWIFTAPICLIENTOKCOIN_H

#include "swiftapiclient.h"
#include "orderparams.h"

class SwiftApiClientOkcoin : public SwiftApiClient
{
    Q_OBJECT
public:
    explicit SwiftApiClientOkcoin(QObject* parent = nullptr);

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
    const QString url_public = "https://www.okcoin.com/api";
    const QString url_private = "https://www.okcoin.com/api";
    const QString path_account = "/account/v3/";
    const QString path_spot = "/spot/v3/";

    int counter;

    QNetworkRequest buildAuth(const QUrl &url, const QByteArray &method, const QByteArray &body = "") const;
    QNetworkReply* sendPostAuth(const QUrl &url, QUrlQuery &body);
    QNetworkReply* sendGetAuth(const QUrl& url, const QByteArray &body = "");
    inline QString sideToStr(int side) const;
};

#endif // SWIFTAPICLIENTOKCOIN_H
