#ifndef SWIFTAPICLIENTKRAKEN_H
#define SWIFTAPICLIENTKRAKEN_H

#include "swiftapiclient.h"
#include <sys/time.h>
#include <sstream>
#include <iomanip>


using namespace std;

class SwiftApiClientKraken : public SwiftApiClient
{
    Q_OBJECT
public:
    SwiftApiClientKraken(QObject* parent = nullptr);
    int getCounter() const;

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
    QNetworkReply* authPostRequest(const QUrl& url_path, QUrlQuery& body);
    inline QString sideToStr(int side) const;
    inline bool isRequreSecondPrice(const QString& ordertype) const;

    const QString url_public = "https://api.kraken.com/0/public/";
    const QString url_private = "https://api.kraken.com/0/private/";
    int counter;
};

#endif // SWIFTAPICLIENTKRAKEN_H
