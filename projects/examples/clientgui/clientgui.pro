QT       += core gui

TARGET = clientgui
TEMPLATE = app

include(../example_settings.pri)

# files
SOURCES += main.cpp\
           clientwindow.cpp \

HEADERS  += clientwindow.h \

FORMS    += clientwindow.ui \

#include general example project settings
OTHER_FILES += \
    ../example_settings.pri

CONFIG += silent

