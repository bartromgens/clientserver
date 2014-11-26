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
    server/connectionstatus.cpp \
    server/server.cpp \
    server/serverobserver.cpp \
    shared/message.cpp \
    shared/messagejson.cpp \

HEADERS += \
    server/connectionstatus.h \
    server/server.h \
    server/serverobserver.h \
    shared/clientserverdata.h \
    shared/message.h \
    shared/messagejson.h \

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
