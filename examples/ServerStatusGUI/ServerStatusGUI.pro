#-------------------------------------------------
#
# Project created by QtCreator 2012-12-15T14:10:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerStatusGUI
TEMPLATE = app

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
MAKEFILE += MakefileServerStatusGUI
