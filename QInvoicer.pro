######################################################################
# Automatically generated by qmake (2.01a) s�b oct 29 12:10:45 2011
######################################################################

TEMPLATE = app
TARGET = qinvoicer
QT += sql
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
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
              src/model/domain \
              src/model/management
INCLUDEPATH += . \
               include/view \
               include \
               include/persistence \
               include/model/domain \
               include/model/management

# Input
HEADERS += include/global.h \
           include/types.h \
           include/persistence/persistencemanager.h \
           include/persistence/sqlagent.h \
           include/view/invoiceeditor.h \
           include/view/operationeditor.h \
           include/view/operationmodel.h \
           include/view/qinvoicer.h \
           include/model/domain/invoice.h \
           include/model/domain/operation.h \
           include/model/domain/product.h \
           include/model/management/invoicemanager.h \
           include/model/management/operationmanager.h \
           include/model/management/productmanager.h
SOURCES += src/main.cpp \
           src/persistence/persistencemanager.cpp \
           src/persistence/sqlagent.cpp \
           src/view/invoiceeditor.cpp \
           src/view/operationeditor.cpp \
           src/view/operationmodel.cpp \
           src/view/qinvoicer.cpp \
           src/model/domain/invoice.cpp \
           src/model/domain/operation.cpp \
           src/model/domain/product.cpp \
           src/model/management/invoicemanager.cpp \
           src/model/management/operationmanager.cpp \
           src/model/management/productmanager.cpp
RESOURCES += resources/images.qrc
