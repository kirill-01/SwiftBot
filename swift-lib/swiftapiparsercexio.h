#ifndef SWIFTAPIPARSERCEXIO_H
#define SWIFTAPIPARSERCEXIO_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserCexio : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserCexio(QObject *parent = nullptr) : SwiftApiParser( parent ) {

    }


    // SwiftApiParser interface
public:

    QString getExchangeName() const override;

public slots:
    void parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
};

#endif // SWIFTAPIPARSERCEXIO_H
