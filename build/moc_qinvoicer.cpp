/****************************************************************************
** Meta object code from reading C++ file 'qinvoicer.h'
**
** Created: Tue Oct 25 18:22:24 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/view/qinvoicer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qinvoicer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_View__QInvoicer[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      37,   16,   16,   16, 0x08,
      56,   16,   16,   16, 0x08,
      70,   16,   16,   16, 0x08,
      86,   16,   16,   16, 0x08,
     100,   16,   16,   16, 0x08,
     120,   16,   16,   16, 0x08,
     139,   16,   16,   16, 0x08,
     155,   16,   16,   16, 0x08,
     172,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_View__QInvoicer[] = {
    "View::QInvoicer\0\0createSaleInvoice()\0"
    "createBuyInvoice()\0loadInvoice()\0"
    "createProduct()\0loadProduct()\0"
    "volumeSaleInvoice()\0volumeBuyInvoice()\0"
    "volumeInvoice()\0unpaidInvoices()\0"
    "about()\0"
};

const QMetaObject View::QInvoicer::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_View__QInvoicer,
      qt_meta_data_View__QInvoicer, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &View::QInvoicer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *View::QInvoicer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *View::QInvoicer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_View__QInvoicer))
        return static_cast<void*>(const_cast< QInvoicer*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int View::QInvoicer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: createSaleInvoice(); break;
        case 1: createBuyInvoice(); break;
        case 2: loadInvoice(); break;
        case 3: createProduct(); break;
        case 4: loadProduct(); break;
        case 5: volumeSaleInvoice(); break;
        case 6: volumeBuyInvoice(); break;
        case 7: volumeInvoice(); break;
        case 8: unpaidInvoices(); break;
        case 9: about(); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
