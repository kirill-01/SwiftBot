/****************************************************************************
** Meta object code from reading C++ file 'wampclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "wampclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wampclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WampClient_t {
    QByteArrayData data[27];
    char stringdata0[296];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WampClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WampClient_t qt_meta_stringdata_WampClient = {
    {
QT_MOC_LITERAL(0, 0, 10), // "WampClient"
QT_MOC_LITERAL(1, 11, 15), // "clientConnected"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "Wamp::Session*"
QT_MOC_LITERAL(4, 43, 7), // "session"
QT_MOC_LITERAL(5, 51, 17), // "clientdiconnected"
QT_MOC_LITERAL(6, 69, 12), // "clientJoined"
QT_MOC_LITERAL(7, 82, 11), // "asyncResult"
QT_MOC_LITERAL(8, 94, 6), // "result"
QT_MOC_LITERAL(9, 101, 8), // "isExited"
QT_MOC_LITERAL(10, 110, 22), // "orderbooksDataReceived"
QT_MOC_LITERAL(11, 133, 5), // "jdata"
QT_MOC_LITERAL(12, 139, 15), // "messageReceived"
QT_MOC_LITERAL(13, 155, 5), // "topic"
QT_MOC_LITERAL(14, 161, 7), // "message"
QT_MOC_LITERAL(15, 169, 10), // "systemText"
QT_MOC_LITERAL(16, 180, 4), // "text"
QT_MOC_LITERAL(17, 185, 12), // "connectRealm"
QT_MOC_LITERAL(18, 198, 9), // "realmname"
QT_MOC_LITERAL(19, 208, 14), // "publishMessage"
QT_MOC_LITERAL(20, 223, 1), // "j"
QT_MOC_LITERAL(21, 225, 11), // "startClient"
QT_MOC_LITERAL(22, 237, 8), // "onJoined"
QT_MOC_LITERAL(23, 246, 11), // "onConnected"
QT_MOC_LITERAL(24, 258, 14), // "onDisconnected"
QT_MOC_LITERAL(25, 273, 5), // "realm"
QT_MOC_LITERAL(26, 279, 16) // "closeConnections"

    },
    "WampClient\0clientConnected\0\0Wamp::Session*\0"
    "session\0clientdiconnected\0clientJoined\0"
    "asyncResult\0result\0isExited\0"
    "orderbooksDataReceived\0jdata\0"
    "messageReceived\0topic\0message\0systemText\0"
    "text\0connectRealm\0realmname\0publishMessage\0"
    "j\0startClient\0onJoined\0onConnected\0"
    "onDisconnected\0realm\0closeConnections"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WampClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   89,    2, 0x06 /* Public */,
       5,    0,   92,    2, 0x06 /* Public */,
       6,    0,   93,    2, 0x06 /* Public */,
       7,    1,   94,    2, 0x06 /* Public */,
       9,    0,   97,    2, 0x06 /* Public */,
      10,    1,   98,    2, 0x06 /* Public */,
      12,    2,  101,    2, 0x06 /* Public */,
      15,    1,  106,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    1,  109,    2, 0x0a /* Public */,
      19,    2,  112,    2, 0x0a /* Public */,
      21,    0,  117,    2, 0x0a /* Public */,
      22,    0,  118,    2, 0x0a /* Public */,
      23,    0,  119,    2, 0x0a /* Public */,
      24,    1,  120,    2, 0x0a /* Public */,
      26,    0,  123,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   13,   14,
    QMetaType::Void, QMetaType::QString,   16,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,   13,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void,

       0        // eod
};

void WampClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WampClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clientConnected((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 1: _t->clientdiconnected(); break;
        case 2: _t->clientJoined(); break;
        case 3: _t->asyncResult((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->isExited(); break;
        case 5: _t->orderbooksDataReceived((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->systemText((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->connectRealm((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 10: _t->startClient(); break;
        case 11: _t->onJoined(); break;
        case 12: _t->onConnected(); break;
        case 13: _t->onDisconnected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->closeConnections(); break;
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
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (WampClient::*)(Wamp::Session * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::clientConnected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WampClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::clientdiconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WampClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::clientJoined)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WampClient::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::asyncResult)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (WampClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::isExited)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (WampClient::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::orderbooksDataReceived)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (WampClient::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::messageReceived)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (WampClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampClient::systemText)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WampClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_WampClient.data,
    qt_meta_data_WampClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WampClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WampClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WampClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WampClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void WampClient::clientConnected(Wamp::Session * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WampClient::clientdiconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void WampClient::clientJoined()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void WampClient::asyncResult(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void WampClient::isExited()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void WampClient::orderbooksDataReceived(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void WampClient::messageReceived(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void WampClient::systemText(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
