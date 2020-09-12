/****************************************************************************
** Meta object code from reading C++ file 'candidatsfilter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "candidatsfilter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'candidatsfilter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CandidatsFilter_t {
    QByteArrayData data[25];
    char stringdata0[322];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CandidatsFilter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CandidatsFilter_t qt_meta_stringdata_CandidatsFilter = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CandidatsFilter"
QT_MOC_LITERAL(1, 16, 14), // "readyCandidats"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 24), // "QList<ArbitrageCandidat>"
QT_MOC_LITERAL(4, 57, 9), // "candidats"
QT_MOC_LITERAL(5, 67, 14), // "publishMessage"
QT_MOC_LITERAL(6, 82, 5), // "topic"
QT_MOC_LITERAL(7, 88, 3), // "msg"
QT_MOC_LITERAL(8, 92, 15), // "placeShiftOrder"
QT_MOC_LITERAL(9, 108, 7), // "pair_id"
QT_MOC_LITERAL(10, 116, 6), // "amount"
QT_MOC_LITERAL(11, 123, 4), // "rate"
QT_MOC_LITERAL(12, 128, 4), // "type"
QT_MOC_LITERAL(13, 133, 21), // "sendArbitrageSnapshow"
QT_MOC_LITERAL(14, 155, 22), // "reportWindowsCollected"
QT_MOC_LITERAL(15, 178, 13), // "onWampSession"
QT_MOC_LITERAL(16, 192, 14), // "Wamp::Session*"
QT_MOC_LITERAL(17, 207, 7), // "session"
QT_MOC_LITERAL(18, 215, 12), // "onOrderBooks"
QT_MOC_LITERAL(19, 228, 35), // "QHash<quint32,QMap<double,dou..."
QT_MOC_LITERAL(20, 264, 4), // "asks"
QT_MOC_LITERAL(21, 269, 4), // "bids"
QT_MOC_LITERAL(22, 274, 18), // "onMoneyShiftExists"
QT_MOC_LITERAL(23, 293, 11), // "is_positive"
QT_MOC_LITERAL(24, 305, 16) // "onMoneyZeroShift"

    },
    "CandidatsFilter\0readyCandidats\0\0"
    "QList<ArbitrageCandidat>\0candidats\0"
    "publishMessage\0topic\0msg\0placeShiftOrder\0"
    "pair_id\0amount\0rate\0type\0sendArbitrageSnapshow\0"
    "reportWindowsCollected\0onWampSession\0"
    "Wamp::Session*\0session\0onOrderBooks\0"
    "QHash<quint32,QMap<double,double> >\0"
    "asks\0bids\0onMoneyShiftExists\0is_positive\0"
    "onMoneyZeroShift"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CandidatsFilter[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    2,   62,    2, 0x06 /* Public */,
       8,    4,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    0,   76,    2, 0x0a /* Public */,
      14,    0,   77,    2, 0x0a /* Public */,
      15,    1,   78,    2, 0x0a /* Public */,
      18,    2,   81,    2, 0x0a /* Public */,
      22,    3,   86,    2, 0x0a /* Public */,
      24,    0,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    6,    7,
    QMetaType::Void, QMetaType::UInt, QMetaType::Double, QMetaType::Double, QMetaType::UInt,    9,   10,   11,   12,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19, 0x80000000 | 19,   20,   21,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Bool,   10,   11,   23,
    QMetaType::Void,

       0        // eod
};

void CandidatsFilter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CandidatsFilter *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readyCandidats((*reinterpret_cast< const QList<ArbitrageCandidat>(*)>(_a[1]))); break;
        case 1: _t->publishMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QJsonObject(*)>(_a[2]))); break;
        case 2: _t->placeShiftOrder((*reinterpret_cast< const quint32(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2])),(*reinterpret_cast< const double(*)>(_a[3])),(*reinterpret_cast< const quint32(*)>(_a[4]))); break;
        case 3: _t->sendArbitrageSnapshow(); break;
        case 4: _t->reportWindowsCollected(); break;
        case 5: _t->onWampSession((*reinterpret_cast< Wamp::Session*(*)>(_a[1]))); break;
        case 6: _t->onOrderBooks((*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[1])),(*reinterpret_cast< const QHash<quint32,QMap<double,double> >(*)>(_a[2]))); break;
        case 7: _t->onMoneyShiftExists((*reinterpret_cast< const double(*)>(_a[1])),(*reinterpret_cast< const double(*)>(_a[2])),(*reinterpret_cast< const bool(*)>(_a[3]))); break;
        case 8: _t->onMoneyZeroShift(); break;
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
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CandidatsFilter::*)(const QList<ArbitrageCandidat> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CandidatsFilter::readyCandidats)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CandidatsFilter::*)(const QString & , const QJsonObject & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CandidatsFilter::publishMessage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CandidatsFilter::*)(const quint32 & , const double & , const double & , const quint32 & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CandidatsFilter::placeShiftOrder)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CandidatsFilter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CandidatsFilter.data,
    qt_meta_data_CandidatsFilter,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CandidatsFilter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CandidatsFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CandidatsFilter.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CandidatsFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CandidatsFilter::readyCandidats(const QList<ArbitrageCandidat> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CandidatsFilter::publishMessage(const QString & _t1, const QJsonObject & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CandidatsFilter::placeShiftOrder(const quint32 & _t1, const double & _t2, const double & _t3, const quint32 & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
