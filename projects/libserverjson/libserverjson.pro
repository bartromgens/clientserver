TEMPLATE = lib
QT       += core gui

TARGET = serverjson
DESTDIR = ../../lib

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lserver

#include general example project settings
include(../common_settings.pri)
VPATH += $$INCLUDEPATH

SOURCES += \
        main.cpp \
        serverwindow.cpp \
        gui/connectionstatustablewidget.cpp \
        server/serverjson.cpp \
        shared/messagejson.cpp \

HEADERS  += \
        serverwindow.h \
        server/connectionstatus.h \
        gui/connectionstatustablewidget.h \
        server/serverjson.h \
        shared/messagejson.h \

FORMS    += serverwindow.ui

OTHER_FILES += \
    ../common_settings.pri \

CONFIG += silent
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
