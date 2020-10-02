/****************************************************************************
** Meta object code from reading C++ file 'nodescontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "nodescontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nodescontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NodesController_t {
    QByteArrayData data[27];
    char stringdata0[330];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NodesController_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NodesController_t qt_meta_stringdata_NodesController = {
    {
QT_MOC_LITERAL(0, 0, 15), // "NodesController"
QT_MOC_LITERAL(1, 16, 10), // "startNodes"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 17), // "QThread::Priority"
QT_MOC_LITERAL(4, 46, 9), // "stopNodes"
QT_MOC_LITERAL(5, 56, 21), // "orderbooksAccumulated"
QT_MOC_LITERAL(6, 78, 5), // "topic"
QT_MOC_LITERAL(7, 84, 6), // "j_stat"
QT_MOC_LITERAL(8, 91, 12), // "summaryStats"
QT_MOC_LITERAL(9, 104, 12), // "clientExited"
QT_MOC_LITERAL(10, 117, 9), // "pairsRate"
QT_MOC_LITERAL(11, 127, 20), // "QMap<quint32,double>"
QT_MOC_LITERAL(12, 148, 5), // "rates"
QT_MOC_LITERAL(13, 154, 9), // "sendRates"
QT_MOC_LITERAL(14, 164, 17), // "nodeThreadStarted"
QT_MOC_LITERAL(15, 182, 18), // "nodeThreadFinished"
QT_MOC_LITERAL(16, 201, 13), // "startAllNodes"
QT_MOC_LITERAL(17, 215, 12), // "onOrderBooks"
QT_MOC_LITERAL(18, 228, 1), // "a"
QT_MOC_LITERAL(19, 230, 1), // "b"
QT_MOC_LITERAL(20, 232, 11), // "onNodeError"
QT_MOC_LITERAL(21, 244, 26), // "OrderbooksNode::NodeErrors"
QT_MOC_LITERAL(22, 271, 3), // "err"
QT_MOC_LITERAL(23, 275, 17), // "findOutdatedPairs"
QT_MOC_LITERAL(24, 293, 11), // "secStatCalc"
QT_MOC_LITERAL(25, 305, 9), // "sendStats"
QT_MOC_LITERAL(26, 315, 14) // "sendOrderbooks"

    },
    "NodesController\0startNodes\0\0"
    "QThread::Priority\0stopNodes\0"
    "orderbooksAccumulated\0topic\0j_stat\0"
    "summaryStats\0clientExited\0pairsRate\0"
    "QMap<quint32,double>\0rates\0sendRates\0"
    "nodeThreadStarted\0nodeThreadFinished\0"
    "startAllNodes\0onOrderBooks\0a\0b\0"
    "onNodeError\0OrderbooksNode::NodeErrors\0"
    "err\0findOutdatedPairs\0secStatCalc\0"
    "sendStats\0sendOrderbooks"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NodesController[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       1,    0,  102,    2, 0x26 /* Public | MethodCloned */,
       4,    0,  103,    2, 0x06 /* Public */,
       5,    2,  104,    2, 0x06 /* Public */,
       8,    2,  109,    2, 0x06 /* Public */,
       9,    0,  114,    2, 0x06 /* Public */,
      10,    1,  115,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,  118,    2, 0x0a /* Public */,
      14,    0,  119,    2, 0x0a /* Public */,
      15,    0,  120,    2, 0x0a /* Public */,
      16,    0,  121,    2, 0x0a /* Public */,
      17,    2,  122,    2, 0x0a /* Public */,
      20,    1,  127,    2, 0x0a /* Public */,
      23,    0,  130,    2, 0x0a /* Public */,
      24,    0,  131,    2, 0x0a /* Public */,
      25,    0,  132,    2, 0x0a /* Public */,
      26,    0,  133,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    6,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonArray, QMetaType::QJsonArray,   18,   19,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NodesController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NodesController *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startNodes((*reinterpret_cast< QThread::Priority(*)>(_a[1]))); break;
        case 1: _t->startNodes(); break;
        case 2: _t->stopNodes(); break;
        case 3: _t->orderbooksAccumulated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 4: _t->summaryStats((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 5: _t->clientExited(); break;
        case 6: _t->pairsRate((*reinterpret_cast< const QMap<quint32,double>(*)>(_a[1]))); break;
        case 7: _t->sendRates(); break;
        case 8: _t->nodeThreadStarted(); break;
        case 9: _t->nodeThreadFinished(); break;
        case 10: _t->startAllNodes(); break;
        case 11: _t->onOrderBooks((*reinterpret_cast< const QJsonArray(*)>(_a[1])),(*reinterpret_cast< const QJsonArray(*)>(_a[2]))); break;
        case 12: _t->onNodeError((*reinterpret_cast< OrderbooksNode::NodeErrors(*)>(_a[1]))); break;
        case 13: _t->findOutdatedPairs(); break;
        case 14: _t->secStatCalc(); break;
        case 15: _t->sendStats(); break;
        case 16: _t->sendOrderbooks(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NodesController::*)(QThread::Priority );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodesController::startNodes)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NodesController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodesController::stopNodes)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NodesController::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodesController::orderbooksAccumulated)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NodesController::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodesController::summaryStats)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NodesController::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodesController::clientExited)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NodesController::*)(const QMap<quint32,double> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NodesController::pairsRate)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NodesController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NodesController.data,
    qt_meta_data_NodesController,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NodesController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NodesController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NodesController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NodesController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void NodesController::startNodes(QThread::Priority _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void NodesController::stopNodes()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void NodesController::orderbooksAccumulated(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NodesController::summaryStats(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NodesController::clientExited()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void NodesController::pairsRate(const QMap<quint32,double> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
