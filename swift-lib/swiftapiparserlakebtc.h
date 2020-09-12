#ifndef SWIFTAPIPARSERLAKEBTC_H
#define SWIFTAPIPARSERLAKEBTC_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserLakebtc : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserLakebtc(QObject * parent = nullptr);

    // SwiftApiParser interface
public:

    QString getExchangeName() const override;

public:
    void parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
};

#endif // SWIFTAPIPARSERLAKEBTC_H
