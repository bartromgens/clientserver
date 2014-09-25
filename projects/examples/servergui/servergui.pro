QT       += core gui

TARGET = servergui
TEMPLATE = app

INCLUDEPATH = ../../../include

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lserver

#include general example project settings
include(../example_settings.pri)

SOURCES += main.cpp \
        serverwindow.cpp \
        calculator.cpp \
        server/connectionstatus.cpp \
        server/server.cpp \
        server/serverobserver.cpp \
        gui/connectionstatustablewidget.cpp \

HEADERS  += serverwindow.h \
        calculator.h \
        server/connectionstatus.h \
        server/server.h \
        server/serverobserver.h \
        gui/connectionstatustablewidget.h \

FORMS    += serverwindow.ui

OTHER_FILES += \
    ../example_settings.pri

OBJECTS_DIR += ./tmp/objects/
