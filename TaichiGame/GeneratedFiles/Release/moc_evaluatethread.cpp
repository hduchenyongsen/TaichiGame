/****************************************************************************
** Meta object code from reading C++ file 'evaluatethread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../evaluatethread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'evaluatethread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EvaluateThread_t {
    QByteArrayData data[12];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EvaluateThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EvaluateThread_t qt_meta_stringdata_EvaluateThread = {
    {
QT_MOC_LITERAL(0, 0, 14), // "EvaluateThread"
QT_MOC_LITERAL(1, 15, 15), // "SigCurrentScore"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "score"
QT_MOC_LITERAL(4, 38, 29), // "EvaluateThread::MatchPosition"
QT_MOC_LITERAL(5, 68, 3), // "pos"
QT_MOC_LITERAL(6, 72, 9), // "OnGetData"
QT_MOC_LITERAL(7, 82, 6), // "_index"
QT_MOC_LITERAL(8, 89, 18), // "QList<QList<int> >"
QT_MOC_LITERAL(9, 108, 4), // "data"
QT_MOC_LITERAL(10, 113, 11), // "OnTerminate"
QT_MOC_LITERAL(11, 125, 6) // "OnOver"

    },
    "EvaluateThread\0SigCurrentScore\0\0score\0"
    "EvaluateThread::MatchPosition\0pos\0"
    "OnGetData\0_index\0QList<QList<int> >\0"
    "data\0OnTerminate\0OnOver"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EvaluateThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   39,    2, 0x0a /* Public */,
      10,    0,   44,    2, 0x0a /* Public */,
      11,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 8,    7,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void EvaluateThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EvaluateThread *_t = static_cast<EvaluateThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SigCurrentScore((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< EvaluateThread::MatchPosition(*)>(_a[2]))); break;
        case 1: _t->OnGetData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QList<QList<int> >(*)>(_a[2]))); break;
        case 2: _t->OnTerminate(); break;
        case 3: _t->OnOver(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QList<int> > >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EvaluateThread::*_t)(int , EvaluateThread::MatchPosition );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EvaluateThread::SigCurrentScore)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject EvaluateThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_EvaluateThread.data,
      qt_meta_data_EvaluateThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *EvaluateThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EvaluateThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EvaluateThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int EvaluateThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void EvaluateThread::SigCurrentScore(int _t1, EvaluateThread::MatchPosition _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
