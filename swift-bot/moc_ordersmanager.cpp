/****************************************************************************
** Meta object code from reading C++ file 'ordersmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ordersmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ordersmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrdersManager_t {
    QByteArrayData data[26];
    char stringdata0[314];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrdersManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrdersManager_t qt_meta_stringdata_OrdersManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "OrdersManager"
QT_MOC_LITERAL(1, 14, 11), // "currentRate"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "rate"
QT_MOC_LITERAL(4, 32, 11), // "orderPlaced"
QT_MOC_LITERAL(5, 44, 23), // "requestExchangeBalances"
QT_MOC_LITERAL(6, 68, 11), // "exchange_id"
QT_MOC_LITERAL(7, 80, 14), // "publishMessage"
QT_MOC_LITERAL(8, 95, 5), // "topic"
QT_MOC_LITERAL(9, 101, 7), // "message"
QT_MOC_LITERAL(10, 109, 16), // "asyncOrderPlaced"
QT_MOC_LITERAL(11, 126, 17), // "asyncTaskFinished"
QT_MOC_LITERAL(12, 144, 4), // "uuid"
QT_MOC_LITERAL(13, 149, 10), // "clearLocks"
QT_MOC_LITERAL(14, 160, 12), // "onPlaceOrder"
QT_MOC_LITERAL(15, 173, 7), // "pair_id"
QT_MOC_LITERAL(16, 181, 6), // "amount"
QT_MOC_LITERAL(17, 188, 4), // "type"
QT_MOC_LITERAL(18, 193, 13), // "onWampSession"
QT_MOC_LITERAL(19, 207, 14), // "Wamp::Session*"
QT_MOC_LITERAL(20, 222, 4), // "sess"
QT_MOC_LITERAL(21, 227, 11), // "onCandidats"
QT_MOC_LITERAL(22, 239, 24), // "QList<ArbitrageCandidat>"
QT_MOC_LITERAL(23, 264, 9), // "candidats"
QT_MOC_LITERAL(24, 274, 22), // "loadCurrencyTotalUsage"
QT_MOC_LITERAL(25, 297, 16) // "getAllOpenOrders"

    },
    "OrdersManager\0currentRate\0\0rate\0"
    "orderPlaced\0requestExchangeBalances\0"
    "exchange_id\0publishMessage\0topic\0"
    "message\0asyncOrderPlaced\0asyncTaskFinished\0"
    "uuid\0clearLocks\0onPlaceOrder\0pair_id\0"
    "amount\0type\0onWampSession\0Wamp::Session*\0"
    "sess\0onCandidats\0QList<ArbitrageCandidat>\0"
    "candidats\0loadCurrencyTotalUsage\0"
    "getAllOpenOrders"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrdersManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    0,   77,    2, 0x06 /* Public */,
       5,    1,   78,    2, 0x06 /* Public */,
       7,    2,   81,    2, 0x06 /* Public */,
      10,    0,   86,    2, 0x06 /* Public */,
      11,    1,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   90,    2, 0x0a /* Public */,
      14,    4,   91,    2, 0x0a /* Public */,
      18,    1,  100,    2, 0x0a /* Public */,
      21,    1,  103,    2, 0x0a /* Public */,
      24,    0,  106,    2, 0x0a /* Public */,
      25,    0,  107,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    8,    9,
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::Double, QMetaType::Double, QMetaType::UInt,   15,   16,    3,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void OrdersManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrdersManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->currentRate((*reinterpret_cast< const double(*)>(_a[1]))); break;
        case 1: _t->orderPlaced(); break;
        case 2: _t->requestExchangeBalances((*reinterpret_cast< const quint32(*)>(_a[1]))); break;
        case 3: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 4: _t->asyncOrderPlaced(); break;
        case 5: _t->asyncTaskFinished((*reinterpret_cast< const quint64(*)>(_a[1]))); break;
        case 6: _t->clearLocks(); break;
        case 7: _t->onPlaceOrder((*reinterpret_cast< const quint32(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const quint32(*)>(_a[4]))); break;
        case 8: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 9: _t->onCandidats((*reinterpret_cast< const QList<ArbitrageCandidat>(*)>(_a[1]))); break;
        case 10: _t->loadCurrencyTotalUsage(); break;
        case 11: _t->getAllOpenOrders(); break;
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
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrdersManager::*)(const double & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersManager::currentRate)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OrdersManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersManager::orderPlaced)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OrdersManager::*)(const quint32 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersManager::requestExchangeBalances)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OrdersManager::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersManager::publishMessage)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OrdersManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersManager::asyncOrderPlaced)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (OrdersManager::*)(const quint64 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersManager::asyncTaskFinished)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OrdersManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_OrdersManager.data,
    qt_meta_data_OrdersManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OrdersManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrdersManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrdersManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OrdersManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void OrdersManager::currentRate(const double & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OrdersManager::orderPlaced()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void OrdersManager::requestExchangeBalances(const quint32 & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OrdersManager::publishMessage(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void OrdersManager::asyncOrderPlaced()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void OrdersManager::asyncTaskFinished(const quint64 & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
