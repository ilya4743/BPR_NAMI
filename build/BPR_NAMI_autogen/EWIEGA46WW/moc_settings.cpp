/****************************************************************************
** Meta object code from reading C++ file 'settings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "settings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Settings_t {
    QByteArrayData data[11];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Settings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Settings_t qt_meta_stringdata_Settings = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Settings"
QT_MOC_LITERAL(1, 9, 7), // "Section"
QT_MOC_LITERAL(2, 17, 7), // "General"
QT_MOC_LITERAL(3, 25, 7), // "Network"
QT_MOC_LITERAL(4, 33, 5), // "Debug"
QT_MOC_LITERAL(5, 39, 3), // "Key"
QT_MOC_LITERAL(6, 43, 9), // "IP_SERVER"
QT_MOC_LITERAL(7, 53, 4), // "PORT"
QT_MOC_LITERAL(8, 58, 12), // "DEBUG_OUTPUT"
QT_MOC_LITERAL(9, 71, 9), // "PRINT_LOG"
QT_MOC_LITERAL(10, 81, 14) // "RECONNECT_TIME"

    },
    "Settings\0Section\0General\0Network\0Debug\0"
    "Key\0IP_SERVER\0PORT\0DEBUG_OUTPUT\0"
    "PRINT_LOG\0RECONNECT_TIME"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Settings[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       2,   14, // enums/sets
       0,    0, // constructors
       4,       // flags
       0,       // signalCount

 // enums: name, alias, flags, count, data
       1,    1, 0x0,    3,   24,
       5,    5, 0x0,    5,   30,

 // enum data: key, value
       2, uint(Settings::General),
       3, uint(Settings::Network),
       4, uint(Settings::Debug),
       6, uint(Settings::IP_SERVER),
       7, uint(Settings::PORT),
       8, uint(Settings::DEBUG_OUTPUT),
       9, uint(Settings::PRINT_LOG),
      10, uint(Settings::RECONNECT_TIME),

       0        // eod
};

QT_INIT_METAOBJECT const QMetaObject Settings::staticMetaObject = { {
    nullptr,
    qt_meta_stringdata_Settings.data,
    qt_meta_data_Settings,
    nullptr,
    nullptr,
    nullptr
} };

QT_WARNING_POP
QT_END_MOC_NAMESPACE
