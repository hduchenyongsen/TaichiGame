/****************************************************************************
** Meta object code from reading C++ file 'playbackrecordthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../playbackrecordthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playbackrecordthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PlayBackRecordThread_t {
    QByteArrayData data[5];
    char stringdata0[53];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PlayBackRecordThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PlayBackRecordThread_t qt_meta_stringdata_PlayBackRecordThread = {
    {
QT_MOC_LITERAL(0, 0, 20), // "PlayBackRecordThread"
QT_MOC_LITERAL(1, 21, 18), // "OnRecieveFrameData"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 5), // "BYTE*"
QT_MOC_LITERAL(4, 47, 5) // "frame"

    },
    "PlayBackRecordThread\0OnRecieveFrameData\0"
    "\0BYTE*\0frame"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PlayBackRecordThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void PlayBackRecordThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PlayBackRecordThread *_t = static_cast<PlayBackRecordThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnRecieveFrameData((*reinterpret_cast< BYTE*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PlayBackRecordThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_PlayBackRecordThread.data,
      qt_meta_data_PlayBackRecordThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PlayBackRecordThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PlayBackRecordThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PlayBackRecordThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int PlayBackRecordThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
