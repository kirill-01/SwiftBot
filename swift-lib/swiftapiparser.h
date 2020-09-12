#ifndef SWIFTAPIPARSER_H
#define SWIFTAPIPARSER_H

#include <QObject>
#include "swiftapiclient.h"

class SwiftApiParser : public QObject
{
    Q_OBJECT
public:
    explicit SwiftApiParser(QObject *parent = nullptr);
    quint32 getExchangeId() const {
        return SwiftLib::getAssets()->getExchangeId( getExchangeName() );
    };
    virtual QString getExchangeName() const=0;
signals:
    void resultParsed( const quint64& uuid, const QJsonObject& j_result );
public slots:
    virtual void parseResponse(  const quint64& uuid, const SwiftApiClient::AsyncMethods& method, const QByteArray& data )=0;
private:

};

#endif // SWIFTAPIPARSER_H
