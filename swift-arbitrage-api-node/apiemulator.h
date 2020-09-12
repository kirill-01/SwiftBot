#ifndef APIEMULATOR_H
#define APIEMULATOR_H

#include <QObject>
#include "wamp.h"
#include "swiftlib.h"
#include "swiftapiclient.h"

class ApiEmulator : public SwiftApiClient
{
    Q_OBJECT
public:
    explicit ApiEmulator(const QString& exchname, QObject *parent = nullptr);
    Wamp::Session * session;
    QString exchnm;
    quint32 exchid;
    AssetsStorage * assets;
signals:

    void resultParsed( const quint64& uuid, const QJsonObject& j_result );
public slots:

public:
    QMutex m;
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
    QJsonArray _placed_orders;
    QJsonArray _open_orders;

    QHash<quint32,double> _balances_in_orders_minus;
    QHash<quint32,double> _balances_in_orders_plus;
};

#endif // APIEMULATOR_H
