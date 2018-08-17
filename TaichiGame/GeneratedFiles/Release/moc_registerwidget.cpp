/****************************************************************************
** Meta object code from reading C++ file 'registerwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../registerwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'registerwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RegisterWidget_t {
    QByteArrayData data[9];
    char stringdata0[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RegisterWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RegisterWidget_t qt_meta_stringdata_RegisterWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "RegisterWidget"
QT_MOC_LITERAL(1, 15, 17), // "nextButtonClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 9), // "OneToNext"
QT_MOC_LITERAL(4, 44, 4), // "text"
QT_MOC_LITERAL(5, 49, 9), // "TwoToNext"
QT_MOC_LITERAL(6, 59, 11), // "ThreeToNext"
QT_MOC_LITERAL(7, 71, 18), // "judgeInputFinished"
QT_MOC_LITERAL(8, 90, 10) // "registerMe"

    },
    "RegisterWidget\0nextButtonClicked\0\0"
    "OneToNext\0text\0TwoToNext\0ThreeToNext\0"
    "judgeInputFinished\0registerMe"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RegisterWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   45,    2, 0x08 /* Private */,
       5,    1,   48,    2, 0x08 /* Private */,
       6,    1,   51,    2, 0x08 /* Private */,
       7,    1,   54,    2, 0x08 /* Private */,
       8,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,

       0        // eod
};

void RegisterWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RegisterWidget *_t = static_cast<RegisterWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->nextButtonClicked(); break;
        case 1: _t->OneToNext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->TwoToNext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->ThreeToNext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->judgeInputFinished((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->registerMe(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RegisterWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RegisterWidget::nextButtonClicked)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RegisterWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RegisterWidget.data,
      qt_meta_data_RegisterWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RegisterWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RegisterWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RegisterWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RegisterWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void RegisterWidget::nextButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
