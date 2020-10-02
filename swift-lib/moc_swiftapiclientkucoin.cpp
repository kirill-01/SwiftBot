/****************************************************************************
** Meta object code from reading C++ file 'swiftapiclientkucoin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "swiftapiclientkucoin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'swiftapiclientkucoin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SwiftApiClientKuCoin_t {
    QByteArrayData data[7];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SwiftApiClientKuCoin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SwiftApiClientKuCoin_t qt_meta_stringdata_SwiftApiClientKuCoin = {
    {
QT_MOC_LITERAL(0, 0, 20), // "SwiftApiClientKuCoin"
QT_MOC_LITERAL(1, 21, 16), // "transferToTrades"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 22), // "parseCustomApiResponse"
QT_MOC_LITERAL(4, 62, 6), // "method"
QT_MOC_LITERAL(5, 69, 4), // "data"
QT_MOC_LITERAL(6, 74, 14) // "syncTimestamps"

    },
    "SwiftApiClientKuCoin\0transferToTrades\0"
    "\0parseCustomApiResponse\0method\0data\0"
    "syncTimestamps"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SwiftApiClientKuCoin[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    2,   30,    2, 0x0a /* Public */,
       6,    0,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QByteArray,    4,    5,
    QMetaType::Void,

       0        // eod
};

void SwiftApiClientKuCoin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SwiftApiClientKuCoin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->transferToTrades(); break;
        case 1: _t->parseCustomApiResponse((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 2: _t->syncTimestamps(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SwiftApiClientKuCoin::staticMetaObject = { {
    QMetaObject::SuperData::link<SwiftApiClient::staticMetaObject>(),
    qt_meta_stringdata_SwiftApiClientKuCoin.data,
    qt_meta_data_SwiftApiClientKuCoin,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SwiftApiClientKuCoin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwiftApiClientKuCoin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SwiftApiClientKuCoin.stringdata0))
        return static_cast<void*>(this);
    return SwiftApiClient::qt_metacast(_clname);
}

int SwiftApiClientKuCoin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SwiftApiClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
