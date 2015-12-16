/****************************************************************************
** Meta object code from reading C++ file 'connector.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../connector.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Connector_t {
    QByteArrayData data[16];
    char stringdata0[147];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Connector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Connector_t qt_meta_stringdata_Connector = {
    {
QT_MOC_LITERAL(0, 0, 9), // "Connector"
QT_MOC_LITERAL(1, 10, 11), // "readAllData"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "writeOK"
QT_MOC_LITERAL(4, 31, 6), // "readOK"
QT_MOC_LITERAL(5, 38, 8), // "readData"
QT_MOC_LITERAL(6, 47, 15), // "handleReadyRead"
QT_MOC_LITERAL(7, 63, 14), // "handleReadData"
QT_MOC_LITERAL(8, 78, 11), // "callForData"
QT_MOC_LITERAL(9, 90, 5), // "start"
QT_MOC_LITERAL(10, 96, 3), // "num"
QT_MOC_LITERAL(11, 100, 11), // "callForWait"
QT_MOC_LITERAL(12, 112, 9), // "startRead"
QT_MOC_LITERAL(13, 122, 8), // "stopRead"
QT_MOC_LITERAL(14, 131, 10), // "startWrite"
QT_MOC_LITERAL(15, 142, 4) // "data"

    },
    "Connector\0readAllData\0\0writeOK\0readOK\0"
    "readData\0handleReadyRead\0handleReadData\0"
    "callForData\0start\0num\0callForWait\0"
    "startRead\0stopRead\0startWrite\0data"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Connector[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    1,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   69,    2, 0x08 /* Private */,
       7,    0,   70,    2, 0x08 /* Private */,
       8,    2,   71,    2, 0x0a /* Public */,
      11,    0,   76,    2, 0x0a /* Public */,
      12,    2,   77,    2, 0x0a /* Public */,
      13,    0,   82,    2, 0x0a /* Public */,
      14,    3,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::SChar, QMetaType::SChar,    9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::SChar, QMetaType::SChar,    9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::SChar, QMetaType::SChar, QMetaType::QByteArray,    9,   10,   15,

       0        // eod
};

void Connector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Connector *_t = static_cast<Connector *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readAllData(); break;
        case 1: _t->writeOK(); break;
        case 2: _t->readOK((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->handleReadyRead(); break;
        case 4: _t->handleReadData(); break;
        case 5: _t->callForData((*reinterpret_cast< qint8(*)>(_a[1])),(*reinterpret_cast< qint8(*)>(_a[2]))); break;
        case 6: _t->callForWait(); break;
        case 7: _t->startRead((*reinterpret_cast< qint8(*)>(_a[1])),(*reinterpret_cast< qint8(*)>(_a[2]))); break;
        case 8: _t->stopRead(); break;
        case 9: _t->startWrite((*reinterpret_cast< qint8(*)>(_a[1])),(*reinterpret_cast< qint8(*)>(_a[2])),(*reinterpret_cast< QByteArray(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Connector::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Connector::readAllData)) {
                *result = 0;
            }
        }
        {
            typedef void (Connector::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Connector::writeOK)) {
                *result = 1;
            }
        }
        {
            typedef void (Connector::*_t)(QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Connector::readOK)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject Connector::staticMetaObject = {
    { &QSerialPort::staticMetaObject, qt_meta_stringdata_Connector.data,
      qt_meta_data_Connector,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Connector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Connector::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Connector.stringdata0))
        return static_cast<void*>(const_cast< Connector*>(this));
    return QSerialPort::qt_metacast(_clname);
}

int Connector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSerialPort::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void Connector::readAllData()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void Connector::writeOK()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void Connector::readOK(QByteArray _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
