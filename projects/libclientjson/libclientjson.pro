TEMPLATE = lib
QT       += core gui

TARGET = clientjson
DESTDIR = ../../lib

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lclient

#include general example project settings
include(../common_settings.pri)
VPATH += $$INCLUDEPATH

SOURCES += \

HEADERS  += \

OTHER_FILES += \
    ../common_settings.pri \

CONFIG += silent

