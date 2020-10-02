/****************************************************************************
** Meta object code from reading C++ file 'balancesworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "balancesworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'balancesworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BalancesWorker_t {
    QByteArrayData data[40];
    char stringdata0[560];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BalancesWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BalancesWorker_t qt_meta_stringdata_BalancesWorker = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BalancesWorker"
QT_MOC_LITERAL(1, 15, 13), // "balancesEvent"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 5), // "event"
QT_MOC_LITERAL(4, 36, 13), // "depositsEvent"
QT_MOC_LITERAL(5, 50, 14), // "withdrawsEvent"
QT_MOC_LITERAL(6, 65, 14), // "withdrawCeated"
QT_MOC_LITERAL(7, 80, 15), // "withdrawHistory"
QT_MOC_LITERAL(8, 96, 13), // "deposiHistory"
QT_MOC_LITERAL(9, 110, 11), // "updateRates"
QT_MOC_LITERAL(10, 122, 13), // "onUpdateEvent"
QT_MOC_LITERAL(11, 136, 15), // "onDepositsEvent"
QT_MOC_LITERAL(12, 152, 16), // "onWithdrawsEvent"
QT_MOC_LITERAL(13, 169, 16), // "onDepositHistory"
QT_MOC_LITERAL(14, 186, 16), // "onWithdrawCeated"
QT_MOC_LITERAL(15, 203, 17), // "onWithdrawHistory"
QT_MOC_LITERAL(16, 221, 12), // "processQueue"
QT_MOC_LITERAL(17, 234, 19), // "getDepositsStatuses"
QT_MOC_LITERAL(18, 254, 19), // "getWithdrawStatuses"
QT_MOC_LITERAL(19, 274, 12), // "beginCollect"
QT_MOC_LITERAL(20, 287, 13), // "onWampSession"
QT_MOC_LITERAL(21, 301, 14), // "Wamp::Session*"
QT_MOC_LITERAL(22, 316, 4), // "sess"
QT_MOC_LITERAL(23, 321, 9), // "onBalance"
QT_MOC_LITERAL(24, 331, 3), // "res"
QT_MOC_LITERAL(25, 335, 18), // "calculateSummaries"
QT_MOC_LITERAL(26, 354, 18), // "saveBalanceHistory"
QT_MOC_LITERAL(27, 373, 26), // "calculateBalancerTransfers"
QT_MOC_LITERAL(28, 400, 13), // "loadAddresses"
QT_MOC_LITERAL(29, 414, 7), // "getRate"
QT_MOC_LITERAL(30, 422, 21), // "getExternalBtcBalance"
QT_MOC_LITERAL(31, 444, 16), // "updateRatesCache"
QT_MOC_LITERAL(32, 461, 16), // "getTodayRateDiff"
QT_MOC_LITERAL(33, 478, 19), // "getTodayTradeProfit"
QT_MOC_LITERAL(34, 498, 13), // "insertAddress"
QT_MOC_LITERAL(35, 512, 4), // "exch"
QT_MOC_LITERAL(36, 517, 4), // "coin"
QT_MOC_LITERAL(37, 522, 7), // "address"
QT_MOC_LITERAL(38, 530, 16), // "calculatePeriods"
QT_MOC_LITERAL(39, 547, 12) // "getRatesDiff"

    },
    "BalancesWorker\0balancesEvent\0\0event\0"
    "depositsEvent\0withdrawsEvent\0"
    "withdrawCeated\0withdrawHistory\0"
    "deposiHistory\0updateRates\0onUpdateEvent\0"
    "onDepositsEvent\0onWithdrawsEvent\0"
    "onDepositHistory\0onWithdrawCeated\0"
    "onWithdrawHistory\0processQueue\0"
    "getDepositsStatuses\0getWithdrawStatuses\0"
    "beginCollect\0onWampSession\0Wamp::Session*\0"
    "sess\0onBalance\0res\0calculateSummaries\0"
    "saveBalanceHistory\0calculateBalancerTransfers\0"
    "loadAddresses\0getRate\0getExternalBtcBalance\0"
    "updateRatesCache\0getTodayRateDiff\0"
    "getTodayTradeProfit\0insertAddress\0"
    "exch\0coin\0address\0calculatePeriods\0"
    "getRatesDiff"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BalancesWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      31,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  169,    2, 0x06 /* Public */,
       4,    1,  172,    2, 0x06 /* Public */,
       5,    1,  175,    2, 0x06 /* Public */,
       6,    1,  178,    2, 0x06 /* Public */,
       7,    1,  181,    2, 0x06 /* Public */,
       8,    1,  184,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,  187,    2, 0x0a /* Public */,
      10,    1,  188,    2, 0x0a /* Public */,
      11,    1,  191,    2, 0x0a /* Public */,
      12,    1,  194,    2, 0x0a /* Public */,
      13,    1,  197,    2, 0x0a /* Public */,
      14,    1,  200,    2, 0x0a /* Public */,
      15,    1,  203,    2, 0x0a /* Public */,
      16,    0,  206,    2, 0x0a /* Public */,
      17,    0,  207,    2, 0x0a /* Public */,
      18,    0,  208,    2, 0x0a /* Public */,
      19,    0,  209,    2, 0x0a /* Public */,
      20,    1,  210,    2, 0x0a /* Public */,
      23,    1,  213,    2, 0x0a /* Public */,
      25,    0,  216,    2, 0x0a /* Public */,
      26,    0,  217,    2, 0x0a /* Public */,
      27,    0,  218,    2, 0x0a /* Public */,
      28,    0,  219,    2, 0x0a /* Public */,
      29,    0,  220,    2, 0x0a /* Public */,
      30,    0,  221,    2, 0x0a /* Public */,
      31,    0,  222,    2, 0x0a /* Public */,
      32,    0,  223,    2, 0x0a /* Public */,
      33,    0,  224,    2, 0x0a /* Public */,
      34,    3,  225,    2, 0x0a /* Public */,
      38,    0,  232,    2, 0x0a /* Public */,
      39,    0,  233,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, QMetaType::QJsonObject,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::QString,   35,   36,   37,
    QMetaType::Void,
    QMetaType::QJsonArray,

       0        // eod
};

void BalancesWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BalancesWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->balancesEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->depositsEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 2: _t->withdrawsEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->withdrawCeated((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->withdrawHistory((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->deposiHistory((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->updateRates(); break;
        case 7: _t->onUpdateEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 8: _t->onDepositsEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 9: _t->onWithdrawsEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 10: _t->onDepositHistory((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 11: _t->onWithdrawCeated((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 12: _t->onWithdrawHistory((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 13: _t->processQueue(); break;
        case 14: _t->getDepositsStatuses(); break;
        case 15: _t->getWithdrawStatuses(); break;
        case 16: _t->beginCollect(); break;
        case 17: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 18: _t->onBalance((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 19: _t->calculateSummaries(); break;
        case 20: _t->saveBalanceHistory(); break;
        case 21: _t->calculateBalancerTransfers(); break;
        case 22: _t->loadAddresses(); break;
        case 23: _t->getRate(); break;
        case 24: _t->getExternalBtcBalance(); break;
        case 25: _t->updateRatesCache(); break;
        case 26: _t->getTodayRateDiff(); break;
        case 27: _t->getTodayTradeProfit(); break;
        case 28: _t->insertAddress((*reinterpret_cast< const quint32(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 29: _t->calculatePeriods(); break;
        case 30: { QJsonArray _r = _t->getRatesDiff();
            if (_a[0]) *reinterpret_cast< QJsonArray*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Wamp::Session* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BalancesWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalancesWorker::balancesEvent)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BalancesWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalancesWorker::depositsEvent)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BalancesWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalancesWorker::withdrawsEvent)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BalancesWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalancesWorker::withdrawCeated)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BalancesWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalancesWorker::withdrawHistory)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BalancesWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalancesWorker::deposiHistory)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BalancesWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_BalancesWorker.data,
    qt_meta_data_BalancesWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BalancesWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BalancesWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BalancesWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BalancesWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 31)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 31;
    }
    return _id;
}

// SIGNAL 0
void BalancesWorker::balancesEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BalancesWorker::depositsEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BalancesWorker::withdrawsEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BalancesWorker::withdrawCeated(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BalancesWorker::withdrawHistory(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BalancesWorker::deposiHistory(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
