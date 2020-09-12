/****************************************************************************
** Meta object code from reading C++ file 'wampproxy.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "wampproxy.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'wampproxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WampProxy_t {
    QByteArrayData data[22];
    char stringdata0[279];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WampProxy_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WampProxy_t qt_meta_stringdata_WampProxy = {
    {
QT_MOC_LITERAL(0, 0, 9), // "WampProxy"
QT_MOC_LITERAL(1, 10, 10), // "orderbooks"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 4), // "vars"
QT_MOC_LITERAL(4, 27, 8), // "balances"
QT_MOC_LITERAL(5, 36, 3), // "obj"
QT_MOC_LITERAL(6, 40, 14), // "sessionStarted"
QT_MOC_LITERAL(7, 55, 14), // "Wamp::Session*"
QT_MOC_LITERAL(8, 70, 7), // "session"
QT_MOC_LITERAL(9, 78, 6), // "closed"
QT_MOC_LITERAL(10, 85, 16), // "subscrubeToAsync"
QT_MOC_LITERAL(11, 102, 17), // "onMoneyShiftState"
QT_MOC_LITERAL(12, 120, 7), // "j_state"
QT_MOC_LITERAL(13, 128, 19), // "onTransferCandidats"
QT_MOC_LITERAL(14, 148, 9), // "candidats"
QT_MOC_LITERAL(15, 158, 11), // "onUserState"
QT_MOC_LITERAL(16, 170, 19), // "onCandidatsFiltered"
QT_MOC_LITERAL(17, 190, 24), // "QList<ArbitrageCandidat>"
QT_MOC_LITERAL(18, 215, 19), // "_filtered_candidats"
QT_MOC_LITERAL(19, 235, 20), // "onWampSessionStarted"
QT_MOC_LITERAL(20, 256, 4), // "sess"
QT_MOC_LITERAL(21, 261, 17) // "sendReloadCommand"

    },
    "WampProxy\0orderbooks\0\0vars\0balances\0"
    "obj\0sessionStarted\0Wamp::Session*\0"
    "session\0closed\0subscrubeToAsync\0"
    "onMoneyShiftState\0j_state\0onTransferCandidats\0"
    "candidats\0onUserState\0onCandidatsFiltered\0"
    "QList<ArbitrageCandidat>\0_filtered_candidats\0"
    "onWampSessionStarted\0sess\0sendReloadCommand"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WampProxy[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,
       9,    0,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   79,    2, 0x0a /* Public */,
      11,    1,   80,    2, 0x0a /* Public */,
      13,    1,   83,    2, 0x0a /* Public */,
      15,    1,   86,    2, 0x0a /* Public */,
      16,    1,   89,    2, 0x0a /* Public */,
      19,    1,   92,    2, 0x0a /* Public */,
      21,    0,   95,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantList,    3,
    QMetaType::Void, QMetaType::QJsonObject,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,   12,
    QMetaType::Void, QMetaType::QJsonArray,   14,
    QMetaType::Void, QMetaType::QJsonObject,   12,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void, 0x80000000 | 7,   20,
    QMetaType::Void,

       0        // eod
};

void WampProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WampProxy *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->orderbooks((*reinterpret_cast< const QVariantList(*)>(_a[1]))); break;
        case 1: _t->balances((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 2: _t->sessionStarted((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 3: _t->closed(); break;
        case 4: _t->subscrubeToAsync(); break;
        case 5: _t->onMoneyShiftState((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 6: _t->onTransferCandidats((*reinterpret_cast< const QJsonArray(*)>(_a[1]))); break;
        case 7: _t->onUserState((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 8: _t->onCandidatsFiltered((*reinterpret_cast< const QList<ArbitrageCandidat>(*)>(_a[1]))); break;
        case 9: _t->onWampSessionStarted((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 10: _t->sendReloadCommand(); break;
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
        case 9:
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
            using _t = void (WampProxy::*)(const QVariantList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampProxy::orderbooks)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (WampProxy::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampProxy::balances)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (WampProxy::*)(Wamp::Session * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampProxy::sessionStarted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (WampProxy::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&WampProxy::closed)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WampProxy::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_WampProxy.data,
    qt_meta_data_WampProxy,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WampProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WampProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WampProxy.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int WampProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void WampProxy::orderbooks(const QVariantList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void WampProxy::balances(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void WampProxy::sessionStarted(Wamp::Session * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WampProxy::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
