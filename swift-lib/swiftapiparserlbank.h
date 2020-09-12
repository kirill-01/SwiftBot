#ifndef SWIFTAPIPARSERLBANK_H
#define SWIFTAPIPARSERLBANK_H

#include <QObject>
#include "swiftapiparser.h"
class SwiftApiParserLbank : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserLbank(QObject *parent = nullptr) : SwiftApiParser(parent) {

    }

signals:
    // SwiftApiParser interface
public:

    QString getExchangeName() const override;

public slots:
    void parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
};

#endif // SWIFTAPIPARSERLBANK_H
