QT       += core gui

TARGET = tests
TEMPLATE = app

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lserver
LIBS += -lclient

#include general example project settings
include(../common_settings.pri)
VPATH += $$INCLUDEPATH

SOURCES += \
        main.cpp \
        ../simulationserver/simulationserver.cpp

HEADERS  += \
        ../simulationserver/simulationserver.h

OTHER_FILES += \
    ../common_settings.pri \

CONFIG += silent

DEFINES -= NDEBUG
