#ifndef SWIFTAPIPARSEREXMO_H
#define SWIFTAPIPARSEREXMO_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserExmo : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserExmo(QObject *parent = nullptr) : SwiftApiParser( parent ) {

    }

    // SwiftApiParser interface
public:
    QStringList opn_orders_cache;

    QString getExchangeName() const override;

public slots:
    void parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
};

#endif // SWIFTAPIPARSEREXMO_H
