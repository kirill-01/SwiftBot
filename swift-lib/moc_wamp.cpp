/****************************************************************************
** Meta object code from reading C++ file 'wamp.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "wamp.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wamp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Wamp__Session_t {
    QByteArrayData data[26];
    char stringdata0[240];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Wamp__Session_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Wamp__Session_t qt_meta_stringdata_Wamp__Session = {
    {
QT_MOC_LITERAL(0, 0, 13), // "Wamp::Session"
QT_MOC_LITERAL(1, 14, 7), // "started"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 6), // "joined"
QT_MOC_LITERAL(4, 30, 4), // "left"
QT_MOC_LITERAL(5, 35, 10), // "subscribed"
QT_MOC_LITERAL(6, 46, 12), // "Subscription"
QT_MOC_LITERAL(7, 59, 10), // "registered"
QT_MOC_LITERAL(8, 70, 12), // "Registration"
QT_MOC_LITERAL(9, 83, 9), // "challenge"
QT_MOC_LITERAL(10, 93, 9), // "Challenge"
QT_MOC_LITERAL(11, 103, 14), // "publishMessage"
QT_MOC_LITERAL(12, 118, 5), // "topic"
QT_MOC_LITERAL(13, 124, 5), // "j_msg"
QT_MOC_LITERAL(14, 130, 8), // "readData"
QT_MOC_LITERAL(15, 139, 11), // "readMessage"
QT_MOC_LITERAL(16, 151, 7), // "message"
QT_MOC_LITERAL(17, 159, 7), // "publish"
QT_MOC_LITERAL(18, 167, 4), // "args"
QT_MOC_LITERAL(19, 172, 6), // "kwargs"
QT_MOC_LITERAL(20, 179, 13), // "MessageFormat"
QT_MOC_LITERAL(21, 193, 7), // "Msgpack"
QT_MOC_LITERAL(22, 201, 4), // "Json"
QT_MOC_LITERAL(23, 206, 13), // "TransportType"
QT_MOC_LITERAL(24, 220, 9), // "WebSocket"
QT_MOC_LITERAL(25, 230, 9) // "RawSocket"

    },
    "Wamp::Session\0started\0\0joined\0left\0"
    "subscribed\0Subscription\0registered\0"
    "Registration\0challenge\0Challenge\0"
    "publishMessage\0topic\0j_msg\0readData\0"
    "readMessage\0message\0publish\0args\0"
    "kwargs\0MessageFormat\0Msgpack\0Json\0"
    "TransportType\0WebSocket\0RawSocket"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Wamp__Session[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       2,  118, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    1,   75,    2, 0x06 /* Public */,
       4,    1,   78,    2, 0x06 /* Public */,
       5,    1,   81,    2, 0x06 /* Public */,
       7,    1,   84,    2, 0x06 /* Public */,
       9,    3,   87,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    2,   94,    2, 0x0a /* Public */,
      14,    0,   99,    2, 0x08 /* Private */,
      15,    1,  100,    2, 0x08 /* Private */,
      17,    3,  103,    2, 0x0a /* Public */,
      17,    2,  110,    2, 0x2a /* Public | MethodCloned */,
      17,    1,  115,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::ULongLong,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 6,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 10,    2,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,   12,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantList, QMetaType::QVariantMap,   12,   18,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantList,   12,   18,
    QMetaType::Void, QMetaType::QString,   12,

 // enums: name, alias, flags, count, data
      20,   20, 0x2,    2,  128,
      23,   23, 0x2,    2,  132,

 // enum data: key, value
      21, uint(Wamp::Session::MessageFormat::Msgpack),
      22, uint(Wamp::Session::MessageFormat::Json),
      24, uint(Wamp::Session::TransportType::WebSocket),
      25, uint(Wamp::Session::TransportType::RawSocket),

       0        // eod
};

void Wamp::Session::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Session *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->started(); break;
        case 1: _t->joined((*reinterpret_cast< quint64(*)>(_a[1]))); break;
        case 2: _t->left((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->subscribed((*reinterpret_cast< Subscription(*)>(_a[1]))); break;
        case 4: _t->registered((*reinterpret_cast< Registration(*)>(_a[1]))); break;
        case 5: _t->challenge((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const Challenge(*)>(_a[3]))); break;
        case 6: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 7: _t->readData(); break;
        case 8: _t->readMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->publish((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariantList(*)>(_a[2])),(*reinterpret_cast< const QVariantMap(*)>(_a[3]))); break;
        case 10: _t->publish((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariantList(*)>(_a[2]))); break;
        case 11: _t->publish((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Session::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Session::started)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Session::*)(quint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Session::joined)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Session::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Session::left)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Session::*)(Subscription );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Session::subscribed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Session::*)(Registration );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Session::registered)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Session::*)(const QString & , const QString & , const Challenge & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Session::challenge)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Wamp::Session::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Wamp__Session.data,
    qt_meta_data_Wamp__Session,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Wamp::Session::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Wamp::Session::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Wamp__Session.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Wamp::Session::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void Wamp::Session::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Wamp::Session::joined(quint64 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Wamp::Session::left(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Wamp::Session::subscribed(Subscription _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Wamp::Session::registered(Registration _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Wamp::Session::challenge(const QString & _t1, const QString & _t2, const Challenge & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
