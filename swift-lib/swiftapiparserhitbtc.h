#ifndef SWIFTAPIPARSERHITBTC_H
#define SWIFTAPIPARSERHITBTC_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserHitbtc : public SwiftApiParser
{
    Q_OBJECT
public:
    SwiftApiParserHitbtc(QObject *parent=nullptr) : SwiftApiParser(parent)
    {

    }
    QMap<quint32,double> _account_balances_storage;


    // SwiftApiParser interface
public:

    QString getExchangeName() const override;

public slots:
    void parseResponse(const quint64 &uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;
};

#endif // SWIFTAPIPARSERHITBTC_H
