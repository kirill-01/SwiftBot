#ifndef SWIFTAPIPARSERBITFINEX_H
#define SWIFTAPIPARSERBITFINEX_H

#include <QObject>
#include <functional>
#include "swiftapiparser.h"

class SwiftApiParserBitfinex : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserBitfinex(QObject *parent = nullptr);

    QString getExchangeName() const override;

    typedef std::function<const QJsonObject (const QJsonArray&)> ParsingArrayToObj;
    typedef std::function<bool (const QJsonArray&)> ConditionFunction;
public slots:
    void parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;

private:
    const QJsonArray unpackArray(const QJsonArray& data, const ParsingArrayToObj func) const;
    const QJsonArray unpackArray(const QJsonObject& data, const ParsingArrayToObj func) const;
    const QJsonArray unpackArrayIf(const QJsonArray& data, ConditionFunction condition, const ParsingArrayToObj func) const;
    const QJsonArray unpackArrayIf(const QJsonObject& data, ConditionFunction condition, const ParsingArrayToObj func) const;

    const QJsonObject parseBitfinexOrder(const QJsonArray& src) const;
    const QJsonObject parseBitfinexMove(const QJsonArray& src) const;
    const QJsonObject parseBitfinexWallet(const QJsonArray& src) const;
    const QJsonObject parseBitfinexDeposit(const QJsonArray& src) const;
};

#endif // SWIFTAPIPARSERBITFINEX_H
