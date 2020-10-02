/****************************************************************************
** Meta object code from reading C++ file 'arbitragewindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "arbitragewindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'arbitragewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ArbitrageWindow_t {
    QByteArrayData data[12];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ArbitrageWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ArbitrageWindow_t qt_meta_stringdata_ArbitrageWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ArbitrageWindow"
QT_MOC_LITERAL(1, 16, 16), // "onCompletedOrder"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "j_itm"
QT_MOC_LITERAL(4, 40, 13), // "onPlacedOrder"
QT_MOC_LITERAL(5, 54, 15), // "onCanceledOrder"
QT_MOC_LITERAL(6, 70, 13), // "checkIsActive"
QT_MOC_LITERAL(7, 84, 10), // "saveWindow"
QT_MOC_LITERAL(8, 95, 13), // "onWampSession"
QT_MOC_LITERAL(9, 109, 14), // "Wamp::Session*"
QT_MOC_LITERAL(10, 124, 4), // "sess"
QT_MOC_LITERAL(11, 129, 11) // "publishData"

    },
    "ArbitrageWindow\0onCompletedOrder\0\0"
    "j_itm\0onPlacedOrder\0onCanceledOrder\0"
    "checkIsActive\0saveWindow\0onWampSession\0"
    "Wamp::Session*\0sess\0publishData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArbitrageWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x0a /* Public */,
       4,    1,   52,    2, 0x0a /* Public */,
       5,    1,   55,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    1,   60,    2, 0x0a /* Public */,
      11,    0,   63,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

       0        // eod
};

void ArbitrageWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ArbitrageWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onCompletedOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->onPlacedOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 2: _t->onCanceledOrder((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->checkIsActive(); break;
        case 4: _t->saveWindow(); break;
        case 5: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 6: _t->publishData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< Wamp::Session* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ArbitrageWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ArbitrageWindow.data,
    qt_meta_data_ArbitrageWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ArbitrageWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArbitrageWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ArbitrageWindow.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ArbitrageWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
