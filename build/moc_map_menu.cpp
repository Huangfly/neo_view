/****************************************************************************
** Meta object code from reading C++ file 'map_menu.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../map_menu.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'map_menu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_map_menu_t {
    QByteArrayData data[9];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_map_menu_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_map_menu_t qt_meta_stringdata_map_menu = {
    {
QT_MOC_LITERAL(0, 0, 8), // "map_menu"
QT_MOC_LITERAL(1, 9, 8), // "OnPatrol"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 20), // "OnNav_NavigationMode"
QT_MOC_LITERAL(4, 40, 17), // "OnNav_ExploreMode"
QT_MOC_LITERAL(5, 58, 12), // "OnClearGoals"
QT_MOC_LITERAL(6, 71, 12), // "OnEditChange"
QT_MOC_LITERAL(7, 84, 14), // "OnSlam_SaveMap"
QT_MOC_LITERAL(8, 99, 15) // "OnSlam_ClearMap"

    },
    "map_menu\0OnPatrol\0\0OnNav_NavigationMode\0"
    "OnNav_ExploreMode\0OnClearGoals\0"
    "OnEditChange\0OnSlam_SaveMap\0OnSlam_ClearMap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_map_menu[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x0a /* Public */,
       6,    0,   53,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    0,   55,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void map_menu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        map_menu *_t = static_cast<map_menu *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnPatrol(); break;
        case 1: _t->OnNav_NavigationMode(); break;
        case 2: _t->OnNav_ExploreMode(); break;
        case 3: _t->OnClearGoals(); break;
        case 4: _t->OnEditChange(); break;
        case 5: _t->OnSlam_SaveMap(); break;
        case 6: _t->OnSlam_ClearMap(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject map_menu::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_map_menu.data,
      qt_meta_data_map_menu,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *map_menu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *map_menu::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_map_menu.stringdata0))
        return static_cast<void*>(const_cast< map_menu*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int map_menu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
