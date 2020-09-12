#ifndef SWIFTAPIPARSERKUCOIN_H
#define SWIFTAPIPARSERKUCOIN_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserKuCoin : public SwiftApiParser
{
    Q_OBJECT
public:
    SwiftApiParserKuCoin( QObject * parent = nullptr ) : SwiftApiParser(parent) {

    }

    // SwiftApiParser interface
public:

    QString getExchangeName() const override;

public slots:

    void parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
private:

};

#endif // SWIFTAPIPARSERKUCOIN_H
