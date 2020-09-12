#ifndef SWIFTAPIPARSERIDCM_H
#define SWIFTAPIPARSERIDCM_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserIDCM : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserIDCM(QObject *parent = nullptr);

    // SwiftApiParser interface

    QString getExchangeName() const override;

public slots:
    void parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;

private:
    QString orderStatus(int status) const;
    QString withdrawStatus(int status) const;
};

#endif // SWIFTAPIPARSERIDCM_H
