/****************************************************************************
** Meta object code from reading C++ file 'swiftapiclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "swiftapiclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'swiftapiclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SwiftApiClient_t {
    QByteArrayData data[53];
    char stringdata0[711];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SwiftApiClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SwiftApiClient_t qt_meta_stringdata_SwiftApiClient = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SwiftApiClient"
QT_MOC_LITERAL(1, 15, 15), // "pubWampResponse"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "topic"
QT_MOC_LITERAL(4, 38, 5), // "j_res"
QT_MOC_LITERAL(5, 44, 4), // "uuid"
QT_MOC_LITERAL(6, 49, 7), // "pubWamp"
QT_MOC_LITERAL(7, 57, 19), // "customRequestResult"
QT_MOC_LITERAL(8, 77, 7), // "reqname"
QT_MOC_LITERAL(9, 85, 4), // "data"
QT_MOC_LITERAL(10, 90, 17), // "apiParsedResponse"
QT_MOC_LITERAL(11, 108, 28), // "SwiftApiClient::AsyncMethods"
QT_MOC_LITERAL(12, 137, 6), // "method"
QT_MOC_LITERAL(13, 144, 8), // "j_result"
QT_MOC_LITERAL(14, 153, 16), // "parseApiResponse"
QT_MOC_LITERAL(15, 170, 11), // "resetLimits"
QT_MOC_LITERAL(16, 182, 8), // "unfreeze"
QT_MOC_LITERAL(17, 191, 18), // "sendApiStateReport"
QT_MOC_LITERAL(18, 210, 14), // "onWampSession_"
QT_MOC_LITERAL(19, 225, 14), // "Wamp::Session*"
QT_MOC_LITERAL(20, 240, 8), // "session_"
QT_MOC_LITERAL(21, 249, 14), // "onNetworkReply"
QT_MOC_LITERAL(22, 264, 14), // "QNetworkReply*"
QT_MOC_LITERAL(23, 279, 5), // "reply"
QT_MOC_LITERAL(24, 285, 23), // "processAsyncQueueResult"
QT_MOC_LITERAL(25, 309, 17), // "processAsyncQueue"
QT_MOC_LITERAL(26, 327, 19), // "onApiResponseParsed"
QT_MOC_LITERAL(27, 347, 19), // "customMethodParsers"
QT_MOC_LITERAL(28, 367, 9), // "ApiErrors"
QT_MOC_LITERAL(29, 377, 17), // "HttpResponseError"
QT_MOC_LITERAL(30, 395, 12), // "AsyncMethods"
QT_MOC_LITERAL(31, 408, 14), // "TimeSyncMethod"
QT_MOC_LITERAL(32, 423, 13), // "GetCurrencies"
QT_MOC_LITERAL(33, 437, 10), // "GetMarkets"
QT_MOC_LITERAL(34, 448, 13), // "GetOrderbooks"
QT_MOC_LITERAL(35, 462, 10), // "OrderPlace"
QT_MOC_LITERAL(36, 473, 11), // "OrderCancel"
QT_MOC_LITERAL(37, 485, 8), // "OrderGet"
QT_MOC_LITERAL(38, 494, 12), // "OrderReplace"
QT_MOC_LITERAL(39, 507, 17), // "WithdrawGetLimits"
QT_MOC_LITERAL(40, 525, 12), // "WithdrawList"
QT_MOC_LITERAL(41, 538, 15), // "WithdrawHistory"
QT_MOC_LITERAL(42, 554, 14), // "WithdrawCancel"
QT_MOC_LITERAL(43, 569, 14), // "WithdrawCreate"
QT_MOC_LITERAL(44, 584, 13), // "WithdrawInner"
QT_MOC_LITERAL(45, 598, 14), // "WithdrawGetFee"
QT_MOC_LITERAL(46, 613, 11), // "GetBalances"
QT_MOC_LITERAL(47, 625, 11), // "GetDeposits"
QT_MOC_LITERAL(48, 637, 17), // "GetDepositAddress"
QT_MOC_LITERAL(49, 655, 12), // "TradeHistory"
QT_MOC_LITERAL(50, 668, 15), // "TradeOpenOrders"
QT_MOC_LITERAL(51, 684, 11), // "TradeGetFee"
QT_MOC_LITERAL(52, 696, 14) // "TradeGetLimits"

    },
    "SwiftApiClient\0pubWampResponse\0\0topic\0"
    "j_res\0uuid\0pubWamp\0customRequestResult\0"
    "reqname\0data\0apiParsedResponse\0"
    "SwiftApiClient::AsyncMethods\0method\0"
    "j_result\0parseApiResponse\0resetLimits\0"
    "unfreeze\0sendApiStateReport\0onWampSession_\0"
    "Wamp::Session*\0session_\0onNetworkReply\0"
    "QNetworkReply*\0reply\0processAsyncQueueResult\0"
    "processAsyncQueue\0onApiResponseParsed\0"
    "customMethodParsers\0ApiErrors\0"
    "HttpResponseError\0AsyncMethods\0"
    "TimeSyncMethod\0GetCurrencies\0GetMarkets\0"
    "GetOrderbooks\0OrderPlace\0OrderCancel\0"
    "OrderGet\0OrderReplace\0WithdrawGetLimits\0"
    "WithdrawList\0WithdrawHistory\0"
    "WithdrawCancel\0WithdrawCreate\0"
    "WithdrawInner\0WithdrawGetFee\0GetBalances\0"
    "GetDeposits\0GetDepositAddress\0"
    "TradeHistory\0TradeOpenOrders\0TradeGetFee\0"
    "TradeGetLimits"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SwiftApiClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       2,  134, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   84,    2, 0x06 /* Public */,
       6,    2,   91,    2, 0x06 /* Public */,
       7,    2,   96,    2, 0x06 /* Public */,
      10,    2,  101,    2, 0x06 /* Public */,
      14,    3,  106,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,  113,    2, 0x0a /* Public */,
      16,    0,  114,    2, 0x0a /* Public */,
      17,    0,  115,    2, 0x0a /* Public */,
      18,    1,  116,    2, 0x0a /* Public */,
      21,    1,  119,    2, 0x0a /* Public */,
      24,    0,  122,    2, 0x0a /* Public */,
      25,    0,  123,    2, 0x0a /* Public */,
      26,    2,  124,    2, 0x0a /* Public */,
      27,    2,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject, QMetaType::ULongLong,    3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    8,    9,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QJsonObject,   12,   13,
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 11, QMetaType::QByteArray,    5,   12,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong, QMetaType::QJsonObject,    5,   13,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QJsonObject,   12,   13,

 // enums: name, alias, flags, count, data
      28,   28, 0x0,    1,  144,
      30,   30, 0x0,   22,  146,

 // enum data: key, value
      29, uint(SwiftApiClient::HttpResponseError),
      31, uint(SwiftApiClient::TimeSyncMethod),
      32, uint(SwiftApiClient::GetCurrencies),
      33, uint(SwiftApiClient::GetMarkets),
      34, uint(SwiftApiClient::GetOrderbooks),
      35, uint(SwiftApiClient::OrderPlace),
      36, uint(SwiftApiClient::OrderCancel),
      37, uint(SwiftApiClient::OrderGet),
      38, uint(SwiftApiClient::OrderReplace),
      39, uint(SwiftApiClient::WithdrawGetLimits),
      40, uint(SwiftApiClient::WithdrawList),
      41, uint(SwiftApiClient::WithdrawHistory),
      42, uint(SwiftApiClient::WithdrawCancel),
      43, uint(SwiftApiClient::WithdrawCreate),
      44, uint(SwiftApiClient::WithdrawInner),
      45, uint(SwiftApiClient::WithdrawGetFee),
      46, uint(SwiftApiClient::GetBalances),
      47, uint(SwiftApiClient::GetDeposits),
      48, uint(SwiftApiClient::GetDepositAddress),
      49, uint(SwiftApiClient::TradeHistory),
      50, uint(SwiftApiClient::TradeOpenOrders),
      51, uint(SwiftApiClient::TradeGetFee),
      52, uint(SwiftApiClient::TradeGetLimits),

       0        // eod
};

void SwiftApiClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SwiftApiClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pubWampResponse((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2])),(*reinterpret_cast< const quint64(*)>(_a[3]))); break;
        case 1: _t->pubWamp((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 2: _t->customRequestResult((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 3: _t->apiParsedResponse((*reinterpret_cast< const SwiftApiClient::AsyncMethods(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 4: _t->parseApiResponse((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const SwiftApiClient::AsyncMethods(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        case 5: _t->resetLimits(); break;
        case 6: _t->unfreeze(); break;
        case 7: _t->sendApiStateReport(); break;
        case 8: _t->onWampSession_((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 9: _t->onNetworkReply((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 10: _t->processAsyncQueueResult(); break;
        case 11: _t->processAsyncQueue(); break;
        case 12: _t->onApiResponseParsed((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 13: _t->customMethodParsers((*reinterpret_cast< const SwiftApiClient::AsyncMethods(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Wamp::Session* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SwiftApiClient::*)(const QString & , const QJsonObject & , const quint64 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwiftApiClient::pubWampResponse)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SwiftApiClient::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwiftApiClient::pubWamp)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SwiftApiClient::*)(const QString & , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwiftApiClient::customRequestResult)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SwiftApiClient::*)(const SwiftApiClient::AsyncMethods & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwiftApiClient::apiParsedResponse)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SwiftApiClient::*)(const quint64 & , const SwiftApiClient::AsyncMethods & , const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SwiftApiClient::parseApiResponse)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SwiftApiClient::staticMetaObject = { {
    QMetaObject::SuperData::link<SwiftWorker::staticMetaObject>(),
    qt_meta_stringdata_SwiftApiClient.data,
    qt_meta_data_SwiftApiClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SwiftApiClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwiftApiClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SwiftApiClient.stringdata0))
        return static_cast<void*>(this);
    return SwiftWorker::qt_metacast(_clname);
}

int SwiftApiClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SwiftWorker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void SwiftApiClient::pubWampResponse(const QString & _t1, const QJsonObject & _t2, const quint64 & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SwiftApiClient::pubWamp(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SwiftApiClient::customRequestResult(const QString & _t1, const QByteArray & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SwiftApiClient::apiParsedResponse(const SwiftApiClient::AsyncMethods & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SwiftApiClient::parseApiResponse(const quint64 & _t1, const SwiftApiClient::AsyncMethods & _t2, const QByteArray & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
