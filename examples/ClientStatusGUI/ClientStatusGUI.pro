#-------------------------------------------------
#
# Project created by QtCreator 2012-12-09T22:20:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientStatusGUI
TEMPLATE = app

INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0

LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/lib
LIBS += -lboost_system -lpthread
LIBS += -lpthread

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileClientStatusGUI

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall


SOURCES += main.cpp\
        clientstatuswindow.cpp \
    ../../src/client/client.cpp

HEADERS  += clientstatuswindow.h \
    ../../src/client/client.h

FORMS    += clientstatuswindow.ui

OTHER_FILES +=
