/****************************************************************************
** Meta object code from reading C++ file 'settingshostcomponent.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "settingshostcomponent.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingshostcomponent.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SettingsHostComponent_t {
    QByteArrayData data[9];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingsHostComponent_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingsHostComponent_t qt_meta_stringdata_SettingsHostComponent = {
    {
QT_MOC_LITERAL(0, 0, 21), // "SettingsHostComponent"
QT_MOC_LITERAL(1, 22, 26), // "componentShutdownRequested"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 23), // "componentStartRequested"
QT_MOC_LITERAL(4, 74, 7), // "logInfo"
QT_MOC_LITERAL(5, 82, 7), // "message"
QT_MOC_LITERAL(6, 90, 8), // "logError"
QT_MOC_LITERAL(7, 99, 13), // "sendHeartBeat"
QT_MOC_LITERAL(8, 113, 18) // "registerRpcMethods"

    },
    "SettingsHostComponent\0componentShutdownRequested\0"
    "\0componentStartRequested\0logInfo\0"
    "message\0logError\0sendHeartBeat\0"
    "registerRpcMethods"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingsHostComponent[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   46,    2, 0x0a /* Public */,
       6,    1,   49,    2, 0x0a /* Public */,
       7,    0,   52,    2, 0x0a /* Public */,
       8,    0,   53,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SettingsHostComponent::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingsHostComponent *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->componentShutdownRequested(); break;
        case 1: _t->componentStartRequested(); break;
        case 2: _t->logInfo((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->logError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->sendHeartBeat(); break;
        case 5: _t->registerRpcMethods(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SettingsHostComponent::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsHostComponent::componentShutdownRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SettingsHostComponent::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsHostComponent::componentStartRequested)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SettingsHostComponent::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_SettingsHostComponent.data,
    qt_meta_data_SettingsHostComponent,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SettingsHostComponent::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsHostComponent::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsHostComponent.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SettingsHostComponent::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SettingsHostComponent::componentShutdownRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SettingsHostComponent::componentStartRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
