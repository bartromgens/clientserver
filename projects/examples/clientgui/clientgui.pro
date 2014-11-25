QT       += core gui

TARGET = clientgui
TEMPLATE = app

LIBS += -L/home/bart/dev/clientserver/lib
LIBS += -lclient

#include general example project settings
include(../example_settings.pri)
VPATH += $$INCLUDEPATH

# files
SOURCES += main.cpp\
           clientwindow.cpp \

HEADERS  += clientwindow.h \

FORMS    += clientwindow.ui \

#include general example project settings
OTHER_FILES += \
    ../example_settings.pri

CONFIG += silent

