#ifndef SWIFTAPIPARSEROKCOIN_H
#define SWIFTAPIPARSEROKCOIN_H

#include <QObject>
#include "swiftapiparser.h"
#include <functional>

class SwiftApiParserOkcoin : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserOkcoin(QObject *parent = nullptr);

    // SwiftApiParser interface

    QString getExchangeName() const override;

public slots:
    void parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;

private:
    QString orderStatusParse(int status) const;
    QString withdrawStatusParse(int status) const;
    QString depositStatusParse(int status) const;
    QString depositTypeParse(int status) const;
    QString fromISOToStr(const QString& time_iso) const;
};

#endif // SWIFTAPIPARSEROKCOIN_H
