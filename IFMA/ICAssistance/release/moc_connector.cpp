/****************************************************************************
** Meta object code from reading C++ file 'connector.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../connector.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connector.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Connector[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,
      25,   10,   10,   10, 0x05,
      44,   35,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   10,   10,   10, 0x08,
      81,   10,   10,   10, 0x08,
     108,   98,   10,   10, 0x0a,
     133,   10,   10,   10, 0x0a,
     147,   98,   10,   10, 0x0a,
     170,   10,   10,   10, 0x0a,
     196,  181,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Connector[] = {
    "Connector\0\0readAllData()\0writeOK()\0"
    "readData\0readOK(QByteArray)\0"
    "handleReadyRead()\0handleReadData()\0"
    "start,num\0callForData(qint8,qint8)\0"
    "callForWait()\0startRead(qint8,qint8)\0"
    "stopRead()\0start,num,data\0"
    "startWrite(qint8,qint8,QByteArray)\0"
};

void Connector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Connector *_t = static_cast<Connector *>(_o);
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
    }
}

const QMetaObjectExtraData Connector::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Connector::staticMetaObject = {
    { &QSerialPort::staticMetaObject, qt_meta_stringdata_Connector,
      qt_meta_data_Connector, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Connector::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Connector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Connector::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Connector))
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
    }
    return _id;
}

// SIGNAL 0
void Connector::readAllData()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Connector::writeOK()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void Connector::readOK(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
