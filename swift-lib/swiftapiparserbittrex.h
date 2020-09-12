#ifndef SWIFTAPIPARSERBITTREX_H
#define SWIFTAPIPARSERBITTREX_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserBittrex : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserBittrex(QObject *parent = nullptr);

signals:


    // SwiftApiParser interface
public:

    QString getExchangeName() const override;

public slots:
    void parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
};

#endif // SWIFTAPIPARSERBITTREX_H
