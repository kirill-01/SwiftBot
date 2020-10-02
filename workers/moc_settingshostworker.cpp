/****************************************************************************
** Meta object code from reading C++ file 'settingshostworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "settingshostworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingshostworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SettingsHostWorker_t {
    QByteArrayData data[20];
    char stringdata0[251];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingsHostWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingsHostWorker_t qt_meta_stringdata_SettingsHostWorker = {
    {
QT_MOC_LITERAL(0, 0, 18), // "SettingsHostWorker"
QT_MOC_LITERAL(1, 19, 17), // "markMethodSuccess"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 6), // "method"
QT_MOC_LITERAL(4, 45, 16), // "markMethodFailed"
QT_MOC_LITERAL(5, 62, 8), // "newParam"
QT_MOC_LITERAL(6, 71, 5), // "param"
QT_MOC_LITERAL(7, 77, 5), // "value"
QT_MOC_LITERAL(8, 83, 11), // "getMemUsage"
QT_MOC_LITERAL(9, 95, 17), // "defineServiceMode"
QT_MOC_LITERAL(10, 113, 15), // "onMethodSuccess"
QT_MOC_LITERAL(11, 129, 14), // "onMethodFailed"
QT_MOC_LITERAL(12, 144, 10), // "onNewParam"
QT_MOC_LITERAL(13, 155, 10), // "loadParams"
QT_MOC_LITERAL(14, 166, 10), // "loadAssets"
QT_MOC_LITERAL(15, 177, 13), // "onWampSession"
QT_MOC_LITERAL(16, 191, 14), // "Wamp::Session*"
QT_MOC_LITERAL(17, 206, 1), // "s"
QT_MOC_LITERAL(18, 208, 22), // "getWithdrawFeesFromApi"
QT_MOC_LITERAL(19, 231, 19) // "getTradeFeesFromApi"

    },
    "SettingsHostWorker\0markMethodSuccess\0"
    "\0method\0markMethodFailed\0newParam\0"
    "param\0value\0getMemUsage\0defineServiceMode\0"
    "onMethodSuccess\0onMethodFailed\0"
    "onNewParam\0loadParams\0loadAssets\0"
    "onWampSession\0Wamp::Session*\0s\0"
    "getWithdrawFeesFromApi\0getTradeFeesFromApi"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingsHostWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       5,    2,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   90,    2, 0x0a /* Public */,
       9,    0,   91,    2, 0x0a /* Public */,
      10,    1,   92,    2, 0x0a /* Public */,
      11,    1,   95,    2, 0x0a /* Public */,
      12,    2,   98,    2, 0x0a /* Public */,
      13,    0,  103,    2, 0x0a /* Public */,
      14,    0,  104,    2, 0x0a /* Public */,
      15,    1,  105,    2, 0x0a /* Public */,
      18,    0,  108,    2, 0x0a /* Public */,
      19,    0,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,

 // slots: parameters
    QMetaType::QJsonObject,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SettingsHostWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingsHostWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markMethodSuccess((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->markMethodFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->newParam((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: { QJsonObject _r = _t->getMemUsage();
            if (_a[0]) *reinterpret_cast< QJsonObject*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->defineServiceMode(); break;
        case 5: _t->onMethodSuccess((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onMethodFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->onNewParam((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->loadParams(); break;
        case 9: _t->loadAssets(); break;
        case 10: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 11: _t->getWithdrawFeesFromApi(); break;
        case 12: _t->getTradeFeesFromApi(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
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
            using _t = void (SettingsHostWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsHostWorker::markMethodSuccess)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SettingsHostWorker::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsHostWorker::markMethodFailed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SettingsHostWorker::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsHostWorker::newParam)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SettingsHostWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SettingsHostWorker.data,
    qt_meta_data_SettingsHostWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SettingsHostWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsHostWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsHostWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SettingsHostWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void SettingsHostWorker::markMethodSuccess(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SettingsHostWorker::markMethodFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SettingsHostWorker::newParam(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
