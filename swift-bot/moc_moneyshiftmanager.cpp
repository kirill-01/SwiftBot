/****************************************************************************
** Meta object code from reading C++ file 'moneyshiftmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "moneyshiftmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'moneyshiftmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MoneyShiftManager_t {
    QByteArrayData data[13];
    char stringdata0[138];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MoneyShiftManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MoneyShiftManager_t qt_meta_stringdata_MoneyShiftManager = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MoneyShiftManager"
QT_MOC_LITERAL(1, 18, 13), // "hasMoneyShift"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 6), // "amount"
QT_MOC_LITERAL(4, 40, 8), // "min_rate"
QT_MOC_LITERAL(5, 49, 11), // "is_positive"
QT_MOC_LITERAL(6, 61, 12), // "hasZeroShift"
QT_MOC_LITERAL(7, 74, 10), // "json_state"
QT_MOC_LITERAL(8, 85, 6), // "j_data"
QT_MOC_LITERAL(9, 92, 14), // "publishMessage"
QT_MOC_LITERAL(10, 107, 5), // "topic"
QT_MOC_LITERAL(11, 113, 7), // "message"
QT_MOC_LITERAL(12, 121, 16) // "loadPeriodOrders"

    },
    "MoneyShiftManager\0hasMoneyShift\0\0"
    "amount\0min_rate\0is_positive\0hasZeroShift\0"
    "json_state\0j_data\0publishMessage\0topic\0"
    "message\0loadPeriodOrders"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MoneyShiftManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,
       6,    0,   46,    2, 0x06 /* Public */,
       7,    1,   47,    2, 0x06 /* Public */,
       9,    2,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Bool,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,   10,   11,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MoneyShiftManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MoneyShiftManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->hasMoneyShift((*reinterpret_cast< const double(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 1: _t->hasZeroShift(); break;
        case 2: _t->json_state((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 4: _t->loadPeriodOrders(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MoneyShiftManager::*)(const double & , const double & , const bool & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MoneyShiftManager::hasMoneyShift)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MoneyShiftManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MoneyShiftManager::hasZeroShift)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MoneyShiftManager::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MoneyShiftManager::json_state)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MoneyShiftManager::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MoneyShiftManager::publishMessage)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MoneyShiftManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MoneyShiftManager.data,
    qt_meta_data_MoneyShiftManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MoneyShiftManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MoneyShiftManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MoneyShiftManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MoneyShiftManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MoneyShiftManager::hasMoneyShift(const double & _t1, const double & _t2, const bool & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MoneyShiftManager::hasZeroShift()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MoneyShiftManager::json_state(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MoneyShiftManager::publishMessage(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
