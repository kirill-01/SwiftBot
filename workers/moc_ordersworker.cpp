/****************************************************************************
** Meta object code from reading C++ file 'ordersworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "ordersworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ordersworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrdersWorker_t {
    QByteArrayData data[37];
    char stringdata0[524];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrdersWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrdersWorker_t qt_meta_stringdata_OrdersWorker = {
    {
QT_MOC_LITERAL(0, 0, 12), // "OrdersWorker"
QT_MOC_LITERAL(1, 13, 14), // "deleteOrderRec"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 1), // "j"
QT_MOC_LITERAL(4, 31, 14), // "publishMessage"
QT_MOC_LITERAL(5, 46, 5), // "topic"
QT_MOC_LITERAL(6, 52, 3), // "msg"
QT_MOC_LITERAL(7, 56, 9), // "saveOrder"
QT_MOC_LITERAL(8, 66, 7), // "j_order"
QT_MOC_LITERAL(9, 74, 14), // "saveOrderState"
QT_MOC_LITERAL(10, 89, 17), // "processApiHistory"
QT_MOC_LITERAL(11, 107, 7), // "j_items"
QT_MOC_LITERAL(12, 115, 17), // "processApiCurrent"
QT_MOC_LITERAL(13, 133, 15), // "market_privided"
QT_MOC_LITERAL(14, 149, 15), // "errorOrderEvent"
QT_MOC_LITERAL(15, 165, 5), // "j_itm"
QT_MOC_LITERAL(16, 171, 19), // "completedOrderEvent"
QT_MOC_LITERAL(17, 191, 16), // "placedOrderEvent"
QT_MOC_LITERAL(18, 208, 18), // "canceledOrderEvent"
QT_MOC_LITERAL(19, 227, 15), // "fixWinRelations"
QT_MOC_LITERAL(20, 243, 12), // "checkUpdates"
QT_MOC_LITERAL(21, 256, 12), // "updateConfig"
QT_MOC_LITERAL(22, 269, 4), // "conf"
QT_MOC_LITERAL(23, 274, 11), // "startClient"
QT_MOC_LITERAL(24, 286, 24), // "checkIndividualOpenState"
QT_MOC_LITERAL(25, 311, 19), // "onProcessApiCurrent"
QT_MOC_LITERAL(26, 331, 19), // "onProcessApiHistory"
QT_MOC_LITERAL(27, 351, 17), // "onErrorOrderEvent"
QT_MOC_LITERAL(28, 369, 21), // "onCompletedOrderEvent"
QT_MOC_LITERAL(29, 391, 18), // "onPlacedOrderEvent"
QT_MOC_LITERAL(30, 410, 20), // "onCanceledOrderEvent"
QT_MOC_LITERAL(31, 431, 14), // "onOrdersFromDb"
QT_MOC_LITERAL(32, 446, 7), // "stored_"
QT_MOC_LITERAL(33, 454, 15), // "onWampConnected"
QT_MOC_LITERAL(34, 470, 20), // "checkAwaitingUpdates"
QT_MOC_LITERAL(35, 491, 13), // "getOpenOrders"
QT_MOC_LITERAL(36, 505, 18) // "getCompletedOrders"

    },
    "OrdersWorker\0deleteOrderRec\0\0j\0"
    "publishMessage\0topic\0msg\0saveOrder\0"
    "j_order\0saveOrderState\0processApiHistory\0"
    "j_items\0processApiCurrent\0market_privided\0"
    "errorOrderEvent\0j_itm\0completedOrderEvent\0"
    "placedOrderEvent\0canceledOrderEvent\0"
    "fixWinRelations\0checkUpdates\0updateConfig\0"
    "conf\0startClient\0checkIndividualOpenState\0"
    "onProcessApiCurrent\0onProcessApiHistory\0"
    "onErrorOrderEvent\0onCompletedOrderEvent\0"
    "onPlacedOrderEvent\0onCanceledOrderEvent\0"
    "onOrdersFromDb\0stored_\0onWampConnected\0"
    "checkAwaitingUpdates\0getOpenOrders\0"
    "getCompletedOrders"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrdersWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  154,    2, 0x06 /* Public */,
       4,    2,  157,    2, 0x06 /* Public */,
       7,    1,  162,    2, 0x06 /* Public */,
       9,    1,  165,    2, 0x06 /* Public */,
      10,    1,  168,    2, 0x06 /* Public */,
      12,    2,  171,    2, 0x06 /* Public */,
      12,    1,  176,    2, 0x26 /* Public | MethodCloned */,
      14,    1,  179,    2, 0x06 /* Public */,
      16,    1,  182,    2, 0x06 /* Public */,
      17,    1,  185,    2, 0x06 /* Public */,
      18,    1,  188,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    0,  191,    2, 0x0a /* Public */,
      20,    0,  192,    2, 0x0a /* Public */,
      21,    1,  193,    2, 0x0a /* Public */,
      23,    0,  196,    2, 0x0a /* Public */,
      24,    0,  197,    2, 0x0a /* Public */,
      25,    2,  198,    2, 0x0a /* Public */,
      25,    1,  203,    2, 0x2a /* Public | MethodCloned */,
      26,    1,  206,    2, 0x0a /* Public */,
      27,    1,  209,    2, 0x0a /* Public */,
      28,    1,  212,    2, 0x0a /* Public */,
      29,    1,  215,    2, 0x0a /* Public */,
      30,    1,  218,    2, 0x0a /* Public */,
      31,    1,  221,    2, 0x0a /* Public */,
      33,    1,  224,    2, 0x0a /* Public */,
      34,    0,  227,    2, 0x0a /* Public */,
      35,    0,  228,    2, 0x0a /* Public */,
      36,    0,  229,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    5,    6,
    QMetaType::Void, QMetaType::QJsonObject,    8,
    QMetaType::Void, QMetaType::QJsonObject,    8,
    QMetaType::Void, QMetaType::QJsonArray,   11,
    QMetaType::Void, QMetaType::QJsonArray, QMetaType::UInt,   11,   13,
    QMetaType::Void, QMetaType::QJsonArray,   11,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonObject,   15,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonArray, QMetaType::UInt,   11,   13,
    QMetaType::Void, QMetaType::QJsonArray,   11,
    QMetaType::Void, QMetaType::QJsonArray,   11,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonObject,   15,
    QMetaType::Void, QMetaType::QJsonArray,   32,
    QMetaType::Void, QMetaType::ULongLong,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void OrdersWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrdersWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deleteOrderRec((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 2: _t->saveOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->saveOrderState((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->processApiHistory((*reinterpret_cast< const QJsonArray(*)>(_a[1]))); break;
        case 5: _t->processApiCurrent((*reinterpret_cast< const QJsonArray(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2]))); break;
        case 6: _t->processApiCurrent((*reinterpret_cast< const QJsonArray(*)>(_a[1]))); break;
        case 7: _t->errorOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 8: _t->completedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 9: _t->placedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 10: _t->canceledOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 11: _t->fixWinRelations(); break;
        case 12: _t->checkUpdates(); break;
        case 13: _t->updateConfig((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->startClient(); break;
        case 15: _t->checkIndividualOpenState(); break;
        case 16: _t->onProcessApiCurrent((*reinterpret_cast< const QJsonArray(*)>(_a[1])),(*reinterpret_cast< const quint32(*)>(_a[2]))); break;
        case 17: _t->onProcessApiCurrent((*reinterpret_cast< const QJsonArray(*)>(_a[1]))); break;
        case 18: _t->onProcessApiHistory((*reinterpret_cast< const QJsonArray(*)>(_a[1]))); break;
        case 19: _t->onErrorOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 20: _t->onCompletedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 21: _t->onPlacedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 22: _t->onCanceledOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 23: _t->onOrdersFromDb((*reinterpret_cast< const QJsonArray(*)>(_a[1]))); break;
        case 24: _t->onWampConnected((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 25: _t->checkAwaitingUpdates(); break;
        case 26: _t->getOpenOrders(); break;
        case 27: _t->getCompletedOrders(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrdersWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::deleteOrderRec)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::publishMessage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::saveOrder)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::saveOrderState)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::processApiHistory)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonArray & , const quint32 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::processApiCurrent)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::errorOrderEvent)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::completedOrderEvent)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::placedOrderEvent)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (OrdersWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrdersWorker::canceledOrderEvent)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OrdersWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_OrdersWorker.data,
    qt_meta_data_OrdersWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OrdersWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrdersWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrdersWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OrdersWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 28)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void OrdersWorker::deleteOrderRec(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OrdersWorker::publishMessage(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OrdersWorker::saveOrder(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OrdersWorker::saveOrderState(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void OrdersWorker::processApiHistory(const QJsonArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void OrdersWorker::processApiCurrent(const QJsonArray & _t1, const quint32 & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 7
void OrdersWorker::errorOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void OrdersWorker::completedOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void OrdersWorker::placedOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void OrdersWorker::canceledOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
