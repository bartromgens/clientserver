#-------------------------------------------------
#
# Project created by QtCreator 2012-12-09T22:20:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientStatusGUI
TEMPLATE = app

# add directory of client and server sources
INCLUDEPATH += ../../src/
DEPENDPATH = $$INCLUDEPATH

# files
SOURCES += main.cpp\
           clientstatuswindow.cpp \
           client/client.cpp \

HEADERS  += clientstatuswindow.h \
            client/client.h \

FORMS    += clientstatuswindow.ui \

OTHER_FILES +=

# boost libraries
INCLUDEPATH += /home/bart/prog/1_tools/boost_1_52_0
LIBS += -L/home/bart/prog/1_tools/boost_1_52_0/stage/lib
LIBS += -lboost_system -lpthread
LIBS += -lpthread

OBJECTS_DIR += ./tmp/objects/
MAKEFILE += MakefileClientStatusGUI

# compiler settings
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

# linker settings
