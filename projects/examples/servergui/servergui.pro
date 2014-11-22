QT       += core gui

TARGET = servergui
TEMPLATE = app

#include general example project settings
include(../example_settings.pri)
VPATH += $$INCLUDEPATH

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

CONFIG += silent
