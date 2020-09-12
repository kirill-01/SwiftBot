#ifndef SWIFTAPICLIENTHITBTC_H
#define SWIFTAPICLIENTHITBTC_H

#include <QObject>
#include "swiftapiclient.h"
class SwiftApiClientHitbtc : public SwiftApiClient
{
    Q_OBJECT
public:
    SwiftApiClientHitbtc( QObject * parent);


    QNetworkReply * sendRequest(const QString& url, const QString& method, const QMap<QString,QString> params);

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
            void customMethod(const QJsonObject& j_params = QJsonObject(), const quint64& async_uuid=0) override;

public slots:
            void transferToTrades();
};



#endif // SWIFTAPICLIENTHITBTC_H
