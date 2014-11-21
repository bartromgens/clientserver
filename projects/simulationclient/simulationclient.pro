QT       += core gui

TARGET = simulationclient
TEMPLATE = app

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lclient

#include general example project settings
include(example_settings.pri)

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
    ../example_settings.pri

OBJECTS_DIR += ./tmp/objects/

CONFIG += silent
