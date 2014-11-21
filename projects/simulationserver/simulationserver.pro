QT       += core gui

TARGET = simulationserver
TEMPLATE = app

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lserver

#include general example project settings
include(example_settings.pri)

SOURCES += main.cpp \
        serverwindow.cpp \
        gui/connectionstatustablewidget.cpp \
        simulationserver.cpp \
        message.cpp \

HEADERS  += serverwindow.h \
        server/connectionstatus.h \
        gui/connectionstatustablewidget.h \
        simulationserver.h \
        message.h \

FORMS    += serverwindow.ui

OTHER_FILES += \
    ../example_settings.pri \
    test.json

OBJECTS_DIR += ./tmp/objects/

CONFIG += silent
