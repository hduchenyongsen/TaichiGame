/****************************************************************************
** Meta object code from reading C++ file 'taichigame.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../taichigame.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'taichigame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TaichiGame_t {
    QByteArrayData data[22];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TaichiGame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TaichiGame_t qt_meta_stringdata_TaichiGame = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TaichiGame"
QT_MOC_LITERAL(1, 11, 15), // "SigTransferData"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "index"
QT_MOC_LITERAL(4, 34, 18), // "QList<QList<int> >"
QT_MOC_LITERAL(5, 53, 4), // "data"
QT_MOC_LITERAL(6, 58, 11), // "OnReadFrame"
QT_MOC_LITERAL(7, 70, 8), // "OnSwitch"
QT_MOC_LITERAL(8, 79, 21), // "OnSolveDemoMediaState"
QT_MOC_LITERAL(9, 101, 25), // "QMediaPlayer::MediaStatus"
QT_MOC_LITERAL(10, 127, 6), // "status"
QT_MOC_LITERAL(11, 134, 25), // "OnSolvePlayBackMediaState"
QT_MOC_LITERAL(12, 160, 20), // "OnToTheVeryBeginning"
QT_MOC_LITERAL(13, 181, 20), // "OnUpdateCurrentScore"
QT_MOC_LITERAL(14, 202, 5), // "score"
QT_MOC_LITERAL(15, 208, 29), // "EvaluateThread::MatchPosition"
QT_MOC_LITERAL(16, 238, 3), // "pos"
QT_MOC_LITERAL(17, 242, 22), // "OnMediaPositionChanged"
QT_MOC_LITERAL(18, 265, 7), // "positon"
QT_MOC_LITERAL(19, 273, 12), // "OnMoveSlider"
QT_MOC_LITERAL(20, 286, 13), // "OnMoveProgBar"
QT_MOC_LITERAL(21, 300, 20) // "OnReleaseVideoWriter"

    },
    "TaichiGame\0SigTransferData\0\0index\0"
    "QList<QList<int> >\0data\0OnReadFrame\0"
    "OnSwitch\0OnSolveDemoMediaState\0"
    "QMediaPlayer::MediaStatus\0status\0"
    "OnSolvePlayBackMediaState\0"
    "OnToTheVeryBeginning\0OnUpdateCurrentScore\0"
    "score\0EvaluateThread::MatchPosition\0"
    "pos\0OnMediaPositionChanged\0positon\0"
    "OnMoveSlider\0OnMoveProgBar\0"
    "OnReleaseVideoWriter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TaichiGame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   74,    2, 0x08 /* Private */,
       7,    0,   75,    2, 0x08 /* Private */,
       8,    1,   76,    2, 0x08 /* Private */,
      11,    1,   79,    2, 0x08 /* Private */,
      12,    0,   82,    2, 0x08 /* Private */,
      13,    2,   83,    2, 0x08 /* Private */,
      17,    1,   88,    2, 0x08 /* Private */,
      19,    0,   91,    2, 0x08 /* Private */,
      20,    0,   92,    2, 0x08 /* Private */,
      21,    0,   93,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,   14,   16,
    QMetaType::Void, QMetaType::LongLong,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TaichiGame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TaichiGame *_t = static_cast<TaichiGame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SigTransferData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QList<QList<int> >(*)>(_a[2]))); break;
        case 1: _t->OnReadFrame(); break;
        case 2: _t->OnSwitch(); break;
        case 3: _t->OnSolveDemoMediaState((*reinterpret_cast< QMediaPlayer::MediaStatus(*)>(_a[1]))); break;
        case 4: _t->OnSolvePlayBackMediaState((*reinterpret_cast< QMediaPlayer::MediaStatus(*)>(_a[1]))); break;
        case 5: _t->OnToTheVeryBeginning(); break;
        case 6: _t->OnUpdateCurrentScore((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< EvaluateThread::MatchPosition(*)>(_a[2]))); break;
        case 7: _t->OnMediaPositionChanged((*reinterpret_cast< qint64(*)>(_a[1]))); break;
        case 8: _t->OnMoveSlider(); break;
        case 9: _t->OnMoveProgBar(); break;
        case 10: _t->OnReleaseVideoWriter(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QList<int> > >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaPlayer::MediaStatus >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QMediaPlayer::MediaStatus >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TaichiGame::*_t)(int , QList<QList<int>> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TaichiGame::SigTransferData)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TaichiGame::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TaichiGame.data,
      qt_meta_data_TaichiGame,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TaichiGame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaichiGame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TaichiGame.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TaichiGame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void TaichiGame::SigTransferData(int _t1, QList<QList<int>> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
