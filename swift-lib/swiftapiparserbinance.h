#ifndef SWIFTAPIPARSERBINANCE_H
#define SWIFTAPIPARSERBINANCE_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserBinance : public SwiftApiParser
{
    Q_OBJECT
public:

signals:


    // SwiftApiParser interface
public:

    QString getExchangeName() const override;

public slots:
    void parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
private:
    QString parseStatus( const QString& stat );
};

#endif // SWIFTAPIPARSERBINANCE_H
