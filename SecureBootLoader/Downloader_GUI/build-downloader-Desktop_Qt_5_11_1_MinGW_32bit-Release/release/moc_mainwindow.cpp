/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../downloader/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[18];
    char stringdata0[260];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 20), // "on_selecFile_clicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 35), // "on_downloadProgressBar_valueC..."
QT_MOC_LITERAL(4, 69, 5), // "value"
QT_MOC_LITERAL(5, 75, 24), // "on_SelectComPort_clicked"
QT_MOC_LITERAL(6, 100, 12), // "get_ComPorts"
QT_MOC_LITERAL(7, 113, 9), // "set_Comms"
QT_MOC_LITERAL(8, 123, 15), // "read_SerialData"
QT_MOC_LITERAL(9, 139, 8), // "uint32_t"
QT_MOC_LITERAL(10, 148, 4), // "data"
QT_MOC_LITERAL(11, 153, 5), // "bytes"
QT_MOC_LITERAL(12, 159, 24), // "on_downloadImage_clicked"
QT_MOC_LITERAL(13, 184, 35), // "on_SelectComPort_Disconnect_c..."
QT_MOC_LITERAL(14, 220, 12), // "Sendpreamble"
QT_MOC_LITERAL(15, 233, 11), // "notify_user"
QT_MOC_LITERAL(16, 245, 7), // "uint8_t"
QT_MOC_LITERAL(17, 253, 6) // "status"

    },
    "MainWindow\0on_selecFile_clicked\0\0"
    "on_downloadProgressBar_valueChanged\0"
    "value\0on_SelectComPort_clicked\0"
    "get_ComPorts\0set_Comms\0read_SerialData\0"
    "uint32_t\0data\0bytes\0on_downloadImage_clicked\0"
    "on_SelectComPort_Disconnect_clicked\0"
    "Sendpreamble\0notify_user\0uint8_t\0"
    "status"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    1,   65,    2, 0x08 /* Private */,
       5,    0,   68,    2, 0x08 /* Private */,
       6,    0,   69,    2, 0x08 /* Private */,
       7,    0,   70,    2, 0x08 /* Private */,
       8,    2,   71,    2, 0x08 /* Private */,
      12,    0,   76,    2, 0x08 /* Private */,
      13,    0,   77,    2, 0x08 /* Private */,
      14,    0,   78,    2, 0x08 /* Private */,
      15,    1,   79,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 9, QMetaType::VoidStar, 0x80000000 | 9,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 16, 0x80000000 | 16,   17,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_selecFile_clicked(); break;
        case 1: _t->on_downloadProgressBar_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_SelectComPort_clicked(); break;
        case 3: _t->get_ComPorts(); break;
        case 4: _t->set_Comms(); break;
        case 5: { uint32_t _r = _t->read_SerialData((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< uint32_t(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< uint32_t*>(_a[0]) = std::move(_r); }  break;
        case 6: _t->on_downloadImage_clicked(); break;
        case 7: _t->on_SelectComPort_Disconnect_clicked(); break;
        case 8: _t->Sendpreamble(); break;
        case 9: { uint8_t _r = _t->notify_user((*reinterpret_cast< uint8_t(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< uint8_t*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
