/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,    8,    7,    7, 0x08,
      63,    8,    7,    7, 0x08,
     104,    8,    7,    7, 0x08,
     151,  146,    7,    7, 0x08,
     187,  146,    7,    7, 0x08,
     223,    7,    7,    7, 0x08,
     257,    7,    7,    7, 0x08,
     290,    7,    7,    7, 0x08,
     327,    7,    7,    7, 0x08,
     363,    7,    7,    7, 0x08,
     397,    7,    7,    7, 0x08,
     434,    7,    7,    7, 0x08,
     463,    7,    7,    7, 0x08,
     493,    7,    7,    7, 0x08,
     522,    7,    7,    7, 0x08,
     552,    7,    7,    7, 0x08,
     571,  562,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0index\0"
    "on_comboBox_startSector_currentIndexChanged(int)\0"
    "on_comboBox_num_currentIndexChanged(int)\0"
    "on_comboBox_port_currentIndexChanged(int)\0"
    "arg1\0on_checkBox_cycle_stateChanged(int)\0"
    "on_checkBox_check_stateChanged(int)\0"
    "on_radioButton_sendText_clicked()\0"
    "on_radioButton_sendHex_clicked()\0"
    "on_radioButton_receiveText_clicked()\0"
    "on_radioButton_receiveHex_clicked()\0"
    "on_pushButton_sendClear_clicked()\0"
    "on_pushButton_receiveClear_clicked()\0"
    "on_pushButton_open_clicked()\0"
    "on_pushButton_close_clicked()\0"
    "on_pushButton_read_clicked()\0"
    "on_pushButton_write_clicked()\0writeOK()\0"
    "readData\0readOK(QByteArray)\0"
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->on_comboBox_startSector_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_comboBox_num_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_comboBox_port_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_checkBox_cycle_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_checkBox_check_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_radioButton_sendText_clicked(); break;
        case 6: _t->on_radioButton_sendHex_clicked(); break;
        case 7: _t->on_radioButton_receiveText_clicked(); break;
        case 8: _t->on_radioButton_receiveHex_clicked(); break;
        case 9: _t->on_pushButton_sendClear_clicked(); break;
        case 10: _t->on_pushButton_receiveClear_clicked(); break;
        case 11: _t->on_pushButton_open_clicked(); break;
        case 12: _t->on_pushButton_close_clicked(); break;
        case 13: _t->on_pushButton_read_clicked(); break;
        case 14: _t->on_pushButton_write_clicked(); break;
        case 15: _t->writeOK(); break;
        case 16: _t->readOK((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Widget,
      qt_meta_data_Widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Widget))
        return static_cast<void*>(const_cast< Widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
