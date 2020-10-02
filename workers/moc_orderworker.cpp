/****************************************************************************
** Meta object code from reading C++ file 'orderworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "orderworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orderworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrderWorker_t {
    QByteArrayData data[27];
    char stringdata0[425];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrderWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrderWorker_t qt_meta_stringdata_OrderWorker = {
    {
QT_MOC_LITERAL(0, 0, 11), // "OrderWorker"
QT_MOC_LITERAL(1, 12, 15), // "errorOrderEvent"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 5), // "j_itm"
QT_MOC_LITERAL(4, 35, 17), // "updatedOrderEvent"
QT_MOC_LITERAL(5, 53, 19), // "completedOrderEvent"
QT_MOC_LITERAL(6, 73, 16), // "placedOrderEvent"
QT_MOC_LITERAL(7, 90, 18), // "canceledOrderEvent"
QT_MOC_LITERAL(8, 109, 17), // "historyOrderEvent"
QT_MOC_LITERAL(9, 127, 16), // "activeOrderEvent"
QT_MOC_LITERAL(10, 144, 9), // "saveOrder"
QT_MOC_LITERAL(11, 154, 14), // "saveOrderState"
QT_MOC_LITERAL(12, 169, 14), // "onOrdersFromDb"
QT_MOC_LITERAL(13, 184, 7), // "stored_"
QT_MOC_LITERAL(14, 192, 12), // "checkUpdates"
QT_MOC_LITERAL(15, 205, 11), // "startClient"
QT_MOC_LITERAL(16, 217, 18), // "processActiveQueue"
QT_MOC_LITERAL(17, 236, 19), // "processHistoryQueue"
QT_MOC_LITERAL(18, 256, 14), // "requestActives"
QT_MOC_LITERAL(19, 271, 14), // "requestHistory"
QT_MOC_LITERAL(20, 286, 17), // "onErrorOrderEvent"
QT_MOC_LITERAL(21, 304, 19), // "onUpdatedOrderEvent"
QT_MOC_LITERAL(22, 324, 21), // "onCompletedOrderEvent"
QT_MOC_LITERAL(23, 346, 18), // "onPlacedOrderEvent"
QT_MOC_LITERAL(24, 365, 20), // "onCanceledOrderEvent"
QT_MOC_LITERAL(25, 386, 19), // "onHistoryOrderEvent"
QT_MOC_LITERAL(26, 406, 18) // "onActiveOrderEvent"

    },
    "OrderWorker\0errorOrderEvent\0\0j_itm\0"
    "updatedOrderEvent\0completedOrderEvent\0"
    "placedOrderEvent\0canceledOrderEvent\0"
    "historyOrderEvent\0activeOrderEvent\0"
    "saveOrder\0saveOrderState\0onOrdersFromDb\0"
    "stored_\0checkUpdates\0startClient\0"
    "processActiveQueue\0processHistoryQueue\0"
    "requestActives\0requestHistory\0"
    "onErrorOrderEvent\0onUpdatedOrderEvent\0"
    "onCompletedOrderEvent\0onPlacedOrderEvent\0"
    "onCanceledOrderEvent\0onHistoryOrderEvent\0"
    "onActiveOrderEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrderWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  129,    2, 0x06 /* Public */,
       4,    1,  132,    2, 0x06 /* Public */,
       5,    1,  135,    2, 0x06 /* Public */,
       6,    1,  138,    2, 0x06 /* Public */,
       7,    1,  141,    2, 0x06 /* Public */,
       8,    1,  144,    2, 0x06 /* Public */,
       9,    1,  147,    2, 0x06 /* Public */,
      10,    1,  150,    2, 0x06 /* Public */,
      11,    1,  153,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,  156,    2, 0x0a /* Public */,
      14,    0,  159,    2, 0x0a /* Public */,
      15,    0,  160,    2, 0x0a /* Public */,
      16,    0,  161,    2, 0x0a /* Public */,
      17,    0,  162,    2, 0x0a /* Public */,
      18,    0,  163,    2, 0x0a /* Public */,
      19,    0,  164,    2, 0x0a /* Public */,
      20,    1,  165,    2, 0x0a /* Public */,
      21,    1,  168,    2, 0x0a /* Public */,
      22,    1,  171,    2, 0x0a /* Public */,
      23,    1,  174,    2, 0x0a /* Public */,
      24,    1,  177,    2, 0x0a /* Public */,
      25,    1,  180,    2, 0x0a /* Public */,
      26,    1,  183,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonArray,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,

       0        // eod
};

void OrderWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrderWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->updatedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 2: _t->completedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->placedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->canceledOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->historyOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->activeOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 7: _t->saveOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 8: _t->saveOrderState((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 9: _t->onOrdersFromDb((*reinterpret_cast< const QJsonArray(*)>(_a[1]))); break;
        case 10: _t->checkUpdates(); break;
        case 11: _t->startClient(); break;
        case 12: _t->processActiveQueue(); break;
        case 13: _t->processHistoryQueue(); break;
        case 14: _t->requestActives(); break;
        case 15: _t->requestHistory(); break;
        case 16: _t->onErrorOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 17: _t->onUpdatedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 18: _t->onCompletedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 19: _t->onPlacedOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 20: _t->onCanceledOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 21: _t->onHistoryOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 22: _t->onActiveOrderEvent((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::errorOrderEvent)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::updatedOrderEvent)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::completedOrderEvent)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::placedOrderEvent)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::canceledOrderEvent)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::historyOrderEvent)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::activeOrderEvent)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::saveOrder)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (OrderWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderWorker::saveOrderState)) {
                *result = 8;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OrderWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<SwiftWorker::staticMetaObject>(),
    qt_meta_stringdata_OrderWorker.data,
    qt_meta_data_OrderWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OrderWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrderWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrderWorker.stringdata0))
        return static_cast<void*>(this);
    return SwiftWorker::qt_metacast(_clname);
}

int OrderWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SwiftWorker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void OrderWorker::errorOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OrderWorker::updatedOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OrderWorker::completedOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void OrderWorker::placedOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void OrderWorker::canceledOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void OrderWorker::historyOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void OrderWorker::activeOrderEvent(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void OrderWorker::saveOrder(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void OrderWorker::saveOrderState(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
