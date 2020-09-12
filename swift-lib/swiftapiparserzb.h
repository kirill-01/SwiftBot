#ifndef SWIFTAPIPARSERZB_H
#define SWIFTAPIPARSERZB_H

#include <QObject>
#include "swiftapiparser.h"

class SwiftApiParserZB : public SwiftApiParser
{
    Q_OBJECT
public:
    explicit SwiftApiParserZB(QObject *parent = nullptr);

    // SwiftApiParser interface

    QString getExchangeName() const override;

public slots:
    void parseResponse( const quint64& uuid, const SwiftApiClient::AsyncMethods &method, const QByteArray &data) override;

private:
    QString translateMsg( const QString& msg ) {
        if ( msg == "根据您在本站的资金综合状况，为了您的资产安全，您需要进行初级实名认证，才能继续相关资金操作。如需更大额度，可直接进行高级实名认证" ) {
            return "Verification required";
        }
        return msg;
    }
    QString orderStatus(int status) const;
    QString withdrawStatus(int status) const;
    QString depositStatus(int status) const;
};

#endif // SWIFTAPIPARSERZB_H
