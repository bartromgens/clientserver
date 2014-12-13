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
        server/connectionstatus.cpp \
        shared/messagejson.cpp \
        server/serverjson.cpp \

HEADERS  += \
        server/connectionstatus.h \
        shared/messagejson.h \
        server/serverjson.h \

OTHER_FILES += \
    ../common_settings.pri \

CONFIG += silent
QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
