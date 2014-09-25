QT       += core gui

TARGET = clientgui
TEMPLATE = app

INCLUDEPATH = ../../../include

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lclient

include(../example_settings.pri)

# files
SOURCES += main.cpp\
           clientwindow.cpp \

HEADERS  += clientwindow.h \

FORMS    += clientwindow.ui \

#include general example project settings
OTHER_FILES += \
    ../example_settings.pri

OBJECTS_DIR += ./tmp/objects/

CONFIG += silent

