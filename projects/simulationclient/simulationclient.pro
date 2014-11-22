QT       += core gui

TARGET = simulationclient
TEMPLATE = app

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lclient

#include general example project settings
include(../common_settings.pri)

SOURCES += \
        main.cpp \
        clientwindow.cpp \
        client/client.cpp \
        ../simulationserver/message.cpp \

HEADERS  += \
        clientwindow.h \
        ../simulationserver/message.h \

FORMS    +=  clientwindow.ui

OTHER_FILES += \
    ../common_settings.pri \

CONFIG += silent
