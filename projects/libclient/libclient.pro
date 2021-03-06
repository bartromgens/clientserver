TEMPLATE = lib
CONFIG += console
CONFIG -= qt

TARGET = client
DESTDIR = ../../lib

INCLUDEPATH += ../../src/
DEPENDPATH += $$INCLUDEPATH
VPATH += $$INCLUDEPATH

LIBS += -lboost_system
LIBS += -lpthread

SOURCES += \
    client/client.cpp \
    shared/message.cpp \

HEADERS += \
    client/client.h \
    shared/clientserverdata.h \
    shared/message.h \

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
