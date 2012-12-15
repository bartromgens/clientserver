#-------------------------------------------------
#
# Project created by QtCreator 2012-12-15T14:10:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerStatusGUI
TEMPLATE = app


SOURCES += main.cpp\
        serverstatuswindow.cpp

HEADERS  += serverstatuswindow.h

FORMS    += serverstatuswindow.ui

OTHER_FILES += \
    ../example_settings.pri

#include general example project settings
include(../example_settings.pri)

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileServerStatusGUI
