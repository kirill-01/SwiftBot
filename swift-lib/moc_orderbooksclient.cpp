/****************************************************************************
** Meta object code from reading C++ file 'orderbooksclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "orderbooksclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orderbooksclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrderbooksClient_t {
    QByteArrayData data[15];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrderbooksClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrderbooksClient_t qt_meta_stringdata_OrderbooksClient = {
    {
QT_MOC_LITERAL(0, 0, 16), // "OrderbooksClient"
QT_MOC_LITERAL(1, 17, 10), // "orderbooks"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 35), // "QHash<quint32,QMap<double,dou..."
QT_MOC_LITERAL(4, 65, 4), // "asks"
QT_MOC_LITERAL(5, 70, 4), // "bids"
QT_MOC_LITERAL(6, 75, 12), // "onOrderBooks"
QT_MOC_LITERAL(7, 88, 7), // "j_books"
QT_MOC_LITERAL(8, 96, 13), // "onWampSession"
QT_MOC_LITERAL(9, 110, 14), // "Wamp::Session*"
QT_MOC_LITERAL(10, 125, 4), // "sess"
QT_MOC_LITERAL(11, 130, 10), // "stopClient"
QT_MOC_LITERAL(12, 141, 11), // "startClient"
QT_MOC_LITERAL(13, 153, 14), // "QList<quint32>"
QT_MOC_LITERAL(14, 168, 6) // "filter"

    },
    "OrderbooksClient\0orderbooks\0\0"
    "QHash<quint32,QMap<double,double> >\0"
    "asks\0bids\0onOrderBooks\0j_books\0"
    "onWampSession\0Wamp::Session*\0sess\0"
    "stopClient\0startClient\0QList<quint32>\0"
    "filter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrderbooksClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   49,    2, 0x0a /* Public */,
       8,    1,   52,    2, 0x0a /* Public */,
      11,    0,   55,    2, 0x0a /* Public */,
      12,    1,   56,    2, 0x0a /* Public */,
      12,    0,   59,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonObject,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,

       0        // eod
};

void OrderbooksClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrderbooksClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->orderbooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 1: _t->onOrderBooks((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 2: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 3: _t->stopClient(); break;
        case 4: _t->startClient((*reinterpret_cast< const QList<quint32>(*)>(_a[1]))); break;
        case 5: _t->startClient(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Wamp::Session* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<quint32> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OrderbooksClient::*)(const QHash<quint32,QMap<double,double>> & , const QHash<quint32,QMap<double,double>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderbooksClient::orderbooks)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OrderbooksClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_OrderbooksClient.data,
    qt_meta_data_OrderbooksClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OrderbooksClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrderbooksClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrderbooksClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OrderbooksClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void OrderbooksClient::orderbooks(const QHash<quint32,QMap<double,double>> & _t1, const QHash<quint32,QMap<double,double>> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
