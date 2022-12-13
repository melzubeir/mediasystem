/****************************************************************************
** Meta object code from reading C++ file 'emsocrdialog.h'
**
** Created: Mon 8. Mar 12:01:43 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "./emsocrdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emsocrdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_emsOCRDialog[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      39,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_emsOCRDialog[] = {
    "emsOCRDialog\0\0on_startButton_clicked()\0"
    "threadTerminated()\0"
};

const QMetaObject emsOCRDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_emsOCRDialog,
      qt_meta_data_emsOCRDialog, 0 }
};

const QMetaObject *emsOCRDialog::metaObject() const
{
    return &staticMetaObject;
}

void *emsOCRDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_emsOCRDialog))
        return static_cast<void*>(const_cast< emsOCRDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int emsOCRDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_startButton_clicked(); break;
        case 1: threadTerminated(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
