/****************************************************************************
** Meta object code from reading C++ file 'swiftapiparsercexio.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "swiftapiparsercexio.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'swiftapiparsercexio.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SwiftApiParserCexio_t {
    QByteArrayData data[7];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SwiftApiParserCexio_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SwiftApiParserCexio_t qt_meta_stringdata_SwiftApiParserCexio = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SwiftApiParserCexio"
QT_MOC_LITERAL(1, 20, 13), // "parseResponse"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 4), // "uuid"
QT_MOC_LITERAL(4, 40, 28), // "SwiftApiClient::AsyncMethods"
QT_MOC_LITERAL(5, 69, 6), // "method"
QT_MOC_LITERAL(6, 76, 4) // "data"

    },
    "SwiftApiParserCexio\0parseResponse\0\0"
    "uuid\0SwiftApiClient::AsyncMethods\0"
    "method\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SwiftApiParserCexio[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::ULongLong, 0x80000000 | 4, QMetaType::QByteArray,    3,    5,    6,

       0        // eod
};

void SwiftApiParserCexio::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SwiftApiParserCexio *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->parseResponse((*reinterpret_cast< const quint64(*)>(_a[1])),(*reinterpret_cast< const SwiftApiClient::AsyncMethods(*)>(_a[2])),(*reinterpret_cast< const QByteArray(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SwiftApiParserCexio::staticMetaObject = { {
    QMetaObject::SuperData::link<SwiftApiParser::staticMetaObject>(),
    qt_meta_stringdata_SwiftApiParserCexio.data,
    qt_meta_data_SwiftApiParserCexio,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SwiftApiParserCexio::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SwiftApiParserCexio::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SwiftApiParserCexio.stringdata0))
        return static_cast<void*>(this);
    return SwiftApiParser::qt_metacast(_clname);
}

int SwiftApiParserCexio::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SwiftApiParser::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
