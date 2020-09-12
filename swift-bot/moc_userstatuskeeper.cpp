/****************************************************************************
** Meta object code from reading C++ file 'userstatuskeeper.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "userstatuskeeper.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'userstatuskeeper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UserStatusKeeper_t {
    QByteArrayData data[17];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UserStatusKeeper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UserStatusKeeper_t qt_meta_stringdata_UserStatusKeeper = {
    {
QT_MOC_LITERAL(0, 0, 16), // "UserStatusKeeper"
QT_MOC_LITERAL(1, 17, 9), // "userState"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "userstat"
QT_MOC_LITERAL(4, 37, 8), // "balances"
QT_MOC_LITERAL(5, 46, 21), // "QHash<quint32,double>"
QT_MOC_LITERAL(6, 68, 4), // "bals"
QT_MOC_LITERAL(7, 73, 14), // "publishMessage"
QT_MOC_LITERAL(8, 88, 5), // "topic"
QT_MOC_LITERAL(9, 94, 3), // "msg"
QT_MOC_LITERAL(10, 98, 13), // "onWampSession"
QT_MOC_LITERAL(11, 112, 14), // "Wamp::Session*"
QT_MOC_LITERAL(12, 127, 7), // "session"
QT_MOC_LITERAL(13, 135, 13), // "sendUserState"
QT_MOC_LITERAL(14, 149, 13), // "onCurrentRate"
QT_MOC_LITERAL(15, 163, 4), // "rate"
QT_MOC_LITERAL(16, 168, 13) // "onOrderPlaced"

    },
    "UserStatusKeeper\0userState\0\0userstat\0"
    "balances\0QHash<quint32,double>\0bals\0"
    "publishMessage\0topic\0msg\0onWampSession\0"
    "Wamp::Session*\0session\0sendUserState\0"
    "onCurrentRate\0rate\0onOrderPlaced"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UserStatusKeeper[] = {

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
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       7,    2,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   60,    2, 0x0a /* Public */,
      13,    0,   63,    2, 0x0a /* Public */,
      14,    1,   64,    2, 0x0a /* Public */,
      16,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    8,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   15,
    QMetaType::Void,

       0        // eod
};

void UserStatusKeeper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UserStatusKeeper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->userState((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->balances((*reinterpret_cast< const QHash<quint32,double>(*)>(_a[1]))); break;
        case 2: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 3: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 4: _t->sendUserState(); break;
        case 5: _t->onCurrentRate((*reinterpret_cast< const double(*)>(_a[1]))); break;
        case 6: _t->onOrderPlaced(); break;
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
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UserStatusKeeper::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserStatusKeeper::userState)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UserStatusKeeper::*)(const QHash<quint32,double> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserStatusKeeper::balances)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UserStatusKeeper::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UserStatusKeeper::publishMessage)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UserStatusKeeper::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UserStatusKeeper.data,
    qt_meta_data_UserStatusKeeper,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UserStatusKeeper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UserStatusKeeper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UserStatusKeeper.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UserStatusKeeper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void UserStatusKeeper::userState(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UserStatusKeeper::balances(const QHash<quint32,double> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UserStatusKeeper::publishMessage(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
