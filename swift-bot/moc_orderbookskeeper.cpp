/****************************************************************************
** Meta object code from reading C++ file 'orderbookskeeper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "orderbookskeeper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'orderbookskeeper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OrderbooksKeeper_t {
    QByteArrayData data[17];
    char stringdata0[203];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OrderbooksKeeper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OrderbooksKeeper_t qt_meta_stringdata_OrderbooksKeeper = {
    {
QT_MOC_LITERAL(0, 0, 16), // "OrderbooksKeeper"
QT_MOC_LITERAL(1, 17, 10), // "orderbooks"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 35), // "QHash<quint32,QMap<double,dou..."
QT_MOC_LITERAL(4, 65, 4), // "asks"
QT_MOC_LITERAL(5, 70, 4), // "bids"
QT_MOC_LITERAL(6, 75, 14), // "publishMessage"
QT_MOC_LITERAL(7, 90, 5), // "topic"
QT_MOC_LITERAL(8, 96, 5), // "j_val"
QT_MOC_LITERAL(9, 102, 12), // "onOrderBooks"
QT_MOC_LITERAL(10, 115, 7), // "j_books"
QT_MOC_LITERAL(11, 123, 14), // "onRealmSession"
QT_MOC_LITERAL(12, 138, 14), // "Wamp::Session*"
QT_MOC_LITERAL(13, 153, 7), // "session"
QT_MOC_LITERAL(14, 161, 15), // "flushOrderbooks"
QT_MOC_LITERAL(15, 177, 13), // "onWampSession"
QT_MOC_LITERAL(16, 191, 11) // "startKeeper"

    },
    "OrderbooksKeeper\0orderbooks\0\0"
    "QHash<quint32,QMap<double,double> >\0"
    "asks\0bids\0publishMessage\0topic\0j_val\0"
    "onOrderBooks\0j_books\0onRealmSession\0"
    "Wamp::Session*\0session\0flushOrderbooks\0"
    "onWampSession\0startKeeper"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OrderbooksKeeper[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       6,    2,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   59,    2, 0x0a /* Public */,
      11,    1,   62,    2, 0x0a /* Public */,
      14,    0,   65,    2, 0x0a /* Public */,
      15,    1,   66,    2, 0x0a /* Public */,
      16,    0,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    7,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonObject,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,

       0        // eod
};

void OrderbooksKeeper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OrderbooksKeeper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->orderbooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 1: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 2: _t->onOrderBooks((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->onRealmSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 4: _t->flushOrderbooks(); break;
        case 5: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 6: _t->startKeeper(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Wamp::Session* >(); break;
            }
            break;
        case 5:
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
            using _t = void (OrderbooksKeeper::*)(const QHash<quint32,QMap<double,double>> & , const QHash<quint32,QMap<double,double>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderbooksKeeper::orderbooks)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OrderbooksKeeper::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OrderbooksKeeper::publishMessage)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OrderbooksKeeper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_OrderbooksKeeper.data,
    qt_meta_data_OrderbooksKeeper,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OrderbooksKeeper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OrderbooksKeeper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OrderbooksKeeper.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OrderbooksKeeper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void OrderbooksKeeper::orderbooks(const QHash<quint32,QMap<double,double>> & _t1, const QHash<quint32,QMap<double,double>> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void OrderbooksKeeper::publishMessage(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
