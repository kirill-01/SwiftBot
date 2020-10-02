/****************************************************************************
** Meta object code from reading C++ file 'arbitragepair.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "arbitragepair.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'arbitragepair.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ArbitragePair_t {
    QByteArrayData data[28];
    char stringdata0[344];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArbitragePair_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArbitragePair_t qt_meta_stringdata_ArbitragePair = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ArbitragePair"
QT_MOC_LITERAL(1, 14, 5), // "debug"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 3), // "msg"
QT_MOC_LITERAL(4, 25, 3), // "log"
QT_MOC_LITERAL(5, 29, 21), // "tocalculateOrderbooks"
QT_MOC_LITERAL(6, 51, 35), // "QHash<quint32,QMap<double,dou..."
QT_MOC_LITERAL(7, 87, 4), // "asks"
QT_MOC_LITERAL(8, 92, 4), // "bids"
QT_MOC_LITERAL(9, 97, 14), // "hasWampSession"
QT_MOC_LITERAL(10, 112, 14), // "Wamp::Session*"
QT_MOC_LITERAL(11, 127, 4), // "sess"
QT_MOC_LITERAL(12, 132, 10), // "errorOrder"
QT_MOC_LITERAL(13, 143, 5), // "j_itm"
QT_MOC_LITERAL(14, 149, 14), // "completedOrder"
QT_MOC_LITERAL(15, 164, 11), // "placedOrder"
QT_MOC_LITERAL(16, 176, 13), // "canceledOrder"
QT_MOC_LITERAL(17, 190, 13), // "loadDbWindows"
QT_MOC_LITERAL(18, 204, 19), // "calculateOrderbooks"
QT_MOC_LITERAL(19, 224, 11), // "reloadCache"
QT_MOC_LITERAL(20, 236, 14), // "setWampSession"
QT_MOC_LITERAL(21, 251, 12), // "onErrorOrder"
QT_MOC_LITERAL(22, 264, 16), // "onCompletedOrder"
QT_MOC_LITERAL(23, 281, 13), // "onPlacedOrder"
QT_MOC_LITERAL(24, 295, 15), // "onCanceledOrder"
QT_MOC_LITERAL(25, 311, 14), // "checkCandidats"
QT_MOC_LITERAL(26, 326, 7), // "sendLog"
QT_MOC_LITERAL(27, 334, 9) // "sendDebug"

    },
    "ArbitragePair\0debug\0\0msg\0log\0"
    "tocalculateOrderbooks\0"
    "QHash<quint32,QMap<double,double> >\0"
    "asks\0bids\0hasWampSession\0Wamp::Session*\0"
    "sess\0errorOrder\0j_itm\0completedOrder\0"
    "placedOrder\0canceledOrder\0loadDbWindows\0"
    "calculateOrderbooks\0reloadCache\0"
    "setWampSession\0onErrorOrder\0"
    "onCompletedOrder\0onPlacedOrder\0"
    "onCanceledOrder\0checkCandidats\0sendLog\0"
    "sendDebug"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArbitragePair[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,
       4,    1,  112,    2, 0x06 /* Public */,
       5,    2,  115,    2, 0x06 /* Public */,
       9,    1,  120,    2, 0x06 /* Public */,
      12,    1,  123,    2, 0x06 /* Public */,
      14,    1,  126,    2, 0x06 /* Public */,
      15,    1,  129,    2, 0x06 /* Public */,
      16,    1,  132,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,  135,    2, 0x0a /* Public */,
      18,    2,  136,    2, 0x0a /* Public */,
      19,    0,  141,    2, 0x0a /* Public */,
      20,    1,  142,    2, 0x0a /* Public */,
      21,    1,  145,    2, 0x0a /* Public */,
      22,    1,  148,    2, 0x0a /* Public */,
      23,    1,  151,    2, 0x0a /* Public */,
      24,    1,  154,    2, 0x0a /* Public */,
      25,    0,  157,    2, 0x0a /* Public */,
      26,    1,  158,    2, 0x0a /* Public */,
      27,    1,  161,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 6,    7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void, QMetaType::QJsonObject,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 6,    7,    8,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void, QMetaType::QJsonObject,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void ArbitragePair::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ArbitragePair *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->debug((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->tocalculateOrderbooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 3: _t->hasWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 4: _t->errorOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->completedOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->placedOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 7: _t->canceledOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 8: _t->loadDbWindows(); break;
        case 9: _t->calculateOrderbooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 10: _t->reloadCache(); break;
        case 11: _t->setWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 12: _t->onErrorOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 13: _t->onCompletedOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 14: _t->onPlacedOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 15: _t->onCanceledOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 16: _t->checkCandidats(); break;
        case 17: _t->sendLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->sendDebug((*reinterpret_cast< const QString(*)>(_a[1]))); break;
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
            using _t = void (ArbitragePair::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::debug)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ArbitragePair::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::log)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ArbitragePair::*)(const QHash<quint32,QMap<double,double>> & , const QHash<quint32,QMap<double,double>> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::tocalculateOrderbooks)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ArbitragePair::*)(Wamp::Session * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::hasWampSession)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ArbitragePair::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::errorOrder)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ArbitragePair::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::completedOrder)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ArbitragePair::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::placedOrder)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ArbitragePair::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ArbitragePair::canceledOrder)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ArbitragePair::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ArbitragePair.data,
    qt_meta_data_ArbitragePair,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ArbitragePair::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArbitragePair::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArbitragePair.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ArbitragePair::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void ArbitragePair::debug(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ArbitragePair::log(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ArbitragePair::tocalculateOrderbooks(const QHash<quint32,QMap<double,double>> & _t1, const QHash<quint32,QMap<double,double>> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ArbitragePair::hasWampSession(Wamp::Session * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ArbitragePair::errorOrder(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ArbitragePair::completedOrder(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ArbitragePair::placedOrder(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ArbitragePair::canceledOrder(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
