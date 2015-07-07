######################################################################
# Automatically generated by qmake (2.01a) jue mar 1 15:48:05 2012
######################################################################

TEMPLATE = app
TARGET = jim
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
QT += sql

greaterThan(QT_MAJOR_VERSION, 4) {
   QT += widgets printsupport
}

DEPENDPATH += . \
              include \
              resources \
              src \
              include/persistence \
              include/printing \
              include/view \
              resources/translations \
              src/persistence \
              src/printing \
              src/view \
              include/model/domain \
              include/model/management \
              include/model/report \
              include/view/invoicing \
              include/view/management \
              include/view/report \
              src/model/domain \
              src/model/management \
              src/model/report \
              src/view/invoicing \
              src/view/management \
              src/view/report

INCLUDEPATH += . \
               include/view \
               include \
               include/persistence \
               include/printing \
               include/model/domain \
               include/view/invoicing \
               include/view/management \
               include/model/management \
               include/view/report \
               include/model/report

# Input
HEADERS += include/global.h \
           include/types.h \
           include/persistence/persistencemanager.h \
           include/persistence/sqlagent.h \
           include/printing/printingmanager.h \
           include/view/authdialog.h \
           include/view/businessloader.h \
           include/view/invoiceloader.h \
           include/view/mainwindow.h \
           include/view/optionsdialog.h \
           include/view/registerdialog.h \
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
           include/model/report/reportmanager.h \
           include/view/invoicing/invoicedelegate.h \
           include/view/invoicing/invoiceeditor.h \
           include/view/invoicing/invoiceeditordatatab.h \
           include/view/invoicing/invoiceeditorothertab.h \
           include/view/invoicing/invoicemodel.h \
           include/view/invoicing/invoicesearch.h \
           include/view/invoicing/invoicesearchresult.h \
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
           include/view/management/entityselector.h \
           include/view/management/entityviewer.h \
           include/view/management/productdialog.h \
           include/view/management/producteditor.h \
           include/view/management/productmodel.h \
           include/view/management/taxapplyingwidget.h \
           include/view/management/taxwidget.h \
           include/view/report/unpaidsreport.h \
           include/view/report/unpaidsreporttab.h \
           include/view/report/volumereport.h \
           include/view/report/volumereportbydatemodel.h \
           include/view/report/volumereportbydatetab.h \
           include/view/report/volumereportbyentitymodel.h \
           include/view/report/volumereportbyentitytab.h \
           include/view/report/volumereportbyproductmodel.h \
           include/view/report/volumereportbyproducttab.h \
           include/view/report/volumereportdialog.h \
    include/view/management/productproxymodel.h \
    include/view/invoicing/operationeditoridwidget.h \
    include/view/invoicing/operationdelegate.h \
    include/view/management/productselector.h

SOURCES += src/main.cpp \
           src/persistence/persistencemanager.cpp \
           src/persistence/sqlagent.cpp \
           src/printing/printingmanager.cpp \
           src/view/authdialog.cpp \
           src/view/businessloader.cpp \
           src/view/invoiceloader.cpp \
           src/view/mainwindow.cpp \
           src/view/optionsdialog.cpp \
           src/view/registerdialog.cpp \
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
           src/model/report/reportmanager.cpp \
           src/view/invoicing/invoicedelegate.cpp \
           src/view/invoicing/invoiceeditor.cpp \
           src/view/invoicing/invoiceeditordatatab.cpp \
           src/view/invoicing/invoiceeditorothertab.cpp \
           src/view/invoicing/invoicemodel.cpp \
           src/view/invoicing/invoicesearch.cpp \
           src/view/invoicing/invoicesearchresult.cpp \
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
           src/view/management/entityselector.cpp \
           src/view/management/entityviewer.cpp \
           src/view/management/productdialog.cpp \
           src/view/management/producteditor.cpp \
           src/view/management/productmodel.cpp \
           src/view/management/taxapplyingwidget.cpp \
           src/view/management/taxwidget.cpp \
           src/view/report/unpaidsreport.cpp \
           src/view/report/unpaidsreporttab.cpp \
           src/view/report/volumereport.cpp \
           src/view/report/volumereportbydatemodel.cpp \
           src/view/report/volumereportbydatetab.cpp \
           src/view/report/volumereportbyentitymodel.cpp \
           src/view/report/volumereportbyentitytab.cpp \
           src/view/report/volumereportbyproductmodel.cpp \
           src/view/report/volumereportbyproducttab.cpp \
           src/view/report/volumereportdialog.cpp \
    src/view/management/productproxymodel.cpp \
    src/view/invoicing/operationeditoridwidget.cpp \
    src/view/invoicing/operationdelegate.cpp \
    src/view/management/productselector.cpp

RESOURCES += resources/jim.qrc

TRANSLATIONS += resources/translations/jim_es.ts

OTHER_FILES += \
    doc/about.txt \
    sql/sqlite.sql \
    THANKS \
    README \
    LICENSE \
    jim.desktop \
    INSTALL \
    COPYRIGHT \
    CHANGELOG \
    AUTHOR

message("Qt Version: $$QT_VERSION")
