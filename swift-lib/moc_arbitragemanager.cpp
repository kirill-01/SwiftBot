/****************************************************************************
** Meta object code from reading C++ file 'arbitragemanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "arbitragemanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'arbitragemanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ArbitrageManager_t {
    QByteArrayData data[29];
    char stringdata0[424];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArbitrageManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArbitrageManager_t qt_meta_stringdata_ArbitrageManager = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ArbitrageManager"
QT_MOC_LITERAL(1, 17, 16), // "sessionConnected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 14), // "Wamp::Session*"
QT_MOC_LITERAL(4, 50, 1), // "s"
QT_MOC_LITERAL(5, 52, 15), // "sendWampMessage"
QT_MOC_LITERAL(6, 68, 5), // "topic"
QT_MOC_LITERAL(7, 74, 7), // "message"
QT_MOC_LITERAL(8, 82, 15), // "errorOrderEvent"
QT_MOC_LITERAL(9, 98, 5), // "j_itm"
QT_MOC_LITERAL(10, 104, 19), // "completedOrderEvent"
QT_MOC_LITERAL(11, 124, 16), // "placedOrderEvent"
QT_MOC_LITERAL(12, 141, 18), // "canceledOrderEvent"
QT_MOC_LITERAL(13, 160, 15), // "checkOrderbooks"
QT_MOC_LITERAL(14, 176, 35), // "QHash<quint32,QMap<double,dou..."
QT_MOC_LITERAL(15, 212, 4), // "asks"
QT_MOC_LITERAL(16, 217, 4), // "bids"
QT_MOC_LITERAL(17, 222, 21), // "tocalculateOrderbooks"
QT_MOC_LITERAL(18, 244, 15), // "updateRequested"
QT_MOC_LITERAL(19, 260, 19), // "calculateOrderbooks"
QT_MOC_LITERAL(20, 280, 15), // "checkNewWindows"
QT_MOC_LITERAL(21, 296, 13), // "onWampSession"
QT_MOC_LITERAL(22, 310, 7), // "session"
QT_MOC_LITERAL(23, 318, 16), // "loadPairSettings"
QT_MOC_LITERAL(24, 335, 11), // "loadDbPairs"
QT_MOC_LITERAL(25, 347, 20), // "loadPairRatesHistory"
QT_MOC_LITERAL(26, 368, 20), // "saveOrdersRelsToDisk"
QT_MOC_LITERAL(27, 389, 21), // "loadOrdersRelFromDisk"
QT_MOC_LITERAL(28, 411, 12) // "sendSnapshot"

    },
    "ArbitrageManager\0sessionConnected\0\0"
    "Wamp::Session*\0s\0sendWampMessage\0topic\0"
    "message\0errorOrderEvent\0j_itm\0"
    "completedOrderEvent\0placedOrderEvent\0"
    "canceledOrderEvent\0checkOrderbooks\0"
    "QHash<quint32,QMap<double,double> >\0"
    "asks\0bids\0tocalculateOrderbooks\0"
    "updateRequested\0calculateOrderbooks\0"
    "checkNewWindows\0onWampSession\0session\0"
    "loadPairSettings\0loadDbPairs\0"
    "loadPairRatesHistory\0saveOrdersRelsToDisk\0"
    "loadOrdersRelFromDisk\0sendSnapshot"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArbitrageManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,
       5,    2,  107,    2, 0x06 /* Public */,
       8,    1,  112,    2, 0x06 /* Public */,
      10,    1,  115,    2, 0x06 /* Public */,
      11,    1,  118,    2, 0x06 /* Public */,
      12,    1,  121,    2, 0x06 /* Public */,
      13,    2,  124,    2, 0x06 /* Public */,
      17,    2,  129,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,  134,    2, 0x0a /* Public */,
      19,    2,  135,    2, 0x0a /* Public */,
      20,    0,  140,    2, 0x0a /* Public */,
      21,    1,  141,    2, 0x0a /* Public */,
      23,    0,  144,    2, 0x0a /* Public */,
      24,    0,  145,    2, 0x0a /* Public */,
      25,    0,  146,    2, 0x0a /* Public */,
      26,    0,  147,    2, 0x0a /* Public */,
      27,    0,  148,    2, 0x0a /* Public */,
      28,    0,  149,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::QJsonObject,    9,
    QMetaType::Void, QMetaType::QJsonObject,    9,
    QMetaType::Void, QMetaType::QJsonObject,    9,
    QMetaType::Void, QMetaType::QJsonObject,    9,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 14,   15,   16,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 14,   15,   16,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14, 0x80000000 | 14,   15,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ArbitrageManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ArbitrageManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sessionConnected((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 1: _t->sendWampMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->errorOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->completedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->placedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->canceledOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->checkOrderbooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 7: _t->tocalculateOrderbooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 8: _t->updateRequested(); break;
        case 9: _t->calculateOrderbooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 10: _t->checkNewWindows(); break;
        case 11: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 12: _t->loadPairSettings(); break;
        case 13: _t->loadDbPairs(); break;
        case 14: _t->loadPairRatesHistory(); break;
        case 15: _t->saveOrdersRelsToDisk(); break;
        case 16: _t->loadOrdersRelFromDisk(); break;
        case 17: _t->sendSnapshot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Wamp::Session* >(); break;
            }
            break;
        case 11:
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
            using _t = void (ArbitrageManager::*)(Wamp::Session * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::sessionConnected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ArbitrageManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::sendWampMessage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ArbitrageManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::errorOrderEvent)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ArbitrageManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::completedOrderEvent)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ArbitrageManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::placedOrderEvent)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ArbitrageManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::canceledOrderEvent)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ArbitrageManager::*)(const QHash<quint32,QMap<double,double> > & , const QHash<quint32,QMap<double,double> > & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::checkOrderbooks)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ArbitrageManager::*)(const QHash<quint32,QMap<double,double>> & , const QHash<quint32,QMap<double,double>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitrageManager::tocalculateOrderbooks)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ArbitrageManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ArbitrageManager.data,
    qt_meta_data_ArbitrageManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ArbitrageManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArbitrageManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArbitrageManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ArbitrageManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void ArbitrageManager::sessionConnected(Wamp::Session * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ArbitrageManager::sendWampMessage(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ArbitrageManager::errorOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ArbitrageManager::completedOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ArbitrageManager::placedOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ArbitrageManager::canceledOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ArbitrageManager::checkOrderbooks(const QHash<quint32,QMap<double,double> > & _t1, const QHash<quint32,QMap<double,double> > & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ArbitrageManager::tocalculateOrderbooks(const QHash<quint32,QMap<double,double>> & _t1, const QHash<quint32,QMap<double,double>> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
