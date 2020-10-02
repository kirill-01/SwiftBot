/****************************************************************************
** Meta object code from reading C++ file 'balanceworker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "balanceworker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'balanceworker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BalanceWorker_t {
    QByteArrayData data[21];
    char stringdata0[282];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BalanceWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BalanceWorker_t qt_meta_stringdata_BalanceWorker = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BalanceWorker"
QT_MOC_LITERAL(1, 14, 11), // "eventUpdate"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 4), // "data"
QT_MOC_LITERAL(4, 32, 12), // "eventDeposit"
QT_MOC_LITERAL(5, 45, 13), // "eventDeposits"
QT_MOC_LITERAL(6, 59, 13), // "eventWithdraw"
QT_MOC_LITERAL(7, 73, 14), // "eventWithdraws"
QT_MOC_LITERAL(8, 88, 15), // "requestBalances"
QT_MOC_LITERAL(9, 104, 21), // "processWithdrawsQueue"
QT_MOC_LITERAL(10, 126, 20), // "processBalancesQueue"
QT_MOC_LITERAL(11, 147, 20), // "processDepositsQueue"
QT_MOC_LITERAL(12, 168, 16), // "requestWithdraws"
QT_MOC_LITERAL(13, 185, 15), // "requestDeposits"
QT_MOC_LITERAL(14, 201, 8), // "onUpdate"
QT_MOC_LITERAL(15, 210, 11), // "updateRates"
QT_MOC_LITERAL(16, 222, 15), // "updateRequested"
QT_MOC_LITERAL(17, 238, 9), // "onDeposit"
QT_MOC_LITERAL(18, 248, 10), // "onDeposits"
QT_MOC_LITERAL(19, 259, 10), // "onWithdraw"
QT_MOC_LITERAL(20, 270, 11) // "onWithdraws"

    },
    "BalanceWorker\0eventUpdate\0\0data\0"
    "eventDeposit\0eventDeposits\0eventWithdraw\0"
    "eventWithdraws\0requestBalances\0"
    "processWithdrawsQueue\0processBalancesQueue\0"
    "processDepositsQueue\0requestWithdraws\0"
    "requestDeposits\0onUpdate\0updateRates\0"
    "updateRequested\0onDeposit\0onDeposits\0"
    "onWithdraw\0onWithdraws"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BalanceWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,
       4,    1,  107,    2, 0x06 /* Public */,
       5,    1,  110,    2, 0x06 /* Public */,
       6,    1,  113,    2, 0x06 /* Public */,
       7,    1,  116,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  119,    2, 0x0a /* Public */,
       9,    0,  120,    2, 0x0a /* Public */,
      10,    0,  121,    2, 0x0a /* Public */,
      11,    0,  122,    2, 0x0a /* Public */,
      12,    0,  123,    2, 0x0a /* Public */,
      13,    0,  124,    2, 0x0a /* Public */,
      14,    1,  125,    2, 0x0a /* Public */,
      15,    0,  128,    2, 0x0a /* Public */,
      16,    0,  129,    2, 0x0a /* Public */,
      17,    1,  130,    2, 0x0a /* Public */,
      18,    1,  133,    2, 0x0a /* Public */,
      19,    1,  136,    2, 0x0a /* Public */,
      20,    1,  139,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,
    QMetaType::Void, QMetaType::QJsonObject,    3,

       0        // eod
};

void BalanceWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BalanceWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->eventUpdate((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 1: _t->eventDeposit((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 2: _t->eventDeposits((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 3: _t->eventWithdraw((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 4: _t->eventWithdraws((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 5: _t->requestBalances(); break;
        case 6: _t->processWithdrawsQueue(); break;
        case 7: _t->processBalancesQueue(); break;
        case 8: _t->processDepositsQueue(); break;
        case 9: _t->requestWithdraws(); break;
        case 10: _t->requestDeposits(); break;
        case 11: _t->onUpdate((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 12: _t->updateRates(); break;
        case 13: _t->updateRequested(); break;
        case 14: _t->onDeposit((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 15: _t->onDeposits((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 16: _t->onWithdraw((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        case 17: _t->onWithdraws((*reinterpret_cast< const QJsonObject(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BalanceWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalanceWorker::eventUpdate)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BalanceWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalanceWorker::eventDeposit)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BalanceWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalanceWorker::eventDeposits)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BalanceWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalanceWorker::eventWithdraw)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BalanceWorker::*)(const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BalanceWorker::eventWithdraws)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BalanceWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<SwiftWorker::staticMetaObject>(),
    qt_meta_stringdata_BalanceWorker.data,
    qt_meta_data_BalanceWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BalanceWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BalanceWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BalanceWorker.stringdata0))
        return static_cast<void*>(this);
    return SwiftWorker::qt_metacast(_clname);
}

int BalanceWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SwiftWorker::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void BalanceWorker::eventUpdate(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BalanceWorker::eventDeposit(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BalanceWorker::eventDeposits(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BalanceWorker::eventWithdraw(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BalanceWorker::eventWithdraws(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
