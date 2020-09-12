#include "swiftapiparser.h"

SwiftApiParser::SwiftApiParser(QObject *parent) : QObject(parent)
{
 qRegisterMetaType<SwiftApiClient::AsyncMethods>("SwiftApiClient::AsyncMethods");
}
