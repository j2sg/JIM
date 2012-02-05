######################################################################
# Automatically generated by qmake (2.01a) vie ene 27 11:14:25 2012
######################################################################

TEMPLATE = app
TARGET = qinvoicer
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
QT += sql
DEPENDPATH += . \
              include \
              resources \
              src \
              include/persistence \
              include/view \
              src/persistence \
              src/view \
              include/model/domain \
              include/model/management \
              include/view/invoicing \
              include/view/management \
              src/model/domain \
              src/model/management \
              src/view/invoicing \
              src/view/management
INCLUDEPATH += . \
               include/view \
               include/persistence \
               include \
               include/view/invoicing \
               include/view/management \
               include/model/domain \
               include/model/management

# Input
HEADERS += include/global.h \
           include/types.h \
           include/persistence/persistencemanager.h \
           include/persistence/sqlagent.h \
           include/view/businessloader.h \
           include/view/invoiceloader.h \
           include/view/qinvoicer.h \
           include/model/domain/business.h \
           include/model/domain/category.h \
           include/model/domain/entity.h \
           include/model/domain/invoice.h \
           include/model/domain/operation.h \
           include/model/domain/ostream.h \
           include/model/domain/product.h \
           include/model/domain/tax.h \
           include/model/management/businessmanager.h \
           include/model/management/categorymanager.h \
           include/model/management/entitymanager.h \
           include/model/management/invoicemanager.h \
           include/model/management/operationmanager.h \
           include/model/management/productmanager.h \
           include/model/management/taxmanager.h \
           include/view/invoicing/invoiceeditor.h \
           include/view/invoicing/invoiceeditordatatab.h \
           include/view/invoicing/invoiceeditorothertab.h \
           include/view/invoicing/operationeditor.h \
           include/view/invoicing/operationmodel.h \
           include/view/invoicing/operationtable.h \
           include/view/invoicing/taxviewerwidget.h \
           include/view/management/categorydialog.h \
           include/view/management/categorymodel.h \
           include/view/management/entitydialog.h \
           include/view/management/entitydialogdatatab.h \
           include/view/management/entitydialogothertab.h \
           include/view/management/entitydialogtaxestab.h \
           include/view/management/entityeditor.h \
           include/view/management/entitymodel.h \
           include/view/management/productdialog.h \
           include/view/management/producteditor.h \
           include/view/management/productmodel.h \
           include/view/management/taxapplyingwidget.h \
           include/view/management/taxwidget.h \
    include/view/management/entityviewer.h \
    include/view/management/entityselector.h
SOURCES += src/main.cpp \
           src/persistence/persistencemanager.cpp \
           src/persistence/sqlagent.cpp \
           src/view/businessloader.cpp \
           src/view/invoiceloader.cpp \
           src/view/qinvoicer.cpp \
           src/model/domain/business.cpp \
           src/model/domain/category.cpp \
           src/model/domain/entity.cpp \
           src/model/domain/invoice.cpp \
           src/model/domain/operation.cpp \
           src/model/domain/ostream.cpp \
           src/model/domain/product.cpp \
           src/model/domain/tax.cpp \
           src/model/management/businessmanager.cpp \
           src/model/management/categorymanager.cpp \
           src/model/management/entitymanager.cpp \
           src/model/management/invoicemanager.cpp \
           src/model/management/operationmanager.cpp \
           src/model/management/productmanager.cpp \
           src/model/management/taxmanager.cpp \
           src/view/invoicing/invoiceeditor.cpp \
           src/view/invoicing/invoiceeditordatatab.cpp \
           src/view/invoicing/invoiceeditorothertab.cpp \
           src/view/invoicing/operationeditor.cpp \
           src/view/invoicing/operationmodel.cpp \
           src/view/invoicing/operationtable.cpp \
           src/view/invoicing/taxviewerwidget.cpp \
           src/view/management/categorydialog.cpp \
           src/view/management/categorymodel.cpp \
           src/view/management/entitydialog.cpp \
           src/view/management/entitydialogdatatab.cpp \
           src/view/management/entitydialogothertab.cpp \
           src/view/management/entitydialogtaxestab.cpp \
           src/view/management/entityeditor.cpp \
           src/view/management/entitymodel.cpp \
           src/view/management/productdialog.cpp \
           src/view/management/producteditor.cpp \
           src/view/management/productmodel.cpp \
           src/view/management/taxapplyingwidget.cpp \
           src/view/management/taxwidget.cpp \
    src/view/management/entityviewer.cpp \
    src/view/management/entityselector.cpp
RESOURCES += resources/images.qrc




