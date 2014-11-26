QT       += core gui

TARGET = simulationclient
TEMPLATE = app

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lclient

#include general example project settings
include(../common_settings.pri)
VPATH += $$INCLUDEPATH

SOURCES += \
        main.cpp \
        clientwindow.cpp \

HEADERS  += \
        clientwindow.h \

FORMS    +=  clientwindow.ui

OTHER_FILES += \
    ../common_settings.pri \

CONFIG += silent
