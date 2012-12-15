#-------------------------------------------------
#
# Project created by QtCreator 2012-12-09T22:20:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientStatusGUI
TEMPLATE = app

# files
SOURCES += main.cpp\
           clientstatuswindow.cpp \
           client/client.cpp \

HEADERS  += clientstatuswindow.h \
            client/client.h \

FORMS    += clientstatuswindow.ui \

#include general example project settings
OTHER_FILES += \
    ../example_settings.pri

include(../example_settings.pri)

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileClientStatusGUI

