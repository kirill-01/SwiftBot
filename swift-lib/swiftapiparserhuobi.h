#ifndef SWIFTAPIPARSERHUOBI_H
#define SWIFTAPIPARSERHUOBI_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserHuobi : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserHuobi(QObject *parent = nullptr);

    // SwiftApiParser interface

    QString getExchangeName() const override;

public slots:
    void parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;

private:
    QString orderStatus(const QString& status) const;
    QString withdrawStatus(const QString& status) const;
    QString depositStatus(const QString& status) const;
    QString sideValue(const QString& side) const;
    QString cropDoubleStr(const QString &s, bool onlyPos=false) const;
};

#endif // SWIFTAPIPARSERHUOBI_H
