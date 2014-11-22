TEMPLATE = lib
CONFIG += console
CONFIG -= qt

TARGET = server
DESTDIR = ../../lib

INCLUDEPATH += ../../src/
DEPENDPATH = $$INCLUDEPATH
VPATH += $$INCLUDEPATH

LIBS += -lboost_system
LIBS += -lpthread

SOURCES += \
    server/server.cpp \
    server/connectionstatus.cpp \
    server/serverobserver.cpp \

HEADERS += \
    server/server.h \
    server/connectionstatus.h \
    shared/clientserverdata.h \
    server/serverobserver.h \

CONFIG(debug, debug|release) {
  OBJECTS_DIR = ./tmp/debug
} else {
  OBJECTS_DIR = ./tmp/release
}

UI_DIR += ./generated
MOC_DIR += ./generated

QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -Wall

CONFIG += silent
