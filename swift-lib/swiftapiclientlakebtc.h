#ifndef SWIFTAPICLIENTLAKEBTC_H
#define SWIFTAPICLIENTLAKEBTC_H

#include <QObject>
#include "swiftapiclient.h"
#include "orderparams.h"

class SwiftApiClientLakeBTC : public SwiftApiClient
{
    Q_OBJECT
public:
    explicit SwiftApiClientLakeBTC(QObject* parent = nullptr);


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
    const QString url_path = "https://api.lakebtc.com/api_v2";

    bool paramValidate(const QJsonObject& j_params, const QStringList& mandatoryParams, bool  isOrderCheck);
    QString buildGetParamStr(const QJsonObject& j_params, const QStringList& params) const;
    QString buildPostParamStr(const QJsonObject& j_params, const QStringList& params) const;
    QByteArray buildSignature(const QUrlQuery& body) const;
    QUrlQuery buildBody(qint64 nonce, const QString& method, const QString& paramsString) const;
    void authPostRequest(const QUrl& url, const QString& method, const QString& paramsString, const quint64 &async_uuid);
    void publicGetRequest(const QUrl& url, const quint64 &async_uuid);
};

#endif // SWIFTAPICLIENTLAKEBTC_H
