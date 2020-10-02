/****************************************************************************
** Meta object code from reading C++ file 'orderbooksnode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "orderbooksnode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orderbooksnode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrderbooksNode_t {
    QByteArrayData data[13];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrderbooksNode_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrderbooksNode_t qt_meta_stringdata_OrderbooksNode = {
    {
QT_MOC_LITERAL(0, 0, 14), // "OrderbooksNode"
QT_MOC_LITERAL(1, 15, 10), // "orderbooks"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 1), // "a"
QT_MOC_LITERAL(4, 29, 1), // "b"
QT_MOC_LITERAL(5, 31, 9), // "nodeError"
QT_MOC_LITERAL(6, 41, 26), // "OrderbooksNode::NodeErrors"
QT_MOC_LITERAL(7, 68, 1), // "e"
QT_MOC_LITERAL(8, 70, 12), // "aboutToClose"
QT_MOC_LITERAL(9, 83, 8), // "stopNode"
QT_MOC_LITERAL(10, 92, 9), // "startNode"
QT_MOC_LITERAL(11, 102, 15), // "onRawOrderbooks"
QT_MOC_LITERAL(12, 118, 14) // "custom_pair_id"

    },
    "OrderbooksNode\0orderbooks\0\0a\0b\0nodeError\0"
    "OrderbooksNode::NodeErrors\0e\0aboutToClose\0"
    "stopNode\0startNode\0onRawOrderbooks\0"
    "custom_pair_id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrderbooksNode[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       5,    1,   54,    2, 0x06 /* Public */,
       8,    0,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   58,    2, 0x0a /* Public */,
      10,    0,   59,    2, 0x0a /* Public */,
      11,    3,   60,    2, 0x0a /* Public */,
      11,    2,   67,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonArray, QMetaType::QJsonArray,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonArray, QMetaType::QJsonArray, QMetaType::UInt,    3,    4,   12,
    QMetaType::Void, QMetaType::QJsonArray, QMetaType::QJsonArray,    3,    4,

       0        // eod
};

void OrderbooksNode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrderbooksNode *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->orderbooks((*reinterpret_cast< const QJsonArray(*)>(_a[1])),(*reinterpret_cast< const QJsonArray(*)>(_a[2]))); break;
        case 1: _t->nodeError((*reinterpret_cast< OrderbooksNode::NodeErrors(*)>(_a[1]))); break;
        case 2: _t->aboutToClose(); break;
        case 3: _t->stopNode(); break;
        case 4: _t->startNode(); break;
        case 5: _t->onRawOrderbooks((*reinterpret_cast< const QJsonArray(*)>(_a[1])),(*reinterpret_cast< const QJsonArray(*)>(_a[2])),(*reinterpret_cast< const quint32(*)>(_a[3]))); break;
        case 6: _t->onRawOrderbooks((*reinterpret_cast< const QJsonArray(*)>(_a[1])),(*reinterpret_cast< const QJsonArray(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrderbooksNode::*)(const QJsonArray & , const QJsonArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderbooksNode::orderbooks)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OrderbooksNode::*)(OrderbooksNode::NodeErrors );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderbooksNode::nodeError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (OrderbooksNode::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderbooksNode::aboutToClose)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OrderbooksNode::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_OrderbooksNode.data,
    qt_meta_data_OrderbooksNode,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OrderbooksNode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrderbooksNode::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrderbooksNode.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OrderbooksNode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void OrderbooksNode::orderbooks(const QJsonArray & _t1, const QJsonArray & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OrderbooksNode::nodeError(OrderbooksNode::NodeErrors _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void OrderbooksNode::aboutToClose()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
